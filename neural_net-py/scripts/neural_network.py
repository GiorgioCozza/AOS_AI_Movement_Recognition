# IMPORT
import numpy as np
from keras.layers import Dense, Conv2D, Flatten, Dropout, MaxPooling2D, BatchNormalization, LSTM
from keras.losses import categorical_crossentropy
from keras.optimizers import Adam, RMSprop, SGD
from keras import Sequential
from keras.callbacks import EarlyStopping
from keras.callbacks import TensorBoard
from tensorboard import program
from datetime import datetime as dt
import os
from scripts.data_visualization import show_confusion_matrix
from scripts.nn_config import *


x_train = []
x_test = []
y_train = []
y_test = []


# Definition of the neural network model
def CNN_model():
    numFilters = 24

    model = Sequential()
    model.add(Conv2D(numFilters, kernel_size=3, strides=(1, 1),
                     activation='relu', input_shape=(WINDOW_SAMPLES, SENS_VALUES, 1)))
    model.add(MaxPooling2D(pool_size=(2, 2), padding='valid'))
    model.add(Flatten())
    model.add(Dense(32, activation='relu'))
    model.add(Dropout(0.2))
    model.add(Dense(32, activation='relu'))
    model.add(Dropout(0.2))
    model.add(Dense(num_classes, activation='softmax'))

    model.summary()
    return model


def RNN_model():

    hid_nodes_lstm = 32
    rec_graph = Sequential()
    rec_graph.add(BatchNormalization(input_shape=(WINDOW_SAMPLES, SENS_VALUES)))
    rec_graph.add(LSTM(units=hid_nodes_lstm, return_sequences=True, name='RNN1'))
    rec_graph.add(Dropout(0.2))
    rec_graph.add(LSTM(units=hid_nodes_lstm, return_sequences=True, name='RNN2'))
    rec_graph.add(Dropout(0.2))
    rec_graph.add(LSTM(units=hid_nodes_lstm, return_sequences=True, name='RNN3'))
    rec_graph.add(Dropout(0.2))
    rec_graph.add(LSTM(units=hid_nodes_lstm, return_sequences=False, name='RNN4'))
    rec_graph.add(Dropout(0.2))
    rec_graph.add(Dense(num_classes, activation='softmax'))

    rec_graph.summary()
    return rec_graph


def train_model(model, x_train, y_train, x_valid, y_valid):
    # set model
    model_name =  "activityRec_model" + "_" + dt.now().strftime("%d-%m-%Y_%H%M%S") + ".log"
    # Training logging
    now = dt.now()
    log_file = log_path + "activityRec_train_log" + "_" + now.strftime("%d-%m-%Y_%H%M%S") + ".log"
    logfile_path = os.path.join(log_dir, log_file)
    tensorboard_cb = TensorBoard(log_dir=logfile_path,histogram_freq=1)
    # callback list
    callback_list = [
        EarlyStopping(monitor='val_acc', mode='max', verbose=1, patience=40),
        tensorboard_cb
    ]

    # Hyper-parameters
    BATCH_SIZE = 32
    EPOCHS = 80

    learning_rate = 0.0001
    rms = RMSprop(lr=learning_rate)
    adam = Adam(lr=learning_rate)
    sgd = SGD(lr=0.001, momentum=0.5, nesterov=True)

    model.compile(loss='categorical_crossentropy',
                  optimizer=rms, metrics=['accuracy'])

    tspe = int(np.round((len(x_train)/BATCH_SIZE)*0.1))
    vspe = int(np.round((len(x_valid)/BATCH_SIZE)*0.1))
    # Training
    history = model.fit(x_train,
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