# IMPORT
import numpy as np
import keras
from scripts.dataset_processing import get_dataset, normalize, prepare_data, \
                                       split_train_valid_test, test_on_csv, test_from_csv, \
                                       shuffle_dataset, merge_session_files
import os
from scripts.data_visualization import plot_temporal_activity, show_activities_histogram, show_activity_files, activity_scattering3D
from scripts.config import *
from scripts.neural_network import CNN_model, train_model, test_model_set, save_best_models, RNN_model
from datetime import datetime as dt


def set_env():
    workspace = os.getcwd()
    mod_path = os.path.join(workspace, mod_dir)

    if not os.path.exists(mod_path):
        os.makedirs(mod_path)
        os.makedirs(best_mod_dir)

    if not os.path.exists(best_mod_dir):
        os.makedirs(best_mod_dir)

    if not os.path.exists(log_dir):
        os.makedirs(log_dir)

    if not os.path.exists(plot_dir):
        os.makedirs(plot_dir)

    if not os.path.exists(csv_dir):
        os.makedirs(csv_dir)

    csv_dir_name = 'test_' + dt.now().strftime('%b%d_%y')
    td_csv_dir = os.path.join(csv_dir, csv_dir_name)

    if not os.path.exists(td_csv_dir):
        os.makedirs(td_csv_dir)



def model_menu():
    c = 0
    while c not in list(cmod.values()):
        m_c = 1
        print("###########################   MODEL MENU   ###################################")
        print("\n")
        for mod in cmod.keys():
            print("\t" + str(m_c) + ". " + mod)
            m_c += 1
        print("\n")
        print("#################################################################################")

        c = input("\nCHOOSE MODEL TO TRAIN: ")
        if c.isdigit():
            c = int(c)

        if c not in list(cmod.values()):
            print("[ERROR]: Bad input!")

    mod_key = list(cmod.keys())[list(cmod.values()).index(c)]
    mod_val = c
    return mod_key, mod_val


def main():
    set_env()

    # merge all the activity files in a single dataset (for each label)
    merge_session_files(datasets_dir, LABELS[0])
    merge_session_files(datasets_dir, LABELS[1])
    merge_session_files(datasets_dir, LABELS[2])
    merge_session_files(datasets_dir, LABELS[3])
    merge_session_files(datasets_dir, LABELS[4])
    merge_session_files(datasets_dir, LABELS[5])
    merge_session_files(datasets_dir, LABELS[6])

    c = show_activity_files()
    # copy into the list all the activity files to be plotted
    act_files_scat = ['walking_23-02-2020_165231.txt',
                 'standing_29-12-2019_063829.txt',
                 'jumping_25-02-2020_094215.txt',
                 'running_24-02-2020_151633.txt',
                 'supine_24-02-2020_125443.txt',
                 'sitting_24-02-2020_081708.txt',
                 'lying_on_side_24-02-2020_134922.txt']

    act_files_plot = ['supine_24-02-2020_125443.txt',
                 'sitting_24-02-2020_081708.txt',
                 'lying_on_side_24-02-2020_134922.txt']

    # UNCOMMENT TO SHOW PLOTS
    act_files_scat = [os.path.join(session_dir, af) for af in act_files_scat]
    act_files_plot = [os.path.join(session_dir, af) for af in act_files_plot]
    #plot_temporal_activity(act_files_plot, limit=WINDOW_SAMPLES*3)
    #activity_scattering3D(act_files_scat, limit=WINDOW_SAMPLES)
    #show_activities_histogram()


    # dataset collection
    stand_dataset = get_dataset(stand_ds_path)
    run_dataset = get_dataset(run_ds_path)
    walk_dataset = get_dataset(walk_ds_path)
    jump_dataset = get_dataset(jump_ds_path)
    sit_dataset = get_dataset(sit_ds_path)
    sup_dataset = get_dataset(sup_ds_path)
    los_dataset = get_dataset(los_ds_path)


    # prepare NN input
    proc_run, run_res = prepare_data(run_dataset, 0)
    proc_walk, walk_res = prepare_data(walk_dataset, 1)
    proc_jump, jump_res = prepare_data(jump_dataset, 2)
    proc_stand, stand_res = prepare_data(stand_dataset, 3)
    proc_sit, sit_res = prepare_data(sit_dataset, 4)
    proc_sup, sup_res = prepare_data(sup_dataset, 5)
    proc_los, los_res = prepare_data(los_dataset, 6)

    batch_len = proc_run.shape[0]

    print("\r\nREADY to TRAIN the NN, Please insert the ratio of the data batches that will be used for testing:\r\n")
    ratio = input("RATIO: ")
    ratio = float(ratio)

    valtest_batches = 2 * int(np.round(batch_len * ratio))
    folds = int(np.floor(batch_len/valtest_batches))

    nn_models = []
    test_iset = []
    test_oset = []

    k, v = model_menu()

    if v == cmod['CNN']:
        mod_fun = CNN_model
    elif v == cmod['RNN']:
        mod_fun = RNN_model

    for j in range(0, folds):
        nn_models.append(mod_fun())       #substitute with CNN_model()/RNN_model() to train the two solutions

    print("\r\nMINIMUM FOLD CYCLE: " + str(folds))
    for i in range(0, folds):
        print("\r\nSTART TRAINING AND TESTING USING FOLD " + str(i) + "\r\n")

        proc_run_dict = split_train_valid_test(proc_run, run_res, ratio, i)
        proc_stand_dict = split_train_valid_test(proc_stand, stand_res, ratio, i)
        proc_walk_dict = split_train_valid_test(proc_walk, walk_res, ratio, i)
        proc_jump_dict = split_train_valid_test(proc_jump, jump_res, ratio, i)
        proc_sit_dict = split_train_valid_test(proc_sit, sit_res, ratio, i)
        proc_sup_dict = split_train_valid_test(proc_sup, sup_res, ratio, i)
        proc_los_dict = split_train_valid_test(proc_los, los_res, ratio, i)

        train_results = np.vstack((proc_run_dict["training"]["output"], proc_stand_dict["training"]["output"],
                                   proc_walk_dict["training"]["output"], proc_jump_dict["training"]["output"],
                                   proc_sit_dict['training']['output'], proc_sup_dict['training']['output'],
                                   proc_los_dict['training']['output']))

        test_results = np.vstack((proc_run_dict["testing"]["output"], proc_stand_dict["testing"]["output"],
                                  proc_walk_dict["testing"]["output"], proc_jump_dict["testing"]["output"],
                                  proc_sit_dict['testing']['output'], proc_sup_dict['testing']['output'],
                                  proc_los_dict['testing']['output']
                                  ))

        valid_results = np.vstack((proc_run_dict["validation"]["output"], proc_stand_dict["validation"]["output"],
                                   proc_walk_dict["validation"]["output"], proc_jump_dict["validation"]["output"],
                                   proc_sit_dict['validation']['output'], proc_sup_dict['validation']['output'],
                                   proc_los_dict['validation']['output']
                                   ))

        x_train = np.vstack((proc_run_dict["training"]["input"], proc_stand_dict["training"]["input"],
                             proc_walk_dict["training"]["input"], proc_jump_dict["training"]["input"],
                             proc_sit_dict['training']['input'], proc_sup_dict['training']['input'],
                             proc_los_dict['training']['input']
                             ))

        x_test = np.vstack((proc_run_dict["testing"]["input"], proc_stand_dict["testing"]["input"],
                            proc_walk_dict["testing"]["input"], proc_jump_dict["testing"]["input"],
                            proc_sit_dict['testing']['input'], proc_sup_dict['testing']['input'],
                            proc_los_dict['testing']['input']
                            ))

        x_valid = np.vstack((proc_run_dict["validation"]["input"], proc_stand_dict["validation"]["input"],
                             proc_walk_dict["validation"]["input"], proc_jump_dict["validation"]["input"],
                             proc_sit_dict['validation']['input'], proc_sup_dict['validation']['input'],
                             proc_los_dict['validation']['input']
                             ))

        y_train = keras.utils.to_categorical(train_results, num_classes=7)
        y_test = keras.utils.to_categorical(test_results, num_classes=7)
        y_valid = keras.utils.to_categorical(valid_results, num_classes=7)

        if v == cmod['CNN']:
            x_train = x_train.reshape(x_train.shape[0], x_train.shape[1], x_train.shape[2], 1)
            x_valid = x_valid.reshape(x_valid.shape[0], x_valid.shape[1], x_valid.shape[2], 1)
            x_test = x_test.reshape(x_test.shape[0], x_test.shape[1], x_test.shape[2], 1)

        x_train, y_train = shuffle_dataset(x_train, y_train)
        x_valid, y_valid = shuffle_dataset(x_valid, y_valid)
        x_test, y_test = shuffle_dataset(x_test, y_test)

        cur_icsv, cur_ocsv = test_on_csv(x_test, y_test, fold_n=i)
        test_iset = np.append(test_iset, cur_icsv)
        test_oset = np.append(test_oset, cur_ocsv)

        train_model(model=nn_models[i], x_train=x_train, y_train=y_train, x_valid=x_valid, y_valid=y_valid)

    test_set = test_from_csv(test_iset, test_oset, v)
    best_acc_mod, best_loss_mod, best_acc_fold, best_loss_fold = test_model_set(model_set=nn_models, test_set=test_set)
    save_best_models(best_acc_mod, best_loss_mod, model_name=k, acc_fold=best_acc_fold, loss_fold=best_loss_fold)


if __name__ == '__main__':
    main()
