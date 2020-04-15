

@echo off

:: ******************  STM32CubeAI CONFIG  ******************************************
set CUBE_REP_DIR=%HOMEPATH%\STM32Cube\Repository
set CUBE_AI_DIR=%CUBE_REP_DIR%\Packs\STMicroelectronics\X-CUBE-AI\4.0.0
set CUBE_EX_DIR=%CUBE_AI_DIR%\Utilities\windows
set PATH=%CUBE_EX_DIR%;%PATH%
:: **********************************************************************************

set BOARD="NODE_F401RE"

:: *****************  STM32CubeProgrammer  ******************************************
set CUBE_PROG_DIR="C:\Program Files (x86)\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin"
set PATH=%CUBE_PROG_DIR%;%PATH%
:: **********************************************************************************


set PROJ_DIR=%~dp0
set PY_DIR=%PROJ_DIR%\neural_net-py
set MIOSIX_DIR=%PROJ_DIR%\miosix-kernel
set NN_DIR=%PROJ_DIR%\NN_Code
set PY_INSTALLER_DIR=%PROJ_DIR%\installer_py
set AI_MOD_MIOSIX_DIR=%MIOSIX_DIR%\ai_module
set PY_MODEL_DIR=%PY_DIR%\best_models
set PY_TESTCSV_DIR=%PY_DIR%\test_csv

set ARM_MIOSIX_EABI=C:\arm-miosix-eabi\bin

:: ***************************   USER CONFIGURATION   *******************************

:: Choose the model and the input and output validation csv files
:: for which you want to generate and validate the network C-code
:: IMPORTANT: The chosen model and files should be coherent to avoid 
:: error or ambiguous validation results  

:: MODEL
set MODEL_NAME=best_loss_RNN_fold3_09-04-2020_221243.h5

:: ***********************************************************************************



set MODEL_PATH=%PY_MODEL_DIR%\%MODEL_NAME%
set TEST_IN_PATH=%PY_TESTCSV_DIR%\%TEST_CSV_DIR%\%TEST_CSV_IN%
set TEST_OUT_PATH=%PY_TESTCSV_DIR%\%TEST_CSV_DIR%\%TEST_CSV_OUT%



ECHO.
ECHO "**********************   Generate and Install STM32CubeAI in Miosix OS   ****************************"
ECHO.
stm32ai --version
ECHO.
stm32ai analyze -m %MODEL_PATH% --type keras

ECHO.
stm32ai generate -m %MODEL_PATH% -o %NN_DIR%
ECHO.
ECHO [LOG] Installing into Miosix OS...
ECHO.

python "%PY_INSTALLER_DIR%\installer.py" %PROJ_DIR%
cd %MIOSIX_DIR%
ECHO "[LOG] Clean Miosix Installation..."
ECHO.
make clean
ECHO "[LOG] Compiling Miosix OS..."
make
cd %PROJ_DIR%

PAUSE