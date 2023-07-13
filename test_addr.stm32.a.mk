TEST_ADDR_STM32:=$(notdir test_addr.stm32.a)

$(TEST_ADDR_STM32)_CC:=$(ARM_TOOLCHAIN)/bin/arm-none-eabi-gcc
$(TEST_ADDR_STM32)_CXX:=$(ARM_TOOLCHAIN)/bin/arm-none-eabi-g++
$(TEST_ADDR_STM32)_AR:=$(ARM_TOOLCHAIN)/bin/arm-none-eabi-ar

$(TEST_ADDR_STM32)_CXXSRCS:=test_addr.cpp

$(TEST_ADDR_STM32)_CFLAGS:=-mcpu=cortex-m0
$(TEST_ADDR_STM32)_CXXFLAGS:=-mcpu=cortex-m0
