PATH_JNI := $(call my-dir)

include $(PATH_JNI)/../../../external/SDL/Android.mk

include $(PATH_JNI)/../../../external/SDL_ttf/Android.mk

include $(PATH_JNI)/../../../external/paho.mqtt.c/android/Android.mk

include $(PATH_JNI)/../../../external/BinkaEngine/Android.mk

include $(PATH_JNI)/src/Android.mk
