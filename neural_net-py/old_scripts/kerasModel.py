# IMPORT
import numpy as np
import math
import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten, Reshape
from keras.optimizers import SGD
from dataProcess import get_dataset, normalize, sub_data, prepare_data
import copy as cp
import os
import datetime

# output labels
LABELS = ["running",
          "walking",
          "jumping",
          "football",
          "basketball"]

SENSORS = ["ACC_LSM6DSL_DS", "GYR_LSM6DSL_DS", "ACC_LSM303AGR_DS", "MAG_LSM303AGR_DS"]
AXIS = ["x", "y", "z"]

ds_model = {"ACC_LSM6DSL_DS": {}, "GYR_LSM6DSL_DS": {}, "ACC_LSM303AGR_DS": {}, "MAG_LSM303AGR_DS": {}}

x_train = []
x_test = []
y_train = []
y_test = []

# datasets info
datasets_path = "../datasets/"
preproc_path = "../datasets/preproc/"
run_path = datasets_path + "running.txt"
jump_path = datasets_path + "jumping.txt"
stand_path = datasets_path + "standing.txt"
walk_path = datasets_path + "walking.txt"
tmp_preproc = preproc_path + "tmp_preproc"
valid_path = datasets_path + "validation/"

# data processing config
WINDOW_SAMPLES = 50
SENS_VALUES = 12


def split_train_test(ds):
    ds_train = cp.deepcopy(ds_model)
    ds_test = cp.deepcopy(ds_model)

    ds_length = len(ds[SENSORS[0]]["x"])
    train_bound = (math.floor(ds_length / 1000) * 1000) - 1000
    valid_bound = (math.floor((ds_length - train_bound) / 100) * 100) + train_bound

    for sens in SENSORS:
        for axis in AXIS:
            ds_train[sens][axis] = ds[sens][axis][0:train_bound]
            ds_test[sens][axis] = ds[sens][axis][valid_bound:]

    return ds_train, ds_test


def main():
    # path3 = "data.txt"
    stand_dataset = get_dataset(stand_path)
    run_dataset = get_dataset(run_path)
    walk_dataset = get_dataset(walk_path)
    jump_dataset = get_dataset(jump_path)

    stand_norm_ds = normalize(stand_dataset)
    run_norm_ds = normalize(run_dataset)
    walk_norm_ds = normalize(walk_dataset)
    jump_norm_ds = normalize(jump_dataset)

    run_train, run_test = split_train_test(run_norm_ds)
    stand_train, stand_test = split_train_test(stand_norm_ds)
    walk_train, walk_test = split_train_test(walk_norm_ds)
    jump_train, jump_test = split_train_test(jump_norm_ds)

    # prepare data
    proc_run_train = np.vstack(prepare_data(run_train))
    proc_stand_train = np.vstack(prepare_data(stand_train))
    proc_walk_train = np.vstack(prepare_data(walk_train))
    proc_jump_train = np.vstack(prepare_data(jump_train))

    proc_run_test = np.vstack(prepare_data(run_test))
    proc_stand_test = np.vstack(prepare_data(stand_test))
    proc_walk_test = np.vstack(prepare_data(walk_test))
    proc_jump_test = np.vstack(prepare_data(jump_test))

    # train_result with all the datasets
    run_res_train = np.zeros((proc_run_train.shape[0], 1))
    stand_res_train = np.vstack(np.ones((proc_stand_train.shape[0], 1)))
    walk_res_train = np.vstack((np.ones((proc_walk_train.shape[0], 1)) + 1))
    jump_res_train = np.vstack((np.ones((proc_jump_train.shape[0], 1)) + 2))
    train_results = np.vstack((run_res_train, stand_res_train, walk_res_train, jump_res_train))

    # train_result with the richest datasets
    # stand_res_train = np.vstack(np.zeros((proc_stand_train.shape[0], 1)))
    # walk_res_train = np.vstack((np.ones((proc_walk_train.shape[0], 1))))
    # train_results = np.vstack((stand_res_train, walk_res_train))

    # test_result with all the datasets
    run_res_test = np.vstack(np.zeros((proc_run_test.shape[0], 1)))
    stand_res_test = np.vstack(np.ones((proc_stand_test.shape[0], 1)))
    walk_res_test = np.vstack((np.ones((proc_walk_test.shape[0], 1)) + 1))
    jump_res_test = np.vstack((np.ones((proc_jump_test.shape[0], 1)) + 2))
    test_results = np.vstack((run_res_test, stand_res_test, walk_res_test, jump_res_test))

    # test_results with the richest datasets
    # stand_res_test = np.vstack(np.zeros((proc_stand_test.shape[0], 1)))
    # walk_res_test = np.vstack((np.ones((proc_walk_test.shape[0], 1))))
    # test_results = np.vstack((stand_res_test, walk_res_test))

    # x_test with all the datasets
    x_train = np.vstack((proc_run_train, proc_stand_train, proc_walk_train, proc_jump_train))
    x_test = np.vstack((proc_run_test, proc_stand_test, proc_walk_test, proc_jump_test))

    # y_train and y_test with all the datasets
    y_train = keras.utils.to_categorical(train_results, num_classes=4)
    y_test = keras.utils.to_categorical(test_results, num_classes=4)

    # x_train with the richest datasets
    # x_train = np.vstack((proc_stand_train, proc_walk_train))
    # x_test = np.vstack((proc_stand_test, proc_walk_test))

    # y_train with richest datasets
    # y_train = keras.utils.to_categorical(train_results, num_classes=2)
    # y_test = keras.utils.to_categorical(test_results, num_classes=2)

    # set model
    model = Sequential()
    model_name = 'activityRec_trained_model.h5'
    num_classes = 4
    model.add(Dense(64, activation='relu', input_dim=120))
    # model.add(Dropout(0.5))
    model.add(Dense(64, activation='relu'))
    # model.add(Dropout(0.5))
    model.add(Dense(64, activation='relu'))
    model.add(Dense(num_classes, activation='softmax'))

    model.compile(loss='categorical_crossentropy',
                  optimizer='adam', metrics=['accuracy'])

    # Hyper-parameters
    BATCH_SIZE = 128
    EPOCHS = 50

    # Enable validation to use ModelCheckpoint and EarlyStopping callbacks.
    history = model.fit(x_train,
                        y_train,
                        batch_size=BATCH_SIZE,
                        epochs=EPOCHS,
                        validation_split=0.2,
                        verbose=1)

    # sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)
    # model.compile(loss='categorical_crossentropy',
    #      optimizer=sgd,
    #      metrics=['accuracy'])

    # model.fit(x_train, y_train,
    #  epochs=50,
    #  batch_size=128)
    score = model.evaluate(x_test, y_test, batch_size=128)

    save_dir = os.path.join(os.getcwd(), '../saved_models')

    # Save model and weights
    if not os.path.isdir(save_dir):
        os.makedirs(save_dir)
    model_path = os.path.join(save_dir, model_name)
    model.save(model_path)
    print('Saved trained model at %s ' % model_path)

    # Score trained model.
    scores = model.evaluate(x_test, y_test, verbose=1)
    print('Test loss:', scores[0])
    print('Test accuracy:', scores[1])


if __name__ == '__main__':
    main()
