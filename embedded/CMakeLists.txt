# This configuration file outlines some of the ways Arduino CMake
# can be used. For a detailed explenation of all the options please
# reade README.rst.
# https://github.com/queezythegreat/arduino-cmake

set(ARDUINO_SDK_PATH /home/mehdi/Mehdi/programmation/arduino/arduino-1.0.5/arduino-1.0.5/)


set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/ArduinoToolchain.cmake) # Arduino Toolchain

cmake_minimum_required(VERSION 2.8)
#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(PowerFailureAlarm C CXX)

print_board_list()
print_programmer_list()




set(FIRMWARE_NAME PowerFailureAlarm)

execute_process(COMMAND find /dev/ -name ttyACM* 
				 COMMAND tr -d '\n'
                OUTPUT_VARIABLE serialPort)

Message("Serial port " ${serialPort} " will be used")

set(ARDUINO_DEFAULT_BOARD micro) # Default Board ID, when not specified
set(ARDUINO_DEFAULT_PORT ${serialPort}) # Default Port, when not specified
set(${FIRMWARE_NAME}_SERIAL "cat @INPUT_PORT@")


set (LIBRARIES_PATH "/usr/share/arduino/libraries")


include_directories(
	inc
	../lib/Settings/inc
)

add_definitions("-Dboard_${ARDUINO_DEFAULT_BOARD}")

#====================================================================#
# Advanced static library exmaple
#====================================================================#
#generate_arduino_library(blink_lib
#    SRCS src/blink_lib.cpp
#    HDRS inc/blink_lib.h
#    BOARD uno)

# Alternative: by variables
#set(blink_lib_SRCS blink_lib.cpp)
#set(blink_lib_HDRS blink_lib.h)
#set(blink_lib_PORT /dev/ttyACM0)
#set(blink_lib_BOARD uno)
#
#generate_arduino_library(blink_lib)
message(${LIBRARIES_PATH}/EEPROM/EEPROM.cpp)
FILE( GLOB source_files_PowerFailureAlarm ../lib/Settings/src/*.cpp ./src/*.cpp ./src/*.c ${LIBRARIES_PATH}/EEPROM/EEPROM.cpp
)
message(${source_files_PowerFailureAlarm})


 ADD_CUSTOM_TARGET(
        findPort COMMAND
        COMMAND find /dev/ -name ttyACM* 
				 COMMAND tr -d '\n'
                OUTPUT_VARIABLE serialPort) 


#====================================================================#
# Advanced firwmare example
#====================================================================#
generate_arduino_firmware(PowerFailureAlarm
    SRCS ${source_files_PowerFailureAlarm}
    BOARD ${ARDUINO_DEFAULT_BOARD}
    )

# Alternative: by variables
#set(blink_SRCS blink.cpp)
#set(blink_LIBS blink_lib)
#
#generate_arduino_firmware(blink)

add_custom_target(serial
    COMMAND ttylog -d ${ARDUINO_DEFAULT_PORT} -b 9600 -f
)
