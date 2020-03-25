
import seaborn as sns
from datetime import datetime as dt
from sklearn import metrics
from sklearn.preprocessing import normalize
from mpl_toolkits.mplot3d import axes3d
from matplotlib import pyplot as plt
import os
import pandas as pd
import numpy as np
from scripts.config import *
from scripts.dataset_processing import get_activity_session, get_dataset


def plot_temporal_activity(filepaths=[], limit=0):

    activities = [el.split('\\')[-1].split('_')[0] for el in filepaths]
    for sens in SENSORS:
        fig, (ax0, ax1, ax2) = plt.subplots(nrows=3, figsize=(10, 10))
        ds_view = {}
        for fp, act in zip(filepaths, activities):
            ds = get_activity_session(fp)
            ds_view['ts'] = [i for i in range(1, len(ds[SENSORS[0]]['x']) + 1)]
            ds_view[act] = {}
            ds_view[act]['x'] = ds[sens]['x']
            ds_view[act]['y'] = ds[sens]['y']
            ds_view[act]['z'] = ds[sens]['z']

            session = fp.split('/')[-1]

            if limit > 0:
                plot_axis(ax0, ds_view['ts'][:limit], normalize([ds_view[act]['x'][WINDOW_SAMPLES*10:WINDOW_SAMPLES*10+limit]])[0], 'X')
                plot_axis(ax1, ds_view['ts'][:limit], normalize([ds_view[act]['y'][WINDOW_SAMPLES*10:WINDOW_SAMPLES*10+limit]])[0], 'Y')
                plot_axis(ax2, ds_view['ts'][:limit], normalize([ds_view[act]['z'][WINDOW_SAMPLES*10:WINDOW_SAMPLES*10+limit]])[0], 'Z')
            else:
                plot_axis(ax0, ds_view['ts'][:limit], normalize([ds_view[act]['x']])[0], 'X')
                plot_axis(ax1, ds_view['ts'][:limit], normalize([ds_view[act]['y']])[0], 'Y')
                plot_axis(ax2, ds_view['ts'][:limit], normalize([ds_view[act]['z']])[0], 'Z')

            plt.subplots_adjust(hspace=0.2)
            fig.suptitle(sens)
            plt.subplots_adjust(top=0.90)
            fnact = '_'.join(activities)
            plotname = 'plot_' + sens + '_' + fnact + '.png'
            plot_path = os.path.join(plot_dir, plotname)

        plt.legend(activities, loc='upper left')
        fig.savefig(plot_path)
        plt.show()



def plot_axis(ax, x, y, title):
    ax.plot(x, y, '-')
    ax.set_title(title)
    ax.xaxis.set_visible(True)
    #ax.set_ylim([min(y) - np.std(y), max(y) + np.std(y)])
    ax.set_xlim([min(x), max(x)])
    ax.grid(True)


def show_confusion_matrix(validations, predictions):

    matrix = metrics.confusion_matrix(validations, predictions)
    fig = plt.figure(figsize=(7, 5))
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
    plotname = 'confmat_' + dt.now().strftime("%d-%m-%Y_%H%M%S") + '.png'
    plot_path = os.path.join(plot_dir, plotname)
    fig.savefig(plot_path)
    plt.show()


def show_activities_histogram():
    plt.style.use('ggplot')
    smp_cnt = []
    activities = []
    fig = plt.figure(figsize=(10, 7))
    for act in LABELS:
        file_path = os.path.join(datasets_dir, act+'.txt')
        if os.path.exists(file_path):
            smp_cnt.append(len(open(file_path, mode='r').readlines()))
            activities.append(act)

    x = pd.Series(smp_cnt, index=activities)
    sns.barplot(x=LABELS, y=smp_cnt)
    fig.suptitle('Dataset Distribution')
    for j, val in enumerate(smp_cnt):
        plt.text(j, val + 100, "%d" % val, ha="center")
    plotname = 'data_dist' + '.png'
    plot_path = os.path.join(plot_dir, plotname)
    fig.savefig(plot_path)
    plt.show()


def show_activity_files():

    m_c = 1
    print("###########################   SESSION FILES   ###################################")
    print("\n")
    actfiles = os.listdir(session_dir)
    for file in actfiles:
        print("\t" + str(m_c) + ". " + str(file))
        m_c += 1
    print("\n")
    print("#################################################################################")




def activity_scattering3D(filepaths=[], limit=0):

    activities = [el.split('\\')[-1].split('_')[0] for el in filepaths]

    for sens in SENSORS:
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        ds_view = {}
        for fp, act in zip(filepaths, activities):
            ds = get_activity_session(fp)
            ds_view['ts'] = [i for i in range(1, len(ds[SENSORS[0]]['x']) + 1)]
            ds_view[act] = {}
            ds_view[act]['x'] = ds[sens]['x']
            ds_view[act]['y'] = ds[sens]['y']
            ds_view[act]['z'] = ds[sens]['z']

            session = fp.split('/')[-1]

            if limit > 0:

                ax.scatter(normalize([ds_view[act]['x'][WINDOW_SAMPLES*10:WINDOW_SAMPLES*10+limit]])[0],
                           normalize([ds_view[act]['y'][WINDOW_SAMPLES*10:WINDOW_SAMPLES*10+limit]])[0],
                           normalize([ds_view[act]['z'][WINDOW_SAMPLES*10:WINDOW_SAMPLES*10+limit]])[0], marker='^')


            ax.set_xlabel('X axis')
            ax.set_ylabel('Y axis')
            ax.set_zlabel('Z axis')

            plt.subplots_adjust(hspace=0.2)
            fig.suptitle(sens)
            plt.subplots_adjust(top=0.90)
            plotname = 'scatter3d_' + sens + '.png'
            plot_path = os.path.join(plot_dir, plotname)

        plt.legend(activities, loc='upper left')
        fig.savefig(plot_path)
        plt.show()

