# IMPORT
import numpy as np
import keras
from scripts.dataset_processing import get_dataset, normalize, prepare_data, split_train_valid_test, test_on_csv, test_from_csv
import os
from scripts.nn_config import *
from scripts.neural_network import CNN_model, train_model, test_model_set, get_folds, save_best_models
from datetime import datetime as dt


def set_env():
    workspace = os.getcwd()
    mod_path = os.path.join(workspace, mod_dir)
    best_mod_path = os.path.join(mod_path, best_mod_dir)

    if not os.path.exists(mod_path):
        os.makedirs(mod_path)
        os.makedirs(best_mod_path)

    if not os.path.exists(best_mod_path):
        os.makedirs(best_mod_path)

    log_path = os.path.join(workspace, log_dir)
    if not os.path.exists(log_path):
        os.makedirs(log_path)

    csv_path = os.path.join(workspace, csv_dir)
    if not os.path.exists(csv_path):
        os.makedirs(csv_path)

    csv_dir_name = 'test_' + dt.now().strftime('%b%d_%y')
    td_csv_dir = os.path.join(csv_path, csv_dir_name)

    if not os.path.exists(td_csv_dir):
        os.makedirs(td_csv_dir)



def main():
    set_env()

    # dataset collection
    stand_dataset = get_dataset(stand_ds_path)
    run_dataset = get_dataset(run_ds_path)
    walk_dataset = get_dataset(walk_ds_path)
    jump_dataset = get_dataset(jump_ds_path)

    stand_num = len(stand_dataset[SENSORS[0]]['x'])
    run_num = len(run_dataset[SENSORS[0]]['x'])
    walk_num = len(walk_dataset[SENSORS[0]]['x'])
    jump_num = len(jump_dataset[SENSORS[0]]['x'])

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

    batches_len = [proc_run.shape[0], proc_stand.shape[0], proc_walk.shape[0], proc_jump.shape[0]]

    ratio, folds = get_folds(batches_len)

    nn_models = []
    test_iset = []
    test_oset = []

    for j in range(0, folds - 1):
        nn_models.append(CNN_model())

    print("\r\nMINIMUM FOLD CYCLE: " + str(folds))
    for i in range(0, folds - 1):
        print("\r\nSTART TRAINING AND TESTING USING FOLD " + str(i) + "\r\n")

        proc_run_dict = split_train_valid_test(proc_run, run_res, ratio, i)
        proc_stand_dict = split_train_valid_test(proc_stand, stand_res, ratio, i)
        proc_walk_dict = split_train_valid_test(proc_walk, walk_res, ratio, i)
        proc_jump_dict = split_train_valid_test(proc_jump, jump_res, ratio, i)

        train_results = np.vstack((proc_run_dict["training"]["output"], proc_stand_dict["training"]["output"],
                                   proc_walk_dict["training"]["output"], proc_jump_dict["training"]["output"]))

        test_results = np.vstack((proc_run_dict["testing"]["output"], proc_stand_dict["testing"]["output"],
                                  proc_walk_dict["testing"]["output"], proc_jump_dict["testing"]["output"]))

        valid_results = np.vstack((proc_run_dict["validation"]["output"], proc_stand_dict["validation"]["output"],
                                   proc_walk_dict["validation"]["output"], proc_jump_dict["validation"]["output"]))


        x_train = np.vstack((proc_run_dict["training"]["input"], proc_stand_dict["training"]["input"],
                             proc_walk_dict["training"]["input"], proc_jump_dict["training"]["input"]))

        x_test = np.vstack((proc_run_dict["testing"]["input"], proc_stand_dict["testing"]["input"],
                            proc_walk_dict["testing"]["input"], proc_jump_dict["testing"]["input"]))

        x_valid = np.vstack((proc_run_dict["validation"]["input"], proc_stand_dict["validation"]["input"],
                             proc_walk_dict["validation"]["input"], proc_jump_dict["validation"]["input"]))

        y_train = keras.utils.to_categorical(train_results, num_classes=4)
        y_test = keras.utils.to_categorical(test_results, num_classes=4)
        y_valid = keras.utils.to_categorical(valid_results, num_classes=4)

        x_train = x_train.reshape(x_train.shape[0], x_train.shape[1], x_train.shape[2], 1)
        x_valid = x_valid.reshape(x_valid.shape[0], x_valid.shape[1], x_valid.shape[2], 1)
        x_test = x_test.reshape(x_test.shape[0], x_test.shape[1], x_test.shape[2], 1)

        cur_icsv, cur_ocsv = test_on_csv(x_test, y_test, fold_n=i)
        test_iset = np.append(test_iset, cur_icsv)
        test_oset = np.append(test_oset, cur_ocsv)

        train_model(model=nn_models[i], x_train=x_train, y_train=y_train, x_valid=x_valid, y_valid=y_valid)

    test_set = test_from_csv(test_iset, test_oset)
    best_acc_mod, best_loss_mod = test_model_set(model_set=nn_models, test_set=test_set)
    save_best_models(best_acc_mod, best_loss_mod)


if __name__ == '__main__':
    main()
