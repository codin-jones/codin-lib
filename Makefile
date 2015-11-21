# Makefile

include ./Make.inc


TEST_DATAGUARD_EXEC = ./test/test_DataGuard
TEST_DATAGUARD_SRCS = ./test/test_DataGuard.cc

TEST_SCOPEDWITH_EXEC = ./test/test_ScopedWith
TEST_SCOPEDWITH_SRCS = ./test/test_ScopedWith.cc

TEST_CIRCULARQUEUE_EXEC = ./test/test_CircularQueue
TEST_CIRCULARQUEUE_SRCS = ./test/test_CircularQueue.cc

# aggregate macros
LIBS  =
EXECS =
TESTS = $(TEST_DATAGUARD_EXEC)  \
        $(TEST_SCOPEDWITH_EXEC) \
        $(TEST_CIRCULARQUEUE_EXEC)

# include the generic rules
include $(PROJECT_ROOT)/MakeRules.inc

# define targets here

$(foreach exe,$(TEST_DATAGUARD_EXEC),$(eval $(call EXE_template,$(exe),,$(TEST_DATAGUARD_SRCS))))

$(foreach exe,$(TEST_SCOPEDWITH_EXEC),$(eval $(call EXE_template,$(exe),,$(TEST_SCOPEDWITH_SRCS))))

$(foreach exe,$(TEST_CIRCULARQUEUE_EXEC),$(eval $(call EXE_template,$(exe),,$(TEST_CIRCULARQUEUE_SRCS))))


discrete_tests: $(TESTS)
