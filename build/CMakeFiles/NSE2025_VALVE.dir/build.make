# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build

# Include any dependencies generated for this target.
include CMakeFiles/NSE2025_VALVE.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/NSE2025_VALVE.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/NSE2025_VALVE.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/NSE2025_VALVE.dir/flags.make

CMakeFiles/NSE2025_VALVE.dir/codegen:
.PHONY : CMakeFiles/NSE2025_VALVE.dir/codegen

CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.obj: CMakeFiles/NSE2025_VALVE.dir/flags.make
CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.obj: /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/main.c
CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.obj: CMakeFiles/NSE2025_VALVE.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.obj"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.obj -MF CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.obj.d -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.obj -c /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/main.c

CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.i"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/main.c > CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.i

CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.s"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/main.c -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.s

CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.obj: CMakeFiles/NSE2025_VALVE.dir/flags.make
CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.obj: /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/gpio.c
CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.obj: CMakeFiles/NSE2025_VALVE.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.obj"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.obj -MF CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.obj.d -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.obj -c /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/gpio.c

CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.i"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/gpio.c > CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.i

CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.s"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/gpio.c -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.s

CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.obj: CMakeFiles/NSE2025_VALVE.dir/flags.make
CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.obj: /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/tim.c
CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.obj: CMakeFiles/NSE2025_VALVE.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.obj"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.obj -MF CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.obj.d -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.obj -c /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/tim.c

CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.i"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/tim.c > CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.i

CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.s"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/tim.c -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.s

CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.obj: CMakeFiles/NSE2025_VALVE.dir/flags.make
CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.obj: /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/usart.c
CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.obj: CMakeFiles/NSE2025_VALVE.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.obj"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.obj -MF CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.obj.d -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.obj -c /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/usart.c

CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.i"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/usart.c > CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.i

CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.s"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/usart.c -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.s

CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.obj: CMakeFiles/NSE2025_VALVE.dir/flags.make
CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.obj: /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/stm32f3xx_it.c
CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.obj: CMakeFiles/NSE2025_VALVE.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.obj"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.obj -MF CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.obj.d -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.obj -c /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/stm32f3xx_it.c

CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.i"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/stm32f3xx_it.c > CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.i

CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.s"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/stm32f3xx_it.c -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.s

CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.obj: CMakeFiles/NSE2025_VALVE.dir/flags.make
CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.obj: /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/stm32f3xx_hal_msp.c
CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.obj: CMakeFiles/NSE2025_VALVE.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.obj"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.obj -MF CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.obj.d -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.obj -c /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/stm32f3xx_hal_msp.c

CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.i"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/stm32f3xx_hal_msp.c > CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.i

CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.s"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/stm32f3xx_hal_msp.c -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.s

CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.obj: CMakeFiles/NSE2025_VALVE.dir/flags.make
CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.obj: /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/sysmem.c
CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.obj: CMakeFiles/NSE2025_VALVE.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.obj"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.obj -MF CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.obj.d -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.obj -c /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/sysmem.c

CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.i"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/sysmem.c > CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.i

CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.s"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/sysmem.c -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.s

CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.obj: CMakeFiles/NSE2025_VALVE.dir/flags.make
CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.obj: /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/syscalls.c
CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.obj: CMakeFiles/NSE2025_VALVE.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.obj"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.obj -MF CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.obj.d -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.obj -c /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/syscalls.c

CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.i"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/syscalls.c > CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.i

CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.s"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/Core/Src/syscalls.c -o CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.s

CMakeFiles/NSE2025_VALVE.dir/startup_stm32f303x8.s.obj: CMakeFiles/NSE2025_VALVE.dir/flags.make
CMakeFiles/NSE2025_VALVE.dir/startup_stm32f303x8.s.obj: /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/startup_stm32f303x8.s
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building ASM object CMakeFiles/NSE2025_VALVE.dir/startup_stm32f303x8.s.obj"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -o CMakeFiles/NSE2025_VALVE.dir/startup_stm32f303x8.s.obj -c /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/startup_stm32f303x8.s

CMakeFiles/NSE2025_VALVE.dir/startup_stm32f303x8.s.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing ASM source to CMakeFiles/NSE2025_VALVE.dir/startup_stm32f303x8.s.i"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -E /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/startup_stm32f303x8.s > CMakeFiles/NSE2025_VALVE.dir/startup_stm32f303x8.s.i

CMakeFiles/NSE2025_VALVE.dir/startup_stm32f303x8.s.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling ASM source to assembly CMakeFiles/NSE2025_VALVE.dir/startup_stm32f303x8.s.s"
	/opt/ST/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -S /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/startup_stm32f303x8.s -o CMakeFiles/NSE2025_VALVE.dir/startup_stm32f303x8.s.s

# Object files for target NSE2025_VALVE
NSE2025_VALVE_OBJECTS = \
"CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.obj" \
"CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.obj" \
"CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.obj" \
"CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.obj" \
"CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.obj" \
"CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.obj" \
"CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.obj" \
"CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.obj" \
"CMakeFiles/NSE2025_VALVE.dir/startup_stm32f303x8.s.obj"

# External object files for target NSE2025_VALVE
NSE2025_VALVE_EXTERNAL_OBJECTS = \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Core/Src/system_stm32f3xx.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c.obj" \
"/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart_ex.c.obj"

NSE2025_VALVE.elf: CMakeFiles/NSE2025_VALVE.dir/Core/Src/main.c.obj
NSE2025_VALVE.elf: CMakeFiles/NSE2025_VALVE.dir/Core/Src/gpio.c.obj
NSE2025_VALVE.elf: CMakeFiles/NSE2025_VALVE.dir/Core/Src/tim.c.obj
NSE2025_VALVE.elf: CMakeFiles/NSE2025_VALVE.dir/Core/Src/usart.c.obj
NSE2025_VALVE.elf: CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_it.c.obj
NSE2025_VALVE.elf: CMakeFiles/NSE2025_VALVE.dir/Core/Src/stm32f3xx_hal_msp.c.obj
NSE2025_VALVE.elf: CMakeFiles/NSE2025_VALVE.dir/Core/Src/sysmem.c.obj
NSE2025_VALVE.elf: CMakeFiles/NSE2025_VALVE.dir/Core/Src/syscalls.c.obj
NSE2025_VALVE.elf: CMakeFiles/NSE2025_VALVE.dir/startup_stm32f303x8.s.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Core/Src/system_stm32f3xx.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c.obj
NSE2025_VALVE.elf: cmake/stm32cubemx/CMakeFiles/STM32_Drivers.dir/__/__/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart_ex.c.obj
NSE2025_VALVE.elf: CMakeFiles/NSE2025_VALVE.dir/build.make
NSE2025_VALVE.elf: CMakeFiles/NSE2025_VALVE.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking C executable NSE2025_VALVE.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/NSE2025_VALVE.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/NSE2025_VALVE.dir/build: NSE2025_VALVE.elf
.PHONY : CMakeFiles/NSE2025_VALVE.dir/build

CMakeFiles/NSE2025_VALVE.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/NSE2025_VALVE.dir/cmake_clean.cmake
.PHONY : CMakeFiles/NSE2025_VALVE.dir/clean

CMakeFiles/NSE2025_VALVE.dir/depend:
	cd /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build /Users/a81802/Documents/Programs/stm32_projects/1.18/NSE2025_VALVE/build/CMakeFiles/NSE2025_VALVE.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/NSE2025_VALVE.dir/depend

