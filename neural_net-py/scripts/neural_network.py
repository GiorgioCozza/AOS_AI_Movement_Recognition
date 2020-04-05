# IMPORT
import numpy as np
from keras.layers import Dense, Conv2D, Flatten, Dropout, MaxPooling2D, BatchNormalization, LSTM
from keras.losses import categorical_crossentropy
from keras.optimizers import Adam, RMSprop, SGD
from keras import Sequential
from keras.callbacks import EarlyStopping
from keras.callbacks import TensorBoard
import keras.backend as K
from datetime import datetime as dt
import tensorflow as tf
import os
from scripts.data_visualization import show_confusion_matrix
from scripts.config import *

x_train = []
x_test = []
y_train = []
y_test = []

# print model summary in a text file
def model_summary(model, mod_type=''):
    if mod_type == '':
        model.summary()
    else:
        sum_name = mod_type + '_summary' + dt.now().strftime("%d-%m-%Y_%H%M%S") + ".txt"
        sum_log = os.path.join(log_dir, sum_name)
        with open(sum_log, mode='w') as f:
            model.summary(print_fn=lambda x: print(x, file=f))
        f.close()


# Convolutional Neural Network model
def CNN_model():
    numFilters = 24

    cnn_model = Sequential()
    cnn_model.add(BatchNormalization(input_shape=(WINDOW_SAMPLES, SENS_VALUES, 1)))
    cnn_model.add(Conv2D(numFilters, kernel_size=3, strides=(1, 1), activation='relu', name='Conv2D_1'))
    cnn_model.add(MaxPooling2D(pool_size=(2, 2), padding='valid', name='MaxPool_1'))
    cnn_model.add(Flatten())
    cnn_model.add(Dense(32, activation='relu', name='FCN2'))
    cnn_model.add(Dropout(0.2))
    cnn_model.add(Dense(num_classes, activation='softmax', name='FCN3'))

    model_summary(cnn_model, mod_type='CNN')
    return cnn_model

# Recurrent Neural Network model
def RNN_model():
    hid_nodes_lstm = 32
    rnn_model = Sequential()
    rnn_model.add(BatchNormalization(input_shape=(WINDOW_SAMPLES, SENS_VALUES)))
    rnn_model.add(LSTM(units=hid_nodes_lstm, return_sequences=True, name='LSTM1'))
    rnn_model.add(Dropout(0.2))
    rnn_model.add(LSTM(units=hid_nodes_lstm, return_sequences=True, name='LSTM2'))
    rnn_model.add(Dropout(0.2))
    rnn_model.add(LSTM(units=hid_nodes_lstm, return_sequences=True, name='LSTM3'))
    rnn_model.add(Dropout(0.2))
    rnn_model.add(LSTM(units=hid_nodes_lstm, return_sequences=False, name='LSTM4'))
    rnn_model.add(Dropout(0.2))
    rnn_model.add(Dense(num_classes, activation='softmax', name='FCN1'))

    model_summary(rnn_model, mod_type='RNN')
    return rnn_model



def train_model(model, x_train, y_train, x_valid, y_valid):
    # Training logging
    now = dt.now()
    log_file = "train_log" + "_" + now.strftime("%d-%m-%Y_%H%M%S") + ".log"
    logfile_path = os.path.join(log_dir, log_file)
    tensorboard_cb = TensorBoard(log_dir=logfile_path, histogram_freq=0)
    # callback list
    callback_list = [
        EarlyStopping(monitor='val_acc', mode='max', verbose=1, patience=10),
        tensorboard_cb
    ]


    BATCH_SIZE = 32
    EPOCHS = 40

    learning_rate = 0.0001
    rms = RMSprop(lr=learning_rate)

    model.compile(loss='categorical_crossentropy',
                  optimizer=rms, metrics=['accuracy'])

    # Training
    model.fit(x_train,
              y_train,
              epochs=EPOCHS,
              batch_size=BATCH_SIZE,
              validation_data=(x_valid, y_valid),
              shuffle=True,
              verbose=1,
              callbacks=callback_list)


# Test all the model of a given set
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

    best_acc_fold = np.argmax(acc_res)
    best_loss_fold = np.argmin(loss_res)
    best_acc_model = model_set[best_acc_fold]
    best_loss_model = model_set[best_loss_fold]

    return best_acc_model, best_loss_model, best_acc_fold, best_loss_fold


def save_best_models(max_acc_mod, min_loss_mod, model_name, acc_fold, loss_fold):

    loss_mod_path = os.path.join(best_mod_dir,
                                 "best_loss_" + model_name + '_fold' + str(loss_fold) + '_' + dt.now().strftime("%d-%m-%Y_%H%M%S") + '.h5')
    acc_mod_path = os.path.join(best_mod_dir,
                                "best_acc_model_" + model_name + '_fold' + str(acc_fold) + '_' + dt.now().strftime("%d-%m-%Y_%H%M%S") + '.h5')

    min_loss_mod.save(loss_mod_path)
    max_acc_mod.save(acc_mod_path)

