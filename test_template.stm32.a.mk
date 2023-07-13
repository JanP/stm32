TEST_TEMPLATE_STM32:=$(notdir test_template.stm32.a)

$(TEST_TEMPLATE_STM32)_CC:=$(ARM_TOOLCHAIN)/bin/arm-none-eabi-gcc
$(TEST_TEMPLATE_STM32)_CXX:=$(ARM_TOOLCHAIN)/bin/arm-none-eabi-g++
$(TEST_TEMPLATE_STM32)_AR:=$(ARM_TOOLCHAIN)/bin/arm-none-eabi-ar

$(TEST_TEMPLATE_STM32)_CXXSRCS:=test_template.cpp

$(TEST_TEMPLATE_STM32)_CFLAGS:=-mcpu=cortex-m0
$(TEST_TEMPLATE_STM32)_CXXFLAGS:=-mcpu=cortex-m0
