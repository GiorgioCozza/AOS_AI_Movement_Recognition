
import os

# CLASS LABELS
LABELS = ["running",
          "walking",
          "jumping",
          "standing",
          "sitting",
          "supine",
          "lying_on_side"]

num_classes = len(LABELS)
# HW INFO
SENSORS = ["ACC_LSM6DSL_DS", "GYR_LSM6DSL_DS", "ACC_LSM303AGR_DS", "MAG_LSM303AGR_DS"]
AXIS = ["x", "y", "z"]


# DATASET AND SETTINGS INFO
workspace = os.getcwd()
datasets_dir = os.path.join(workspace, "../datasets/")
session_dir = os.path.join(datasets_dir, "activity_files")
run_ds_path = os.path.join(datasets_dir, "running.txt")
jump_ds_path = os.path.join(datasets_dir, "jumping.txt")
stand_ds_path = os.path.join(datasets_dir, "standing.txt")
walk_ds_path = os.path.join(datasets_dir, "walking.txt")
sit_ds_path =os.path.join( datasets_dir, "sitting.txt")
sup_ds_path = os.path.join(datasets_dir, "supine.txt")
los_ds_path = os.path.join(datasets_dir, "lying_on_side.txt")


log_dir = os.path.join(workspace, "../log/")
mod_dir = os.path.join(workspace, "../saved_models")
csv_dir = os.path.join(workspace, "../test_csv")
best_mod_dir = os.path.join(workspace, "../best_models")

log_path = log_dir
mod_path = mod_dir
csv_path = csv_dir
best_mod_path = best_mod_dir

# DATA PROCESSING INFO
WINDOW_SAMPLES = 30
SENS_VALUES = 12
ds_model = {"ACC_LSM6DSL_DS": {}, "GYR_LSM6DSL_DS": {}, "ACC_LSM303AGR_DS": {}, "MAG_LSM303AGR_DS": {}}


