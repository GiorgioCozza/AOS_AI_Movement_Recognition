# IMPORT
import numpy as np
import keras
from keras.models import Sequential
from keras.layers import Dense, Conv2D, MaxPooling2D, Dropout, Flatten
from keras.callbacks import CSVLogger
from keras.optimizers import SGD, Adam
from scripts.dataset_processing import get_dataset, normalize, prepare_data, split_train_test
import seaborn as sns
import datetime
import os

from sklearn import metrics

from matplotlib import pyplot as plt

# output labels
LABELS = ["running",
          "walking",
          "jumping",
          "standing",
          "football",
          "basketball"]

SENSORS = ["ACC_LSM6DSL_DS", "GYR_LSM6DSL_DS", "ACC_LSM303AGR_DS", "MAG_LSM303AGR_DS"]
AXIS = ["x", "y", "z"]

ds_model = {"ACC_LSM6DSL_DS": {}, "GYR_LSM6DSL_DS": {}, "ACC_LSM303AGR_DS": {}, "MAG_LSM303AGR_DS": {}}

x_train = []
x_test = []
y_train = []
y_test = []

# datasets and settings info
datasets_path = "../datasets/"
run_path = datasets_path + "running.txt"
jump_path = datasets_path + "jumping.txt"
stand_path = datasets_path + "standing.txt"
walk_path = datasets_path + "walking.txt"
valid_path = datasets_path + "validation/validation.txt"
log_path = "../log/"

# data processing config
WINDOW_SAMPLES = 50
SENS_VALUES = 12


# Definition of the neural network model
def CNN_model():
    numFilters = 32

    model = Sequential()
    num_classes = 4
    model.add(Conv2D(numFilters, kernel_size=3, strides=(1, 3), input_shape=(SENS_VALUES, WINDOW_SAMPLES, 1),
                     activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2), padding='valid'))
    model.add(Dropout(0.2))
    model.add(Flatten())
    model.add(Dense(32, activation='relu'))
    model.add(Dense(32, activation='relu'))
    model.add(Dense(num_classes, activation='softmax'))

    return model


def show_confusion_matrix(validations, predictions):

    matrix = metrics.confusion_matrix(validations, predictions)
    plt.figure(figsize=(6, 4))
    sns.heatmap(matrix,
                cmap='coolwarm',
                linecolor='white',
                linewidths=1,
                xticklabels=LABELS,
                yticklabels=LABELS,
                annot=True,
                fmt='d')
    plt.title('Confusion Matrix')
    plt.ylabel('True Label')
    plt.xlabel('Predicted Label')
    plt.show()


def main():
    # dataset collection
    stand_dataset = get_dataset(stand_path)
    run_dataset = get_dataset(run_path)
    walk_dataset = get_dataset(walk_path)
    jump_dataset = get_dataset(jump_path)

    stand_num = len(stand_dataset[SENSORS[0]]['x'])
    run_num = len(run_dataset[SENSORS[0]]['x'])
    walk_num = len(walk_dataset[SENSORS[0]]['x'])
    jump_num = len(jump_dataset[SENSORS[0]]['x'])

    act_batches = np.empty((3, 4))

    # data normalization
    stand_norm_ds = normalize(stand_dataset)
    run_norm_ds = normalize(run_dataset)
    walk_norm_ds = normalize(walk_dataset)
    jump_norm_ds = normalize(jump_dataset)


    # prepare NN input 
    proc_run, run_res = prepare_data(run_norm_ds, 0)
    proc_stand, stand_res = prepare_data(stand_norm_ds, 1)
    proc_walk, walk_res = prepare_data(walk_norm_ds, 2)
    proc_jump, jump_res = prepare_data(jump_norm_ds, 3)

    print("\r\nREADY to TRAIN the NN, Please insert the ratio of the data batches that will be used for testing:\r\n")
    ratio = input("RATIO: ")
    ratio = float(ratio)

    loss_res = []
    acc_res = []
    max_y_pred_test = []
    max_y_test = []
    y_pred_test = []
    batches_len = [proc_run.shape[0], proc_stand.shape[0], proc_walk.shape[0], proc_jump.shape[0]]
    test_batch_num = [int(np.round(ratio*batches_len[0])),
                      int(np.round(ratio*batches_len[1])),
                      int(np.round(ratio*batches_len[2])),
                      int(np.round(ratio*batches_len[3]))]
    max_folds = [int(np.floor(batches_len[0]/test_batch_num[0])),
                int(np.floor(batches_len[1]/test_batch_num[0])),
                int(np.floor(batches_len[2]/test_batch_num[0])),
                int(np.floor(batches_len[3]/test_batch_num[0]))]

    min_max_fold = np.min(max_folds)

    print("\r\nMINIMUM FOLD CYCLE: " + str(min_max_fold))
    for i in range(0, min_max_fold-1):
        # dataset partitioning

        print("\r\nSTART TRAINING AND TESTING USING FOLD " + str(i) + "\r\n")
        proc_run_train, proc_run_test, run_res_train, run_res_test = split_train_test(proc_run, run_res, ratio, i)
        proc_stand_train, proc_stand_test, stand_rest_train, stand_res_test = split_train_test(proc_stand, stand_res, ratio, i)
        proc_walk_train, proc_walk_test, walk_rest_train, walk_res_test = split_train_test(proc_walk, walk_res, ratio, i)
        proc_jump_train, proc_jump_test, jump_res_train, jump_res_test = split_train_test(proc_jump, jump_res, ratio, i)

        train_results = np.vstack((run_res_train, stand_rest_train, walk_rest_train, jump_res_train))
        test_results = np.vstack((run_res_test, stand_res_test, walk_res_test, jump_res_test))

        x_train = np.vstack((proc_run_train, proc_stand_train, proc_walk_train, proc_jump_train))
        x_test = np.vstack((proc_run_test, proc_stand_test, proc_walk_test, proc_jump_test))

        # y_train with richest datasets
        y_train = keras.utils.to_categorical(train_results, num_classes=4)
        y_test = keras.utils.to_categorical(test_results, num_classes=4)

        # set model
        model = CNN_model()
        model_name = 'activityRec_trained_model.h5'

        # prepare model saving env
        save_dir = os.path.join(os.getcwd(), '../saved_models')

        # Save model and weights
        if not os.path.isdir(save_dir):
            os.makedirs(save_dir)
        model_path = os.path.join(save_dir, model_name)

        # Training logging
        now = datetime.datetime.now()
        log_file = log_path + "activityRec_train_log" + "_" + now.strftime("%d-%m-%Y_%H%M%S") + ".log"
        csv_log = CSVLogger(filename=log_file, separator='|')

        # callback list
        callback_list = [
            keras.callbacks.ModelCheckpoint(model_path,
                                            monitor='val_loss',
                                            save_best_only=True),
            keras.callbacks.EarlyStopping(monitor='val_loss', mode='min', verbose=1, patience=15),
            csv_log
        ]

        # Hyper-parameters
        BATCH_SIZE = 64
        EPOCHS = 30

        # Learning rate setting
        # SGD optimizer
        #learning_rate = 0.1
        #decay_rate = learning_rate / EPOCHS
        #mnt = 0.6
        #sgd = keras.optimizers.SGD(lr=learning_rate, momentum=mnt, decay=decay_rate, nesterov=False

        # Adam optimizer
        adam = keras.optimizers.Adam(lr=0.001, decay=1e-6)


        model.compile(loss='categorical_crossentropy',
                      optimizer=adam, metrics=['accuracy'])

        # Reshape NN input dataset

        x_train = x_train.reshape(x_train.shape[0], x_train.shape[1], x_train.shape[2], 1)
        x_test = x_test.reshape(x_test.shape[0], x_test.shape[1], x_test.shape[2], 1)

        # Training
        history = model.fit(x_train,
                            y_train,
                            batch_size=BATCH_SIZE,
                            epochs=EPOCHS,
                            validation_split=0.3,
                            verbose=1,
                            callbacks=callback_list)

        model.save(model_path)
        print('Saved trained model at %s ' % model_path)

        # Score trained model.
        scores = model.evaluate(x_test, y_test, verbose=1)
        loss_res.append(scores[0])
        acc_res.append(scores[1])
        y_pred_test.append(model.predict(x_test))
        max_y_pred_test.append(np.argmax(y_pred_test[i], axis=1))
        max_y_test.append(np.argmax(y_test, axis=1))


    av_loss = np.mean(loss_res)
    av_acc = np.mean(acc_res)

    print("\r\n\n TRAINING AND TESTING FINISHED:\n")
    print("\r\nShowing results...\r\n")

    print("\r\n********  TOTAL SCORE  ************ ")
    print("\t\n - Average Loss: ", av_loss)
    print("\t\n - Average Accuracy: ", av_acc)
    print("\r\n\n")

    for j in range(0, min_max_fold - 1):
        print("\r\n********  FOLD " + str(j+1) + "  ************")
        print("\t\n - Average Loss: ", loss_res[j])
        print("\t\n - Average Accuracy: ", acc_res[j])
        show_confusion_matrix(max_y_test[j], max_y_pred_test[j])
        print("\r\n\n")

if __name__ == '__main__':
    main()
