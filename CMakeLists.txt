cmake_minimum_required(VERSION 3.13)

# pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

project(prepaid_water_meter C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

add_executable  (   main   
                    main.cpp
                    services/MFRC522_.cpp
                    services/MFRC522.cpp
                )

# Pull in our pico_stdlib which pulls in commonl

target_link_libraries(  main 
                        pico_stdlib 
                        hardware_i2c
                        hardware_spi
                    )

#create map/bin/hex file etc
pico_add_extra_outputs(main)

