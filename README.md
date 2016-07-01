# Port of Arduino AccelStepper library to STM32F4 microcontrollers.

### Software
This project contains [example code](https://github.com/IRNAS/AccelStepper/blob/master/STM32_AccelStepper/src/main.c) for using the modified [AccelStepper](https://github.com/IRNAS/AccelStepper/blob/master/STM32_AccelStepper/inc/AccelStepper.h) library to drive unipolar stepper motors. All required HAL drivers were added from [STM32CubeF4 1.12.0](http://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32cube-embedded-software/stm32cubef4.html).

### Environment setup

Programing environment is based on System Workbench for STM32. The System Workbench toolchain, called SW4STM32, is a free multi-OS software development environment based on Eclipse, which supports the full range of STM32 microcontrollers and associated boards. The SW4STM32 toolchain may be obtained from the website www.openstm32.org, which includes forums, blogs, and trainings for technical support. Once registered to this site, users will get installation instructions at the Documentation > System Workbench page to proceed with the download of the free toolchain.

Import project to IDE and compile.

Information on importing project, compiling, and downloading code to the board, can be found here www.openstm32.org.

### Hardware
For this example the [STM32F411RET6](http://www.st.com/content/st_com/en/products/microcontrollers/stm32-32-bit-arm-cortex-mcus/stm32f4-series/stm32f411/stm32f411re.html) MCU was used. This MCU is located on the [STM NUCLEO](http://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f411re.html) board. LED LD2 can be configured for periodic flashing. The stepper motor drivers are connected to pins as follows:

* X axis:
  * PC13 - signal 1
  * PC14 - signal 2
  * PC15 - signal 3
  * PC9 - signal 4
* Y axis:
  * PC2 - signal 1
  * PC3 - signal 2
  * PC5 - signal 3
  * PC11 - signal 4
* F axis:
  * PB1 - signal 1
  * PB2 - signal 2
  * PB10 - signal 3
  * PC12 - signal 4
