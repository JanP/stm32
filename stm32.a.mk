STM32_A:=$(notdir stm32.a)

$(STM32_A)_CXXSRCS:=test.cpp
$(STM32_A)_CXXSRCS+=test_private_addr.cpp
