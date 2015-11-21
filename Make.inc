#
# Make.inc
#
PROJECT_ROOT = $(CURDIR)

OS_SYSTEM_NAME = $(shell uname -s)
OS_SYSTEM_HWNM = $(shell uname -m)
OS_SYSTEM_FULL = $(OS_SYSTEM_NAME)_$(OS_SYSTEM_HWNM)

# Assume we are using gcc/g++
CC        = gcc
CXX       = g++

# Define MAC_USE_GCC use gcc on the Mac, otherwise it will use clang
# If you use clang then you will need alternate versions of the gtest libraries
MAC_USE_GCC = 1

# If the OS is Darwin (aka Mac) assume they want clang/clang++ unless they have
# defined MAC_USE_GCC, which will revert back to the assumed gcc/g++ 
ifeq ("$(OS_SYSTEM_NAME)","Darwin")
ifndef MAC_USE_GCC
CC        = clang
CXX       = clang++
endif
endif


CPPFLAGS  = -g -Wall -m64

CXXFLAGS  = -std=c++11

DEP_ROOT   = $(PROJECT_ROOT)/dependencies

GTEST_ROOT = $(DEP_ROOT)/$(OS_SYSTEM_FULL)/gtest-1.7.0
GTEST_INC  = -I$(GTEST_ROOT)/include
GTEST_LIB  = $(GTEST_ROOT)/lib/libgtest.a \
             $(GTEST_ROOT)/lib/libgtest_main.a

CPPFLAGS  += -I/opt/local/include \
             -I$(PROJECT_ROOT)/thread \
             -I$(PROJECT_ROOT)/misc \
             -I$(PROJECT_ROOT)/container \
             $(GTEST_INC)

LDFLAGS   +=  -L. \
              -L/opt/local/lib \
              -L$(PROJECT_ROOT) \
              $(GTEST_LIB) \
              -lpthread

OBJ_DIR    = objs
