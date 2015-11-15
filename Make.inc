#
# Make.inc
#
PROJECT_ROOT = $(CURDIR)

CC        = gcc
CXX       = g++

CPPFLAGS  = -g -Wall -m64

CXXFLAGS  = -std=c++11

GTEST_ROOT = $(PROJECT_ROOT)/gtest-1.7.0
GTEST_INC  = -I$(GTEST_ROOT)/include
GTEST_LIB  = $(GTEST_ROOT)/lib/libgtest.a \
             $(GTEST_ROOT)/lib/libgtest_main.a

# TODO: Move gtest into codin-lib
CPPFLAGS  += -I/opt/local/include \
             -I$(PROJECT_ROOT)/thread \
             $(GTEST_INC)

LDFLAGS   +=  -L. \
              -L/opt/local/lib \
              -L$(PROJECT_ROOT) \
              $(GTEST_LIB) \
              -lpthread

OBJ_DIR    = objs
                 