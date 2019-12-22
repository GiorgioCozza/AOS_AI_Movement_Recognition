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
walk_path = datasets_path + "data.txt"
tmp_preproc = preproc_path + "tmp_preproc"


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
    data1 = sub_data(4, data)
    return data1


model_name = 'keras_test_trained_model.h5'
# Generate dummy data
import numpy as np

x_train = np.random.random((1000, 120))
y_train = keras.utils.to_categorical(np.random.randint(10, size=(1000, 1)), num_classes=10)
x_test = np.random.random((100, 120))
y_test = keras.utils.to_categorical(np.random.randint(10, size=(100, 1)), num_classes=10)

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
