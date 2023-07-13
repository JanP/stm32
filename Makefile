ARM_TOOLCHAIN:=Dependencies/ARM-toolchains/arm-none-eabi

APPS:=
LIBS:=
ARCHIVES:=test_template.a test_addr.a test_template.stm32.a test_addr.stm32.a

all: $(APPS) $(LIBS) $(ARCHIVES)

include Dependencies/makefile_rules/rules.mk
