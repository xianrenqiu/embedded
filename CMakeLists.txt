# CMake minimum version
cmake_minimum_required(VERSION 3.1)
# Project Infomation
project(embedded ASM C)

if(DEFINED SAMPLE)
message("")
message("-- embedded CMake Project for ${SAMPLE} sample.")
message("")
else()
    message(FATAL_ERROR "FATAL: SAMPLE Not Define, 
    You should Specify the sample name. e.g.: cmake -DSAMPLE=hello ..")
endif()

if(WIN32)
    set(TOOLCHAIN_SUFFIX ".exe")
    add_custom_target(FIX-LINK
                      ALL
                      COMMAND ${PROJECT_SOURCE_DIR}/tools/sed/sed.exe -i "s/-Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles\\\\embedded.elf.dir\\\\linklibs.rsp//g" ${PROJECT_SOURCE_DIR}/build/CMakefiles/embedded.elf.dir/link.txt
                      COMMENT "-- fix mingw32 link error.")
else()
    set(TOOLCHAIN_SUFFIX "")
endif()

# For CMAKE Debug
set(CMAKE_VERBOSE_MAKEFILE off)

# Shared library link flag
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

# Reset output path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# CC AR LD AS OBJCOPY SIZE
set(CMAKE_AR arm-none-eabi-ar${TOOLCHAIN_SUFFIX})
set(CMAKE_SIZE arm-none-eabi-size${TOOLCHAIN_SUFFIX})
set(CMAKE_C_COMPILER arm-none-eabi-gcc${TOOLCHAIN_SUFFIX})
set(CMAKE_ASM_COMPILER arm-none-eabi-as${TOOLCHAIN_SUFFIX})
set(CMAKE_OBJCOPY arm-none-eabi-objcopy${TOOLCHAIN_SUFFIX})

# CMSIS
set(CMSIS_DIR ${PROJECT_SOURCE_DIR}/driver/cmsis)

# CFLAGS
set(CMAKE_C_FLAGS 
    "-g -O0 -gdwarf-2 -fno-builtin -std=gnu99 -mcpu=cortex-m3 -mlittle-endian -mthumb -mthumb-interwork -DCONFIG_ARM -DSTM32F103xB -DUSE_HAL_DRIVER -DMBEDTLS_CONFIG_FILE='<mbedtls_config.h>'"
)

# ASMFLAGS
set(CMAKE_ASM_FLAGS 
    "-mcpu=cortex-m3 -mlittle-endian -mthumb"
)

# LDFLAGS
SET(CMAKE_EXE_LINKER_FLAGS 
    "-nostartfiles -nodefaultlibs -nostdlib -Wl,-Map,${CMAKE_PROJECT_NAME}.map -T ${CMSIS_DIR}/gcc_arm.ld"
)

# Debug
set(CMAKE_BUILD_TYPE "Debug")

# Set include path
include_directories(${CMSIS_DIR}/include)
include_directories(${CMSIS_DIR}/device)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/libc)
include_directories(${PROJECT_SOURCE_DIR}/library/mbedtls/include)
include_directories(${PROJECT_SOURCE_DIR}/library/stm32f1xx_hal_driver/inc)
include_directories(${PROJECT_SOURCE_DIR}/sample/${SAMPLE})

# The need build source path and build all files
aux_source_directory(${PROJECT_SOURCE_DIR}/driver DIR_USER_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/library/libc DIR_USER_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/library/mbedtls/library DIR_USER_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/library/stm32f1xx_hal_driver/src DIR_USER_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/sample/${SAMPLE} DIR_USER_SRCS)

# Generate the target
add_executable(${CMAKE_PROJECT_NAME}.elf ${CMSIS_DIR}/startup_stm32f1xx.S ${DIR_USER_SRCS})

# build hex file
add_custom_target(${CMAKE_PROJECT_NAME}.hex
                 ALL
                 DEPENDS ${CMAKE_PROJECT_NAME}.elf${TOOLCHAIN_SUFFIX}
                 COMMAND ${CMAKE_OBJCOPY} -Oihex ${CMAKE_PROJECT_NAME}.elf${TOOLCHAIN_SUFFIX} ${CMAKE_PROJECT_NAME}.hex
                 WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
                 COMMENT "building  ${CMAKE_PROJECT_NAME}.hex")
                
# build bin file
add_custom_target(${CMAKE_PROJECT_NAME}.bin
                  ALL
                  DEPENDS ${CMAKE_PROJECT_NAME}.elf${TOOLCHAIN_SUFFIX}
                  COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf${TOOLCHAIN_SUFFIX} ${CMAKE_PROJECT_NAME}.bin
                  WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
                  COMMENT "building ${CMAKE_PROJECT_NAME}.bin")

# display ELF info
add_custom_target(CMAKE_SIZE
                  ALL
                  DEPENDS ${CMAKE_PROJECT_NAME}.elf${TOOLCHAIN_SUFFIX}
                  COMMAND ${CMAKE_SIZE} -t -x ${CMAKE_PROJECT_NAME}.elf${TOOLCHAIN_SUFFIX}
                  WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})

if(UNIX)
# copy gdb.init file to output path
add_custom_target(gdbinit
                  ALL
                  COMMAND cp ${PROJECT_SOURCE_DIR}/tools/debug/gdb.init ${CMAKE_BINARY_DIR}
                  COMMENT "copying ${PROJECT_SOURCE_DIR}/.gdbinit to ${CMAKE_BINARY_DIR}/.gdbinit")

# copy gdb.sh file to output path
add_custom_target(gdbsh
                  ALL
                  COMMAND cp ${PROJECT_SOURCE_DIR}/tools/debug/gdb.sh ${CMAKE_BINARY_DIR}/gdb.sh
                  COMMENT "copying ${PROJECT_SOURCE_DIR}/gdb.sh to ${CMAKE_BINARY_DIR}/gdb.sh")

# Make clean-all
add_custom_target(clean-all COMMAND rm -rf ${CMAKE_BINARY_DIR}/*)
endif()