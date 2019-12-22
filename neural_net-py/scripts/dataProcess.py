# coding:utf-8
import numpy as np
import math
import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation
from keras.optimizers import SGD
import os

# datasets info
datasets_path = "../datasets/"
preproc_path = "../datasets/preproc/"
run_path = datasets_path + "running.txt"
jump_path = datasets_path + "jumping.txt"
stand_path = datasets_path + "standing.txt"
walk_path = datasets_path + "walking.txt"
tmp_preproc = preproc_path + "tmp_preproc"

RUN_TEST_SAMPLES = 100
JUMP_TEST_SAMPLES = 100
STAND_TEST_SAMPLES = 100
WALK_TEST_SAMPLES = 100

RUN_TRAIN_SAMPLES = 15000
JUMP_TRAIN_SAMPLES = 15000
STAND_TRAIN_SAMPLES = 15000
WALK_TRAIN_SAMPLES = 15000

SENSORS = ["ACC_LSM6DSL_DS", "GYR_LSM6DSL_DS", "ACC_LSM303AGR_DS", "MAG_LSM303AGR_DS"]
AXIS = ["x", "y", "z"]

SENS_VALUES = 12
WINDOW_SIZE = 50


def get_array(filename):
    file_handle = open(filename, mode='r')
    line = file_handle.readline().split()
    line = list(map(int, line))
    file_handle.readline()
    data = []
    count = 0
    while 1:
        # read each line
        line2 = file_handle.readline().split()  # directly produce a list
        line2 = list(map(int, line2))  # convert str to int
        line3 = [line2[i] - line[i] for i in range(len(line2))]  # operation in the list
        if not line2:  # read by line
            break
        file_handle.readline()  # process the empty line
        line = line2  # store for sub
        # data = [data,line3]   #This way could not work
        data.append(line3)  # 'NoneType' object has no attribute 'append'  No prefix should be added into the array
        count = count + 1
    # print(data)
    print(len(data))
    data1 = sub_data(4, data)
    return data1


def sub_data(step, list):
    bound = len(list)
    row = math.floor((bound - 10) / step)
    count = 0
    iter = 0
    data = np.zeros((row, 120))
    while iter < row:
        vector = []
        for i in range(10):
            vector.append(list[i + count])
        vectornp = np.array(vector)
        vectornp = vectornp.reshape(1, 120)
        data[iter] = vectornp
        iter = iter + 1
        count = count + step
    return data

# Get samples from the dataset file and organize them into a dictionary
def get_dataset(filename):
    res = cp.deepcopy(ds_model)

    with open(filename, mode='r') as ins:
        for line in ins:
            ar = line.split()
            ar = list(map(int, ar))
            v_count = 0
            for s in SENSORS:
                for a in AXIS:
                    if v_count == 12:
                        v_count = 0
                    res[s][a].append(ar[v_count])
                    v_count += 1

    return res


def main():
    # get the input data

    data1 = get_array(walk_path)
    data2 = get_array(stand_path)
    running_data = np.vstack(data1)
    jumping_data = np.vstack(data2)

    running_train = running_data[0:4000]
    running_test = running_data[4001:]

    jumping_bound = math.floor((jumping_data.shape[0]) / 100) * 100
    jumping_train = jumping_data[range(0, jumping_bound, 1)]
    jumping_test = jumping_data[range(jumping_bound + 1, jumping_data.shape[0] - 1, 1)]

    train_results = np.vstack((np.zeros((running_train.shape[0], 1)), np.ones((jumping_train.shape[0], 1))))
    test_results = np.vstack((np.zeros((running_test.shape[0], 1)), np.ones((jumping_test.shape[0], 1))))

    print(running_train.shape)
    print(jumping_train.shape)
    # segment to the train and test data
    x_train = np.vstack((running_train, jumping_train))
    x_test = np.vstack((running_test, jumping_test))
    y_train = keras.utils.to_categorical(train_results, num_classes=2)
    y_test = keras.utils.to_categorical(test_results, num_classes=2)

    # set model
    model = Sequential()
    model_name = 'keras_cifar10_trained_model.h5'
    model.add(Dense(64, activation='relu', input_dim=120))
    model.add(Dropout(0.5))
    model.add(Dense(64, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(2, activation='softmax'))
    sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)
    model.compile(loss='categorical_crossentropy',
                  optimizer=sgd,
                  metrics=['accuracy'])

    model.fit(x_train, y_train,
              epochs=20,
              batch_size=128)
    score = model.evaluate(x_test, y_test, batch_size=128)

    save_dir = os.path.join(os.getcwd(), 'saved_models')

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


if __name__ == "__main__":
    main()
