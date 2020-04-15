

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

:: TEST_CSV_DIR
set TEST_CSV_DIR=test_Apr09_20

:: TEST_CSV_IN
set TEST_CSV_IN=test_input_fold-1_Apr09_22-09-51.csv

:: TEST_CSV_OUT
set TEST_CSV_OUT=test_output_fold-1_Apr09_22-09-51.csv

:: INSERT COM USB
set COM=COM13
:: ***********************************************************************************


ECHO.
ECHO "**********************  STM32CubeAI: Neural Network Validation  ****************************"
ECHO.
stm32ai --version
ECHO.
ECHO [LOG] Model analysis...
ECHO.
stm32ai analyze -m %MODEL_PATH% --type keras

ECHO.
ECHO [LOG] Validation on Desktop:
ECHO.
stm32ai validate -m %MODEL_PATH% -vi %TEST_IN_PATH% -vo %TEST_OUT_PATH% --mode x86 

ECHO.
:: It is possible to uncomment this line of code only if the 
:: Validation app is compiled and flashed in the board. This
:: is not possible by the CLI.

cd %NN_DIR%
::make
::cd ..
FOR %%u IN (A: B: C: D: E: F: G:) DO (
	FOR /F "delims=: tokens=2" %%a IN ('fsutil fsinfo volumeInfo %%u') DO (
		FOR %%s IN (%%a) DO ( IF "%%s" == "NODE_F401RE" ( ECHO "[LOG] NODE_F401RE Board detected, establishing a connection..."
	STM32_Programmer_CLI -c port=SWD mode=Normal -w "%NN_DIR%\build\NN_Code.bin" 0x08000000
	stm32ai validate -m %MODEL_PATH% -vi %TEST_IN_PATH% -vo %TEST_OUT_PATH% --mode stm32 -d %COM%:115200
	PAUSE )  )
	)	
)

