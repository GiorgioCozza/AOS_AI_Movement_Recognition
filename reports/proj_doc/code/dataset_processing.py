import numpy as np
import copy as cp
from datetime import datetime as dt
import os
import keras
import csv
import random as rnd
from scripts.config import *

ds_model = {"ACC_LSM6DSL_DS": {"x": [], "y": [], "z": []}, "GYR_LSM6DSL_DS": {"x": [], "y": [], "z": []},
                   "ACC_LSM303AGR_DS": {"x": [], "y": [], "z": []}, "MAG_LSM303AGR_DS": {"x": [], "y": [], "z": []}}




def merge_session_files(dir_path, activity):

    act_filename = activity + '.txt'
    actfile_dir = os.path.join(dir_path, 'activity_files')
    ds_file = os.path.join(dir_path, act_filename)
    sess_ds = {}
    with open(ds_file, mode='w', newline='') as fw:
        csv_wrt = csv.writer(fw, delimiter=' ')
        for file in os.listdir(actfile_dir):
            if file.startswith(activity):
                print('[LOG]: Extract samples from file: ' + file.split('datasets')[-1])
                filepath = os.path.join(actfile_dir, file)
                with open(filepath, mode='r') as fr:
                    csv_rd = csv.reader(fr, delimiter=',')
                    for line in csv_rd:
                        if len(line) == 13:
                            rd = [float(line[i].replace(' ', '')) for i in range(0, len(line))]
                            rd = list(map(int, rd))
                            rd = rd[1:]
                            csv_wrt.writerow(rd)
                fr.close()
    fw.close()




def get_activity_session(filename):

    res = cp.deepcopy(ds_model)

    print('[LOG]: Extract samples from session file: ' + filename.split('datasets')[-1])
    filepath = os.path.join(session_dir, filename)
    with open(filepath, mode='r') as fr:
        csv_rd = csv.reader(fr, delimiter=',')
        r_count = 1
        for line in csv_rd:
            if len(line) == 13:
                rd = [float(line[i].replace(' ', '')) for i in range(0, len(line))]
                rd = list(map(int, rd))
                rd = rd[1:]

                v_count = 0
                for s in SENSORS:
                    for a in AXIS:
                        if v_count == 12:
                            v_count = 0
                            r_count += 1
                        res[s][a].append(rd[v_count])
                        v_count += 1
    res['ts'] = [i for i in range(1,r_count+1)]
    fr.close()

    return res




# Dataset partitioning (training, testing)
def split_train_valid_test(ds_batches, labels, ratio, fold):

    ds_batch_num = ds_batches.shape[0]
    test_batch_num = int(np.round(ratio*ds_batch_num))
    val_batch_num = int(np.round(ratio*ds_batch_num))
    max_fold = int(np.floor(ds_batch_num/(test_batch_num + val_batch_num)))
    if test_batch_num > 0 and fold in range(0, max_fold-1):
        test_bstart = fold * test_batch_num
        test_bend = test_bstart + test_batch_num
        val_bstart = test_bend + fold*val_batch_num
        val_bend = val_bstart + val_batch_num
        ds_test_batches = ds_batches[test_bstart:test_bend]
        ds_val_batches = ds_batches[val_bstart:val_bend]
        ds_train_batches = np.delete(ds_batches, range(test_bstart, test_bend), axis=0)
        ds_train_batches = np.delete(ds_train_batches, range(val_bstart, val_bend), axis=0)
        test_labels = labels[test_bstart:test_bend]
        val_labels = labels[val_bstart:val_bend]
        train_labels = np.delete(labels, range(test_bstart, test_bend), axis=0)
        train_labels = np.delete(train_labels, range(val_bstart, val_bend), axis=0)
    else:
        print("\r\nBAD INPUT: Fold out of range or negative test batches")
        return

    split_dict = {"training" : {"input" : ds_train_batches, "output" : train_labels},
                  "validation" : {"input" : ds_val_batches, "output":val_labels},
                  "testing" : {"input": ds_test_batches, "output": test_labels}}
    #print(split_dict["testing"]["input"][0])
    return split_dict



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
def normalize(dataset_dict, activity):
    norm_dataset = cp.deepcopy(ds_model)

    log_file = 'preprocessing_log_' + activity + '_' + dt.now().strftime("%d-%m-%Y_%H%M") + '.txt'
    norm_log = os.path.join(log_dir, log_file)
    with open(norm_log, 'w') as f:

        f.write(activity)
        f.write("\n")
        for s in SENSORS:
            for ax in AXIS:
                max_ax = np.max(dataset_dict[s][ax])
                min_ax = np.min(dataset_dict[s][ax])
                ar = s + ', ' + ax + ', MIN: ' + str(min_ax) + ', MAX: ' + str(max_ax) + "\n"
                f.write(ar)
                norm_dataset[s][ax] = (dataset_dict[s][ax] - min_ax) / (max_ax - min_ax)

    f.close()
    return norm_dataset



# Reshape and organize the input and output sets
def prepare_data(ds, label):
    segments = np.empty((0, WINDOW_SAMPLES, SENS_VALUES))
    step = WINDOW_SAMPLES
    labels = []
    ds_len = len(ds["ACC_LSM6DSL_DS"]["x"])
    ds_r = ds_len % WINDOW_SAMPLES
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

        reshaped_segments = np.expand_dims(reshaped_segments, axis=0)
        segments = np.append(segments, reshaped_segments, axis=0)
        labels.append(label)

    labels = np.vstack(labels)

    return segments, labels





def test_on_csv(xtest, ytest, find_dir=None, fold_n=0):

    icsv_fname = 'test_input_' + 'fold-' + str(fold_n) + '_' + dt.now().strftime('%b%d_%H-%M-%S') + '.csv'
    ocsv_fname = 'test_output_' + 'fold-' + str(fold_n) + '_' + dt.now().strftime('%b%d_%H-%M-%S') + '.csv'

    csv_dir_name = 'test_' + dt.now().strftime('%b%d_%y')
    dt_csv_dir = os.path.join(csv_dir, csv_dir_name)

    if not os.path.exists(dt_csv_dir):
        os.makedirs(dt_csv_dir)

    with open(os.path.join(dt_csv_dir, icsv_fname), 'w', newline='') as f:

        wrt = csv.writer(f, delimiter=',')

        for idx in range(0, len(xtest)):
            ar = np.reshape(xtest[idx], [-1])
            wrt.writerow(ar)

        f.close()

    with open(os.path.join(dt_csv_dir, ocsv_fname), 'w', newline='') as f:

        wrt = csv.writer(f, delimiter=',')

        for idx in range(0, len(ytest)):
            ar = np.reshape(ytest[idx], [-1])
            wrt.writerow(ar)

        f.close()

    return icsv_fname, ocsv_fname


def test_from_csv(test_in_set, test_out_set):

    test_set = np.array([])

    if (len(test_in_set) == len(test_out_set)):

        csv_dir_name = 'test_' + dt.now().strftime('%b%d_%y')
        dt_csv_dir = os.path.join(csv_dir, csv_dir_name)

        if not os.path.exists(dt_csv_dir):
            os.makedirs(dt_csv_dir)

        print("\r\n[LOG]: Test sets dimension correct!")
        n = len(test_in_set)

        for i in range(0, n):

            test_set = np.append(test_set, {"input":  np.empty((0, WINDOW_SAMPLES, SENS_VALUES)), "output": []})

            with open(os.path.join(dt_csv_dir, test_in_set[i]), 'r') as f_in:

                rd = csv.reader(f_in)
                for row in rd:
                    row = [float(num) for num in row]
                    ar = np.reshape(row, (WINDOW_SAMPLES, SENS_VALUES))
                    ar = np.transpose(ar)
                    test_set[i]["input"] = np.vstack([test_set[i]["input"], np.dstack(ar)])

                f_in.close()

            with open(os.path.join(dt_csv_dir,  test_out_set[i]), 'r') as f_out:

                rd = csv.reader(f_out)

                for row in rd:
                    ar = np.argmax(row)
                    test_set[i]["output"] = np.append(test_set[i]["output"], ar)

                f_out.close()

            if cmod['CNN']:
                test_set[i]["input"] = test_set[i]["input"].reshape(test_set[i]["input"].shape[0],
                                                                   test_set[i]["input"].shape[1],
                                                                    test_set[i]["input"].shape[2], 1)
            test_set[i]["output"] = keras.utils.to_categorical(test_set[i]["output"], num_classes=num_classes)

    return test_set



def shuffle_dataset(x_ds, y_ds):

    print(x_ds.shape)
    zip_ds = list(zip(x_ds, y_ds))

    rnd.shuffle(zip_ds)

    x_shf, y_shf = zip(*zip_ds)

    return np.array(x_shf), np.array(y_shf)