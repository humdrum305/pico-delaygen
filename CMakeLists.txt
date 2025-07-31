# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2_w CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-delaygen C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb")
    message("DBPRINT build")
endif()

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()
#add_compile_definitions(PICO_DEFAULT_LED_PIN=25)
# Initialize the SDK
pico_sdk_init()

if (NOT PICO_CYW43_SUPPORTED)
    message("Only building blink_any for non W boards as PICO_CYW43_SUPPORTED is not set")
endif()

add_executable(${PROJECT_NAME}
    src/main.c
    src/oc.c
   src/main.h
)

# pull in common dependencies
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_pio
    pico_unique_id
    pico_cyw43_arch_none    # or lwIP variant if using networking
)
# Include header files
#target_include_directories(${PROJECT_NAME} 
#    PRIVATE 
#    "include"
#)

# Set custom USB name
target_compile_options(${PROJECT_NAME} PRIVATE "-DUSBD_PRODUCT=\"Delay Generator\"")

# disable uart output, enable usb output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Create C header file with the name <pio program>.pio.h
pico_generate_pio_header(${PROJECT_NAME}  
    ${CMAKE_CURRENT_LIST_DIR}/src/square.pio
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${PROJECT_NAME})

# clocking the flash so it keeps up
pico_define_boot_stage2(custom-boot ${PICO_DEFAULT_BOOT_STAGE2_FILE})
target_compile_definitions(custom-boot PRIVATE PICO_FLASH_SPI_CLKDIV=4)
pico_set_boot_stage2(${PROJECT_NAME} custom-boot)

