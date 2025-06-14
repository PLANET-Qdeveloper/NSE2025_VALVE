cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

project(NSE2025_VALVE C ASM)

# Basic settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable debug information
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3 -O0")
set(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g3")

# Enable languages
enable_language(C ASM)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Add custom source files
file(GLOB CUSTOM_SOURCES
    "Core/Src/*.c"
    "FATFS/App/*.c"
    "FATFS/Target/*.c"
)

# Remove system_stm32f4xx.c from CUSTOM_SOURCES as it's already included in STM32_Drivers
list(FILTER CUSTOM_SOURCES EXCLUDE REGEX ".*system_stm32f4xx\\.c$")
# Remove duplicate fatfs_sd.c from Core/Src
list(FILTER CUSTOM_SOURCES EXCLUDE REGEX ".*Core/Src/fatfs_sd\\.c$")

# Set MX_APPLICATION_SOURCES
set(MX_APPLICATION_SOURCES ${MX_Application_Src})

# Create executable
add_executable(${CMAKE_PROJECT_NAME} 
    ${CUSTOM_SOURCES} 
    ${MX_APPLICATION_SOURCES}
    startup_stm32f413xx.s
)

# Configure target
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    Core/Inc
    FATFS/Target
    FATFS/App
    Middlewares/Third_Party/FatFs/src
)

# Apply STM32CubeMX settings
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${MX_LINK_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${MX_LINK_LIBS})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${MX_CLEAN_FILES})
