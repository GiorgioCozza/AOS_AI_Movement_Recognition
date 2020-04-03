

@echo off
set CUBE_REP_DIR=%cd%\STM32Cube\Repository
set CUBE_AI_DIR=%CUBE_REP_DIR%\Packs\STMicroelectronics\X-CUBE-AI\4.0.0
set CUBE_EX_DIR=%CUBE_AI_DIR%\Utilities\windows
set PATH=%CUBE_EX_DIR%;%PATH%

set PROJ_DIR=.
set PY_DIR=%PROJ_DIR%\neural_net-py
set MIOSIX_DIR=%PROJ_DIR%\miosix-kernel
set NN_DIR=%PROJ_DIR%\STM32CubeAI

set AI_MOD_MIOSIX_DIR=%MIOSIX_DIR%\ai_module
set PY_MODEL_DIR=%PY_DIR%\best_models
set PY_TESTCSV_DIR=%PY_DIR%\test_csv


:: ***************************   USER CONFIGURATION   *******************************

:: Choose the model and the input and output validation csv files
:: for which you want to generate and validate the network C-code
:: IMPORTANT: The chosen model and files should be coherent to avoid 
:: error or ambiguous validation results  

:: MODEL
set MODEL_NAME=best_acc_model_RNN_fold1_02-04-2020_191256.h5

:: TEST_CSV_DIR
set TEST_CSV_DIR=test_Apr02_20

:: TEST_CSV_IN
set TEST_CSV_IN=test_input_fold-1_Apr02_19-04-24.csv

:: TEST_CSV_OUT
set TEST_CSV_OUT=test_output_fold-1_Apr02_19-04-24.csv

:: ***********************************************************************************



set MODEL_PATH=%PY_MODEL_DIR%\%MODEL_NAME%
set TEST_IN_PATH=%PY_TESTCSV_DIR%\%TEST_CSV_DIR%\%TEST_CSV_IN%
set TEST_OUT_PATH=%PY_TESTCSV_DIR%\%TEST_CSV_DIR%\%TEST_CSV_OUT%




ECHO.
ECHO **********************   Generate and Install STM32CubeAI in Miosix OS   ****************************
ECHO.
stm32ai --version
ECHO.
stm32ai analyze -m %MODEL_PATH% --type keras
ECHO.
ECHO [LOG] Validation on Desktop:
ECHO.
stm32ai validate -m %MODEL_PATH% -vi %TEST_IN_PATH% -vo %TEST_OUT_PATH% --mode x86 
ECHO.
stm32ai generate -m %MODEL_PATH% -o %NN_DIR%
ECHO.
ECHO [LOG] Installing into Miosix OS...
ECHO.

python "%NN_DIR%\fix_network_h.py"
copy %NN_DIR%\*.h %AI_MOD_MIOSIX_DIR%\Inc
copy %NN_DIR%\*.c %AI_MOD_MIOSIX_DIR%\Src


PAUSE