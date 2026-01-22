LOCAL_PATH := $(call my-dir)

PAHO_SRC := ../src

PAHO_INCLUDES := \
    $(LOCAL_PATH)/$(PAHO_SRC)

PAHO_COMMON_SRCS := \
    $(PAHO_SRC)/MQTTProtocolClient.c \
    $(PAHO_SRC)/Tree.c \
    $(PAHO_SRC)/Heap.c \
    $(PAHO_SRC)/MQTTPacket.c \
    $(PAHO_SRC)/Clients.c \
    $(PAHO_SRC)/Thread.c \
    $(PAHO_SRC)/utf-8.c \
    $(PAHO_SRC)/StackTrace.c \
    $(PAHO_SRC)/MQTTProtocolOut.c \
    $(PAHO_SRC)/Socket.c \
    $(PAHO_SRC)/Log.c \
    $(PAHO_SRC)/Messages.c \
    $(PAHO_SRC)/LinkedList.c \
    $(PAHO_SRC)/MQTTPersistence.c \
    $(PAHO_SRC)/MQTTPacketOut.c \
    $(PAHO_SRC)/SocketBuffer.c \
    $(PAHO_SRC)/MQTTPersistenceDefault.c \
    $(PAHO_SRC)/MQTTTime.c \
    $(PAHO_SRC)/MQTTProperties.c \
    $(PAHO_SRC)/MQTTReasonCodes.c \
    $(PAHO_SRC)/WebSocket.c \
    $(PAHO_SRC)/Proxy.c \
    $(PAHO_SRC)/Base64.c \
    $(PAHO_SRC)/SHA1.c




PAHO_CLIENT_SRC := \
    $(PAHO_SRC)/MQTTClient.c

include $(CLEAR_VARS)

LOCAL_MODULE := libpaho-mqtt3c
LOCAL_SRC_FILES := \
    $(PAHO_COMMON_SRCS) \
    $(PAHO_CLIENT_SRC)

LOCAL_C_INCLUDES := $(PAHO_INCLUDES)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(PAHO_INCLUDES)

include $(BUILD_STATIC_LIBRARY)
