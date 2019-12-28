
# CLASS LABELS
LABELS = ["running",
          "walking",
          "jumping",
          "standing",
          "football",
          "basketball"]

# HW INFO
SENSORS = ["ACC_LSM6DSL_DS", "GYR_LSM6DSL_DS", "ACC_LSM303AGR_DS", "MAG_LSM303AGR_DS"]
AXIS = ["x", "y", "z"]


# DATASET AND SETTINGS INFO
datasets_dir = "../datasets/"
run_ds_path = datasets_dir + "running.txt"
jump_ds_path = datasets_dir + "jumping.txt"
stand_ds_path = datasets_dir + "standing.txt"
walk_ds_path = datasets_dir + "walking.txt"

log_dir = "../log/"
mod_dir = "../saved_models"
csv_dir = "../test_csv"
best_mod_dir = "../best_models"

log_path = log_dir
mod_path = mod_dir
csv_path = csv_dir
best_mod_path = best_mod_dir

# DATA PROCESSING INFO
WINDOW_SAMPLES = 50
SENS_VALUES = 12
ds_model = {"ACC_LSM6DSL_DS": {}, "GYR_LSM6DSL_DS": {}, "ACC_LSM303AGR_DS": {}, "MAG_LSM303AGR_DS": {}}


