cmake_minimum_required(VERSION 3.12)

# pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

project(blink C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

add_executable(blink
                blink.c
                )

# Pull in our pico_stdlib which pulls in commonl
target_link_libraries(blink pico_stdlib)

#create map/bin/hex file etc
pico_add_extra_outputs(blink)


#remember to export your sdk Path
# $ export  PICO_SDK_PATH=/home/usr/.... 
