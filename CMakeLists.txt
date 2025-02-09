cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(ProjectName wifi_bootloader)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project (${ProjectName})

set(LINKER_SCRIPT linker_script.ld)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostdlib -T ${CMAKE_SOURCE_DIR}/linker_script.ld")

set(SOURCES 
    src/main.c
    src/startup.c
    src/uart.c
    src/logger.c
    src/wifi.c)

# Flash binary onto device
add_custom_target(flash
                    COMMAND openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program ${PROJECT_NAME} verify reset exit")
# Start openocd debug session on localhost:3333
add_custom_target(debug
                    COMMAND openocd -f interface/stlink.cfg -f target/stm32f4x.cfg)
                    
# Todo: maybe change this according to cmd-line arguments passed to the cmake command.
add_compile_options(-mcpu=cortex-m4 -mthumb -nostdlib -g3 -DSTM32F411XE -O0)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/inc
        ${PROJECT_SOURCE_DIR}/cmsis_device_f4/Include
        ${PROJECT_SOURCE_DIR}/CMSIS_5/CMSIS/Core/Include)