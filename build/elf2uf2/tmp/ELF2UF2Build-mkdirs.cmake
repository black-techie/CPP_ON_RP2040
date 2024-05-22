# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/black/pico-sdk/tools/elf2uf2"
  "/Users/black/Desktop/work/codes/cmake/CPP_ON_RP2040/build/elf2uf2"
  "/Users/black/Desktop/work/codes/cmake/CPP_ON_RP2040/build/elf2uf2"
  "/Users/black/Desktop/work/codes/cmake/CPP_ON_RP2040/build/elf2uf2/tmp"
  "/Users/black/Desktop/work/codes/cmake/CPP_ON_RP2040/build/elf2uf2/src/ELF2UF2Build-stamp"
  "/Users/black/Desktop/work/codes/cmake/CPP_ON_RP2040/build/elf2uf2/src"
  "/Users/black/Desktop/work/codes/cmake/CPP_ON_RP2040/build/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/black/Desktop/work/codes/cmake/CPP_ON_RP2040/build/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/black/Desktop/work/codes/cmake/CPP_ON_RP2040/build/elf2uf2/src/ELF2UF2Build-stamp${cfgdir}") # cfgdir has leading slash
endif()
