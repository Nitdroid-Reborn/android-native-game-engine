# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE            := libpng
LOCAL_LDLIBS            := -lz
LOCAL_SRC_FILES         := libpng/libpng.so
include $(PREBUILT_SHARED_LIBRARY)



include $(CLEAR_VARS)
LOCAL_MODULE            := libbox2d
LOCAL_SRC_FILES         := Box2D/libbox2d.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := android-game-engine
LOCAL_SRC_FILES := main.cpp \
                   AndroidEngine.cpp \
                   Utils.cpp \
                   AudioSystem.cpp \
                   ContentManager/PNGLoader.cpp \
                   FileIO/AndroidFileIO.cpp \
                   FileIO/BufferStream.cpp \
                   Clock.cpp \
                   Input/Input.cpp \
                   Input/KeysState.cpp \
                   Input/TouchState.cpp \
                   Math/BoundingBox.cpp \
                   Math/Frustum.cpp \
                   Math/Matrix4x4.cpp \
                   Math/Plane.cpp \
                   Math/Ray.cpp \
                   Math/Vector2.cpp \
                   Math/Vector3.cpp \
                   Math/Vector4.cpp \
                   MyModel.cpp \
                   VirtualInput/VirtualInput.cpp \
                   VirtualInput/VirtualKey.cpp \
                   VirtualInput/VirtualSingleKey.cpp \
                   VirtualInput/VirtualDPad.cpp \
                   Graphics/Texture.cpp \
                   Graphics/TextureRegion.cpp \
                   Graphics/SpriteBatcher.cpp \
                   MemoryManagement/StackAllocator.cpp \
                   Thread.cpp \
                   Mutex.cpp \
                   ConditionalVariable.cpp \
                   AndroidRenderer.cpp


LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv1_CM -lOpenSLES -lz
LOCAL_SHARED_LIBRARIES :=  libpng libbox2d
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
