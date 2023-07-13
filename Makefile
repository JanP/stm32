APPS:=
LIBS:=
ARCHIVES:=test_template.a test_addr.a

all: $(APPS) $(LIBS) $(ARCHIVES)

include Dependencies/makefile_rules/rules.mk
