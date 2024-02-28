cmake_minimum_required(VERSION 3.15)

project(STM32H757_OS_CM7 LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.arm.stm32
	VERSION 2024.01
	MCU STM32H757XI
	CONFIGURATION com.sysprogs.toolchainoptions.arm.syscallspecs=--specs=rdimon.specs
	HWREGISTER_LIST_FILE STM32H7xxxx/DeviceDefinitions/stm32h757xx.xml
	CXX_STANDARD 17
	DISABLE_GNU_EXTENSIONS)

add_bsp_based_executable(
	NAME STM32H757_OS_CM7
	GENERATE_BIN
	GENERATE_MAP
	LINKER_SCRIPT STM32H757XI_flash.lds
	SOURCES
		../../osSTM32H7xx/SystemInitCM7.cpp
		Main.cpp
		../SharedData/SharedData.cpp
		../SharedData/SharedData.h
		../../osSTM32H7xx/HSEM.h
		../../osSTM32H7xx/MemoryDefs.h
		../../osSTM32H7xx/PWR.h
		../../osSTM32H7xx/SystemInit.h
		../../osSTM32H7xx/HSEM.cpp
		../../osSTM32H7xx/StartupSemafor.cpp
		../../osSTM32H7xx/StartupSemafor.h)

target_include_directories(STM32H757_OS_CM7 PRIVATE ../../HeatCentral/HeatPump/riverdi-70-stm32h7-lvgl-main/Drivers/CMSIS/Device/ST/STM32H7xx/Include ../../osSTM32H7xx ../../CCR/Test2/BSP/CMSIS/Core/Include ..)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
target_compile_definitions(STM32H757_OS_CM7 PRIVATE STM32H757xx DEBUG DEBUG_DEFAULT_INTERRUPT_HANDLERS)
    message(STATUS "Setting debug flags...")
	target_compile_options(STM32H757_OS_CM7 PRIVATE -O0 -Wall -Wextra)
	message(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS_DEBUG})
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
target_compile_definitions(STM32H757_OS_CM7 PRIVATE STM32H757xx RELEASE)
    message(STATUS "Setting release flags...")
	target_compile_options(STM32H757_OS_CM7 PRIVATE -O3)
	message(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS_RELEASE})
endif()

