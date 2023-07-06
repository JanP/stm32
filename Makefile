APPS:=
LIBS:=
ARCHIVES:=stm32.a

all: $(APPS) $(LIBS) $(ARCHIVES)

include Dependencies/makefile_rules/rules.mk
