

@echo off

:: ******************  STM32CubeAI CONFIG  ******************************************
set CUBE_REP_DIR=%HOMEPATH%\STM32Cube\Repository
set CUBE_AIv5_DIR=%CUBE_REP_DIR%\Packs\STMicroelectronics\X-CUBE-AI\5.0.0
set CUBE_EX_DIR=%CUBE_AIv5_DIR%\Utilities\windows
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

:: STM32CUBEAI PROJECT NAME
set CUBEAI_PROJ_NAME=NN_Code

:: ***********************************************************************************

set CUBEAI_PROJ_PATH=%PROJ_DIR%\%CUBEAI_PROJ_NAME%
set MODEL_PATH=%PY_MODEL_DIR%\%MODEL_NAME%


ECHO.
ECHO "**********************   Generate and Install STM32CubeAI in Miosix OS   ****************************"
ECHO.
stm32ai --version
ECHO.
stm32ai generate -m %MODEL_PATH% -o %CUBEAI_PROJ_PATH%

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

FOR %%u IN (A: B: C: D: E: F: G:) DO (
	FOR /F "delims=: tokens=2" %%a IN ('fsutil fsinfo volumeInfo %%u') DO (
		FOR %%s IN (%%a) DO ( IF "%%s" == %BOARD% ( ECHO "[LOG] NODE_F401RE Board detected, establishing a connection..."
			STM32_Programmer_CLI -c port=SWD mode=Normal -w "%MIOSIX_DIR%/main.bin" 0x08000000
				))
			)	
		)
	)
PAUSE