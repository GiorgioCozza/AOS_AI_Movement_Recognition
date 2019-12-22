# coding:utf-8
import numpy as np
import math
import copy as cp

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

ds_model = {"ACC_LSM6DSL_DS": {"x": [], "y": [], "z": []}, "GYR_LSM6DSL_DS": {"x": [], "y": [], "z": []},
                   "ACC_LSM303AGR_DS": {"x": [], "y": [], "z": []}, "MAG_LSM303AGR_DS": {"x": [], "y": [], "z": []}}




# Dataset partitioning (training, testing)
def split_train_test(ds_batches, labels, ratio, fold):

    ds_batch_num = ds_batches.shape[0]
    test_batch_num = int(np.round(ratio*ds_batch_num))
    max_fold = int(np.floor(ds_batch_num/test_batch_num))
    if test_batch_num > 0 and fold in range(0, max_fold-1):
        test_bstart = fold * test_batch_num
        test_bend = test_bstart + test_batch_num
        ds_test_batches = ds_batches[test_bstart:test_bend]
        ds_train_batches = np.delete(ds_batches, range(test_bstart, test_bend), axis=0)
        test_labels = labels[test_bstart:test_bend]
        train_labels = np.delete(labels, range(test_bstart, test_bend), axis=0)
    else:
        print("\r\nBAD INPUT: Fold out of range or negative test batches")
        return

    return ds_train_batches, ds_test_batches, train_labels, test_labels



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



# Dataset normalization
def normalize(dataset_dict):
    norm_dataset = cp.deepcopy(ds_model)

    for s in SENSORS:
        for ax in AXIS:
            max_ax = np.max(dataset_dict[s][ax])
            min_ax = np.min(dataset_dict[s][ax])
            #mu = np.mean(dataset_dict[s][ax], axis=0)
            #sigma = np.std(dataset_dict[s][ax], axis=0)
            norm_dataset[s][ax] = (dataset_dict[s][ax] - min_ax) / (max_ax - min_ax)

            # _itr = iter(dataset_dict[s][ax])
            # _max = max(_itr)
            # for k in range(0, len(dataset_dict[s][ax])):
            #    val = dataset_dict[s][ax][k]
            #    norm_val = val / _max
            #    norm_val = round(norm_val, 4)
            #    norm_dataset[s][ax].append(norm_val)

    return norm_dataset





# Reshape and organize the input and output sets
def prepare_data(ds, label):
    segments = np.empty((0, SENS_VALUES, WINDOW_SIZE))
    step = 50
    labels = []
    reshaped_segments = np.empty((12,50))
    ds_len = len(ds["ACC_LSM6DSL_DS"]["x"])
    ds_r = ds_len % WINDOW_SIZE
    ds_len = ds_len - ds_r

    for i in range(0, ds_len - step, step):
        xs_acc_lsm6dsl = ds[SENSORS[0]]["x"][i: i + step]
        ys_acc_lsm6dsl = ds[SENSORS[0]]["y"][i: i + step]
        zs_acc_lsm6dsl = ds[SENSORS[0]]["z"][i: i + step]
        xs_gyr_lsm6dsl = ds[SENSORS[1]]["x"][i: i + step]
        ys_gyr_lsm6dsl = ds[SENSORS[1]]["y"][i: i + step]
        zs_gyr_lsm6dsl = ds[SENSORS[1]]["z"][i: i + step]
        xs_acc_lsm303agr = ds[SENSORS[2]]["x"][i: i + step]
        ys_acc_lsm303agr = ds[SENSORS[2]]["y"][i: i + step]
        zs_acc_lsm303agr = ds[SENSORS[2]]["z"][i: i + step]
        xs_mag_lsm303agr = ds[SENSORS[3]]["x"][i: i + step]
        ys_mag_lsm303agr = ds[SENSORS[3]]["y"][i: i + step]
        zs_mag_lsm303agr = ds[SENSORS[3]]["z"][i: i + step]

        reshaped_segments = [xs_acc_lsm6dsl, ys_acc_lsm6dsl, zs_acc_lsm6dsl,
                             xs_gyr_lsm6dsl, ys_gyr_lsm6dsl, zs_gyr_lsm6dsl,
                             xs_acc_lsm303agr, ys_acc_lsm303agr, zs_acc_lsm303agr,
                             xs_mag_lsm303agr, ys_mag_lsm303agr, zs_mag_lsm303agr]
        reshaped_segments = np.transpose(reshaped_segments)

        segments = np.vstack([segments, np.dstack(reshaped_segments)])
        labels.append(label)

    labels = np.vstack(labels)
    return segments, labels




def prepare_data_simp_mod(ds, label):
    segments = np.empty((0, WINDOW_SIZE, SENS_VALUES))
    step = 50
    labels = []
    reshaped_segments = []
    ds_len = len(ds["ACC_LSM6DSL_DS"]["x"])
    n_feature = 12
    for i in range(0, ds_len - step, step):
        xs_acc_lsm6dsl = ds[SENSORS[0]]["x"][i: i + step]
        ys_acc_lsm6dsl = ds[SENSORS[0]]["y"][i: i + step]
        zs_acc_lsm6dsl = ds[SENSORS[0]]["z"][i: i + step]
        xs_gyr_lsm6dsl = ds[SENSORS[1]]["x"][i: i + step]
        ys_gyr_lsm6dsl = ds[SENSORS[1]]["y"][i: i + step]
        zs_gyr_lsm6dsl = ds[SENSORS[1]]["z"][i: i + step]
        xs_acc_lsm303agr = ds[SENSORS[2]]["x"][i: i + step]
        ys_acc_lsm303agr = ds[SENSORS[2]]["y"][i: i + step]
        zs_acc_lsm303agr = ds[SENSORS[2]]["z"][i: i + step]
        xs_mag_lsm303agr = ds[SENSORS[3]]["x"][i: i + step]
        ys_mag_lsm303agr = ds[SENSORS[3]]["y"][i: i + step]
        zs_mag_lsm303agr = ds[SENSORS[3]]["z"][i: i + step]

        segments = np.vstack([segments, np.dstack([xs_acc_lsm6dsl, ys_acc_lsm6dsl, zs_acc_lsm6dsl,
                                                   xs_gyr_lsm6dsl, ys_gyr_lsm6dsl, zs_gyr_lsm6dsl,
                                                   xs_acc_lsm303agr, ys_acc_lsm303agr, zs_acc_lsm303agr,
                                                   xs_mag_lsm303agr, ys_mag_lsm303agr, zs_mag_lsm303agr])])

        #segments.append([xs_acc_lsm6dsl, ys_acc_lsm6dsl, zs_acc_lsm6dsl,
         #                xs_gyr_lsm6dsl, ys_gyr_lsm6dsl, zs_gyr_lsm6dsl,
          #               xs_acc_lsm303agr, ys_acc_lsm303agr, zs_acc_lsm303agr,
          #               xs_mag_lsm303agr, ys_mag_lsm303agr, zs_mag_lsm303agr])
        labels.append(label)

    reshaped_segments = np.asarray(segments, dtype=np.float32).reshape(-1, step, n_feature)
    labels = np.asarray(labels)
    return reshaped_segments, labels


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

