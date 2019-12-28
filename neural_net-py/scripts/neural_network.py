# IMPORT
import numpy as np
import keras
from keras.models import Sequential
from keras.layers import Dense, Conv2D, MaxPooling2D, Dropout, Flatten
from keras.callbacks import CSVLogger
import seaborn as sns
from datetime import datetime as dt
from sklearn import metrics
from matplotlib import pyplot as plt
import os

from scripts.nn_config import *

x_train = []
x_test = []
y_train = []
y_test = []


# Definition of the neural network model
def CNN_model():
    numFilters = 16

    model = Sequential()
    num_classes = 4
    model.add(Conv2D(numFilters, kernel_size=3, strides=(1, 1),
                     activation='relu', input_shape=(SENS_VALUES, WINDOW_SAMPLES, 1)))
    model.add(MaxPooling2D(pool_size=(2, 2), padding='valid'))
    model.add(Dropout(0.2))
    model.add(Flatten())
    model.add(Dense(32, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(32, activation='relu'))
    model.add(Dense(num_classes, activation='softmax'))

    model.summary()
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


def train_model(model, x_train, y_train, x_valid, y_valid):
    # set model
    model_name =  "activityRec_model" + "_" + dt.now().strftime("%d-%m-%Y_%H%M%S") + ".log"
    # Training logging
    now = dt.now()
    log_file = log_path + "activityRec_train_log" + "_" + now.strftime("%d-%m-%Y_%H%M%S") + ".log"
    csv_log = CSVLogger(filename=log_file, separator='|')

    # callback list
    callback_list = [
        keras.callbacks.ModelCheckpoint(best_mod_path,
                                        monitor='val_accuracy',
                                        mode='max',
                                        save_best_only=True),
        keras.callbacks.EarlyStopping(monitor='val_accuracy', mode='max', verbose=1, patience=15),
        csv_log
    ]

    # Hyper-parameters
    BATCH_SIZE = 64
    EPOCHS = 100

    # Learning rate setting
    # SGD optimizer
    learning_rate = 0.0001
    decay_rate = learning_rate / EPOCHS
    mnt = 0.5
    sgd = keras.optimizers.RMSprop(lr=learning_rate)

    # Adam optimizer
    adam = keras.optimizers.Adam(lr=0.0001)

    model.compile(loss='categorical_crossentropy',
                  optimizer=sgd, metrics=['accuracy'])

    # Training
    history = model.fit(x_train,
                        y_train,
                        batch_size=BATCH_SIZE,
                        epochs=EPOCHS,
                        validation_data=(x_valid, y_valid),
                        shuffle=True,
                        verbose=1,
                        callbacks=callback_list)




def test_model_set(model_set, test_set):

    loss_res = []
    acc_res = []
    max_y_pred_test = []
    max_y_test = []
    y_pred_test = []
    for i in range(0, len(model_set)):
        scores = model_set[i].evaluate(test_set[i]["input"], test_set[i]["output"], verbose=1)
        loss_res.append(scores[0])
        acc_res.append(scores[1])
        y_pred_test.append(model_set[i].predict(test_set[i]["input"]))
        max_y_pred_test.append(np.argmax(y_pred_test[i], axis=1))
        max_y_test.append(np.argmax(test_set[i]["output"], axis=1))

        print("\r\n********  FOLD " + str(i + 1) + "  ************")
        print("\t\n - Fold Loss: ", loss_res[i])
        print("\t\n - Fold Accuracy: ", acc_res[i])
        show_confusion_matrix(max_y_test[i], max_y_pred_test[i])
        print("\r\n\n")

    av_loss = np.mean(loss_res)
    av_acc = np.mean(acc_res)

    print("\r\n\n TRAINING AND TESTING FINISHED:\n")
    print("\r\nShowing results...\r\n")

    print("\r\n********  TOTAL SCORE  ************ ")
    print("\t\n - Average Loss: ", av_loss)
    print("\t\n - Average Accuracy: ", av_acc)
    print("\r\n\n")

    best_acc_model = model_set[np.argmax(acc_res)]
    best_loss_model = model_set[np.argmax(loss_res)]

    return best_acc_model, best_loss_model


def get_folds(btch_lens):

    print("\r\nREADY to TRAIN the NN, Please insert the ratio of the data batches that will be used for testing:\r\n")
    ratio = input("RATIO: ")
    ratio = float(ratio)

    test_batch_num = [int(np.round(ratio * btch_lens[0])),
                      int(np.round(ratio * btch_lens[1])),
                      int(np.round(ratio * btch_lens[2])),
                      int(np.round(ratio * btch_lens[3]))]
    valid_batch_num = [int(np.round(ratio * btch_lens[0])),
                       int(np.round(ratio * btch_lens[1])),
                       int(np.round(ratio * btch_lens[2])),
                       int(np.round(ratio * btch_lens[3]))]
    max_folds = [int(np.floor(btch_lens[0] / (test_batch_num[0] + valid_batch_num[0]))),
                 int(np.floor(btch_lens[1] / (test_batch_num[1] + valid_batch_num[1]))),
                 int(np.floor(btch_lens[2] / (test_batch_num[2] + valid_batch_num[2]))),
                 int(np.floor(btch_lens[3] / (test_batch_num[3] + valid_batch_num[3])))]

    folds = np.min(max_folds)

    return ratio, folds



def save_best_models(max_acc_mod, min_loss_mod):

    loss_mod_path = os.path.join(best_mod_path, "best_loss_model_" + dt.now().strftime("%d-%m-%Y_%H%M%S") + '.h5')
    acc_mod_path = os.path.join(best_mod_path, "best_acc_model_" + dt.now().strftime("%d-%m-%Y_%H%M%S") + '.h5')

    min_loss_mod.save(loss_mod_path)
    max_acc_mod.save(acc_mod_path)