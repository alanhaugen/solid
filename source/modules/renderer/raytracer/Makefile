#############################################################################
# Library name and path 
#############################################################################

TARGET = librayrenderer
SOLIDCORE = ../../../..

#############################################################################
# Source and header files
#############################################################################

SOURCES = \
        raytacerrenderer.cpp \
        raytacerdrawable.cpp \
        raytacershader.cpp

HEADERS = \
        raytacerrenderer.h \
        raytacerdrawable.h \
        raytacershader.h

#############################################################################
# Setup build platform rules 
#############################################################################

include $(SOLIDCORE)/Makefile.rules

#############################################################################
# Build as library
#############################################################################

debug: $(SOLIDCORE)/lib/$(OUTPUT_DIR)/$(TARGET).a
release: $(SOLIDCORE)/lib/$(OUTPUT_DIR)/$(TARGET).a
