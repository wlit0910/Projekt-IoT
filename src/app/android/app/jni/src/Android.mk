LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := $(LOCAL_PATH)/../../../../external/SDL
SDL_TTF_PATH := $(LOCAL_PATH)/../../../../external/SDL_ttf
BINKAENGINE_PATH := $(LOCAL_PATH)/../../../../external/BinkaEngine
SRC_PATH := $(LOCAL_PATH)/../../../../src

LOCAL_C_INCLUDES := $(SDL_PATH)/include \
                    $(SDL_TTF_PATH) \
                    $(BINKAENGINE_PATH)/include

# Add your application source files here...
LOCAL_SRC_FILES := $(SRC_PATH)/main.cpp

LOCAL_STATIC_LIBRARIES := binkaui
LOCAL_SHARED_LIBRARIES := SDL2 SDL2_ttf binkaengine

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid -lstdc++

include $(BUILD_SHARED_LIBRARY)

