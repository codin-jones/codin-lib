# Makefile

include ./Make.inc


TEST_DATAGUARD_EXEC = test_DataGuard
TEST_DATAGUARD_SRCS = ./test/test_DataGuard.cc


# aggregate macros
LIBS  =
EXECS =
TESTS = $(TEST_DATAGUARD_EXEC)

# include the generic rules
include $(PROJECT_ROOT)/MakeRules.inc

# define targets here
$(foreach exe,$(TEST_DATAGUARD_EXEC),$(eval $(call EXE_template,$(exe),,$(TEST_DATAGUARD_SRCS))))
