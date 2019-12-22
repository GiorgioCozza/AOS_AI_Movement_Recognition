# IMPORT
import numpy as np
import keras
from keras.models import Sequential
from keras.layers import Dense, Conv2D, MaxPooling2D, Dropout, Flatten, Reshape
from keras.callbacks import CSVLogger
from keras.optimizers import SGD, Adam
from scripts.dataset_processing import get_dataset, normalize, prepare_data_simp_mod, split_train_test
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
def simple_mod():
    numFilters = 32

    # set model
    model = Sequential()
    model_name = 'activityRec_trained_model.h5'
    num_classes = 4
    model.add(Dense(100, activation='relu'))
    model.add(Dense(100, activation='relu'))
    model.add(Dense(100, activation='relu'))
    model.add(Flatten())
    model.add(Dense(num_classes, activation='softmax'))

    return model


def show_confusion_matrix(validation_ds, predictions):
    conf_mat = metrics.confusion_matrix(validation_ds, predictions)
    plt.figure(figsize=(6, 4))


def main():
    # dataset collection
    stand_dataset = get_dataset(stand_path)
    run_dataset = get_dataset(run_path)
    walk_dataset = get_dataset(walk_path)
    jump_dataset = get_dataset(jump_path)

    # data normalization
    stand_norm_ds = normalize(stand_dataset)
    run_norm_ds = normalize(run_dataset)
    walk_norm_ds = normalize(walk_dataset)
    jump_norm_ds = normalize(jump_dataset)
    # dataset partitioning
    run_train, run_test = split_train_test(run_norm_ds)
    stand_train, stand_test = split_train_test(stand_norm_ds)
    walk_train, walk_test = split_train_test(walk_norm_ds)
    jump_train, jump_test = split_train_test(jump_norm_ds)

    # prepare NN input
    proc_run_train, run_res_train = prepare_data_simp_mod(run_train, 0)
    proc_stand_train, stand_res_train = prepare_data_simp_mod(stand_train, 1)
    proc_walk_train, walk_res_train = prepare_data_simp_mod(walk_train, 2)
    proc_jump_train, jump_res_train = prepare_data_simp_mod(jump_train, 3)

    proc_run_test, run_res_test = prepare_data_simp_mod(run_test, 0)
    proc_stand_test, stand_res_test = prepare_data_simp_mod(stand_test, 1)
    proc_walk_test, walk_res_test = prepare_data_simp_mod(walk_test, 2)
    proc_jump_test, jump_res_test = prepare_data_simp_mod(jump_test, 3)

    print(run_res_train.shape[1])
    print(stand_res_train.shape[1])
    print(walk_res_train.shape[1])

    train_results = np.vstack((run_res_train, stand_res_train, walk_res_train, jump_res_train))
    test_results = np.vstack((run_res_test, stand_res_test, walk_res_test, jump_res_test))

    x_train = np.vstack((proc_run_train, proc_stand_train, proc_walk_train, proc_jump_train))
    x_test = np.vstack((proc_run_test, proc_stand_test, proc_walk_test, proc_jump_test))

    # y_train with richest datasets
    y_train = keras.utils.to_categorical(train_results, num_classes=4)
    y_test = keras.utils.to_categorical(test_results, num_classes=4)



    # set model
    model = simple_mod()
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
        # keras.callbacks.EarlyStopping(monitor='acc', patience=1),
        csv_log
    ]

    # Hyper-parameters
    BATCH_SIZE = 64
    EPOCHS = 20

    # Learning rate setting
    # SGD optimizer
    #learning_rate = 0.1
    #decay_rate = learning_rate / EPOCHS
    #mnt = 0.6
    #sgd = keras.optimizers.SGD(lr=learning_rate, momentum=mnt, decay=decay_rate, nesterov=False)

    # Adam optimizer
    adam = keras.optimizers.Adam(lr=0.001, decay=1e-6)


    model.compile(loss='categorical_crossentropy',
                  optimizer=adam, metrics=['accuracy'])

    # Reshape NN input dataset
    num_sens = 12
    batches = 50
    inp_shape = num_sens *batches
    #x_train = x_train.reshape((inp_shape,))
    #x_test = x_test.reshape((inp_shape,))

    # Training
    history = model.fit(x_train,
                        y_train,
                        batch_size=BATCH_SIZE,
                        epochs=EPOCHS,
                        validation_split=0.2,
                        verbose=1,
                        callbacks=callback_list)

    model.save(model_path)
    print('Saved trained model at %s ' % model_path)

    # Score trained model.
    scores = model.evaluate(x_test, y_test, verbose=1)

    print('Test loss:', scores[0])
    print('Test accuracy:', scores[1])


if __name__ == '__main__':
    main()
