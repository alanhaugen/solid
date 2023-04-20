#############################################################################
# Library name and path
#############################################################################

TARGET = libcore
SOLIDCORE = .

#############################################################################
# Setup custom modules
#############################################################################

#RENDERER        = RaytracerRenderer
#RENDERER_HEADER = "../../hw3-CSE167x/module/raytracerrenderer.h"

#############################################################################
# Source files
#############################################################################

SOURCES = \
    $(PLATFORM_SOURCES) \
    source/core/components/actor.cpp \
    source/core/components/component.cpp \
    source/core/components/fpscounter.cpp \
    source/core/components/background.cpp \
    source/core/components/command.cpp \
    source/core/components/cell.cpp \
    source/core/components/cube.cpp \
    source/core/components/grid.cpp \
    source/core/components/snowglobecamera.cpp \
    source/core/components/fpscamera.cpp \
    source/core/components/arealight.cpp \
    source/core/components/directionallight.cpp \
    source/core/components/pointlight.cpp \
    source/core/components/spotlight.cpp \
    source/core/components/camera.cpp \
    source/core/components/text.cpp \
    source/core/components/mesh.cpp \
    source/core/components/sprite.cpp \
    source/core/components/sphere.cpp \
    source/core/components/triangle.cpp \
    source/core/components/rectangle.cpp \
    source/core/components/circle.cpp \
    source/core/components/line.cpp \
    source/core/components/collisionbox.cpp \
    source/core/components/sound.cpp \
    source/core/components/videocapture.cpp \
    source/core/components/tankcontrols.cpp \
    source/core/components/laddercontrols.cpp \
    source/core/components/skipcutscene.cpp \
    source/core/x-platform/mat.cpp \
    source/core/x-platform/uniform.cpp \
    source/core/x-platform/generatedscene.cpp \
    source/core/x-platform/parserjson.cpp \
    source/core/x-platform/parserjsmn.cpp \
    source/core/x-platform/input.cpp \
    source/core/x-platform/locator.cpp \
    source/core/x-platform/services.cpp \
    source/core/x-platform/splash.cpp \
    source/core/x-platform/memory.cpp \
    source/core/x-platform/pixmap.cpp \
    source/core/x-platform/random.cpp \
    source/core/x-platform/string.cpp \
    source/core/x-platform/url.cpp \
    source/core/x-platform/keyframe.cpp \
    source/core/x-platform/animation.cpp \
    source/core/x-platform/joint.cpp \
    source/core/application.cpp

#############################################################################
# Setup build platform rules
#############################################################################

include $(SOLIDCORE)/Makefile.rules

#############################################################################
# Build as library
#############################################################################

debug: lib/$(OUTPUT_DIR)/$(TARGET).$(LIBRARY_EXTENSION)
release: lib/$(OUTPUT_DIR)/$(TARGET).$(LIBRARY_EXTENSION)
