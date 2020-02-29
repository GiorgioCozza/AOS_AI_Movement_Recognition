
import seaborn as sns
from datetime import datetime as dt
from sklearn import metrics
from matplotlib import pyplot as plt
import os
import pandas as pd
import numpy as np
from scripts.nn_config import *
from scripts.dataset_processing import get_activity_session, get_dataset


def plot_temporal_activity(filepath, limit=0):
    ds_view = get_activity_session(filepath)
    ds_view['ts'] = [i for i in range(1, len(ds_view[SENSORS[0]]['x']) + 1)]
    session = filepath.split('/')[-1]

    for sens in SENSORS:

        fig, (ax0, ax1, ax2) = plt.subplots(nrows=3, figsize=(15, 10), sharex=True)

        if limit > 0:
            plot_axis(ax0, ds_view['ts'][:limit], ds_view[sens]['x'][:limit], 'X')
            plot_axis(ax1, ds_view['ts'][:limit], ds_view[sens]['y'][:limit], 'Y')
            plot_axis(ax2, ds_view['ts'][:limit], ds_view[sens]['z'][:limit], 'Z')
        else:
            plot_axis(ax0, ds_view['ts'], ds_view[sens]['x'], 'X')
            plot_axis(ax1, ds_view['ts'], ds_view[sens]['y'], 'Y')
            plot_axis(ax2, ds_view['ts'], ds_view[sens]['z'], 'Z')

        plt.subplots_adjust(hspace=0.2)
        fig.suptitle(session)
        plt.subplots_adjust(top=0.90)
        plt.show()


def plot_axis(ax, x, y, title):
    ax.plot(x, y, 'r')
    ax.set_title(title)
    ax.xaxis.set_visible(False)
    ax.set_ylim([min(y) - np.std(y), max(y) + np.std(y)])
    ax.set_xlim([min(x), max(x)])
    ax.grid(True)


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


def show_activities_histogram():
    plt.style.use('ggplot')
    smp_cnt = []
    activities = []
    for act in LABELS:
        file_path = os.path.join(datasets_dir, act+'.txt')
        if os.path.exists(file_path):
            smp_cnt.append(len(open(file_path, mode='r').readlines()))
            activities.append(act)

    x = pd.Series(smp_cnt, index=activities)
    x.plot(kind='bar', grid=True)

    plt.show()