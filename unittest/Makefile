#############################################################################
# Executable name
#############################################################################

TARGET = unit

#############################################################################
# Setup custom modules
#############################################################################

#############################################################################
# Libraries 
#############################################################################

LIBS = \
    -lcore

#############################################################################
# Source and header files
#############################################################################

SOURCES = test_string.cpp

#HEADERS = 

#############################################################################
# Setup build platform rules 
#############################################################################

SOLIDCORE = ../

include $(SOLIDCORE)/Makefile.rules

parent:
	$(MAKE) -C ..

all: parent

#############################################################################
# Build as executable
#############################################################################

debug: executable
#release: executable # dont care about release builds of unit-tests
