#############################################################################
# Library name and path to SolidCore
#############################################################################

TARGET = libvulkanrenderer
SOLIDCORE = ../../../..

#############################################################################
# Source files
#############################################################################

SOURCES = \
        vulkanrenderer.cpp \
        vulkandrawable.cpp \
        vulkantexture.cpp \
        vulkanshader.cpp

#############################################################################
# Setup build platform rules 
#############################################################################

include $(SOLIDCORE)/Makefile.rules

#############################################################################
# Build as library
#############################################################################

debug: $(SOLIDCORE)/lib/$(OUTPUT_DIR)/$(TARGET).$(LIBRARY_EXTENSION)
release: $(SOLIDCORE)/lib/$(OUTPUT_DIR)/$(TARGET).$(LIBRARY_EXTENSION)
