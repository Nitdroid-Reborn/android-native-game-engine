LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_C_INCLUDES := Scripts/lua/ Scripts/
LOCAL_MODULE    := android-game-engine
LOCAL_CPPFLAGS += -fexceptions
LOCAL_SRC_FILES := main.cpp \
                   Core/AndroidEngine.cpp \
                   Utils/Utils.cpp \
                   ContentManager/PNGLoader.cpp \
                   FileIO/AndroidFileIO.cpp \
                   FileIO/BufferStream.cpp \
                   Utils/Clock.cpp \
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
                   VirtualInput/VirtualInput.cpp \
                   VirtualInput/VirtualSingleKey.cpp \
                   VirtualInput/VirtualDPad.cpp \
                   Graphics/OGLTexture.cpp \
                   Graphics/TextureRegion.cpp \
                   Graphics/SpriteBatcher.cpp \
                   MemoryManagement/StackAllocator.cpp \
                   Core/Thread.cpp \
                   Core/Mutex.cpp \
                   Core/ConditionalVariable.cpp \
                   Graphics/AndroidRenderer.cpp \
                   ContentManager/AndroidContentManager.cpp \
                   Utils/Log.cpp \
                   Utils/Assert.cpp \
                   Core/Object.cpp \
                   ContentManager/Handle.cpp \
                   ContentManager/HandleManager.cpp \
                   ContentManager/TextureManager.cpp \
                   ContentManager/SoundManager.cpp\
                   Audio/SoundFactory.cpp \
                   Audio/Sound.cpp \
                   Graphics/OGLTextureFactory.cpp \
                   Graphics/TextBox.cpp \
                   Utils/Profiler.cpp \
                   Utils/TextBuffer.cpp \
                   Utils/RelativeClock.cpp \
                   Audio/AndroidAudioSystem.cpp \
                   Audio/WaveSound.cpp \
                   Scripts/lua/print.c \
                   Scripts/lua/lzio.c \
                   Scripts/lua/lvm.c \
                   Scripts/lua/lundump.c \
                   Scripts/lua/ltm.c \
                   Scripts/lua/ltablib.c \
                   Scripts/lua/ltable.c \
                   Scripts/lua/lstrlib.c \
                   Scripts/lua/lstring.c \
                   Scripts/lua/lstate.c \
                   Scripts/lua/lparser.c \
                   Scripts/lua/loslib.c \
                   Scripts/lua/lopcodes.c \
                   Scripts/lua/lobject.c \
                   Scripts/lua/loadlib.c \
                   Scripts/lua/lmem.c \
                   Scripts/lua/lmathlib.c \
                   Scripts/lua/llex.c \
                   Scripts/lua/liolib.c \
                   Scripts/lua/linit.c \
                   Scripts/lua/lgc.c \
                   Scripts/lua/lfunc.c \
                   Scripts/lua/ldump.c \
                   Scripts/lua/ldo.c \
                   Scripts/lua/ldebug.c \
                   Scripts/lua/ldblib.c \
                   Scripts/lua/lcode.c \
                   Scripts/lua/lbaselib.c \
                   Scripts/lua/lauxlib.c \
                   Scripts/lua/lapi.c \
                   Scripts/luabind/src/wrapper_base.cpp \
                   Scripts/luabind/src/weak_ref.cpp \
                   Scripts/luabind/src/stack_content_by_name.cpp \
                   Scripts/luabind/src/scope.cpp \
                   Scripts/luabind/src/pcall.cpp \
                   Scripts/luabind/src/open.cpp \
                   Scripts/luabind/src/object_rep.cpp \
                   Scripts/luabind/src/link_compatibility.cpp \
                   Scripts/luabind/src/inheritance.cpp \
                   Scripts/luabind/src/function.cpp \
                   Scripts/luabind/src/exception_handler.cpp \
                   Scripts/luabind/src/error.cpp \
                   Scripts/luabind/src/create_class.cpp \
                   Scripts/luabind/src/class_rep.cpp \
                   Scripts/luabind/src/class_registry.cpp \
                   Scripts/luabind/src/class_info.cpp \
                   Scripts/luabind/src/class.cpp \
                   Scripts/ScriptManager.cpp \
                   Scripts/Script.cpp \
                   Scripts/ScriptSource.cpp \
                   Scripts/ScriptSourceFactory.cpp \
                   ContentManager/ScriptSourceManager.cpp \
                   Graphics/Font.cpp \
                   Graphics/Shader.cpp \
                   Graphics/ShaderProgram.cpp \
                   Graphics/VBO.cpp \
                   Graphics/ModelGeometry.cpp \
                   Graphics/MS3DModel.cpp \
                   Graphics/Camera.cpp \
                   ContentManager/ShaderManager.cpp \
                   ContentManager/ShaderProgramManager.cpp \
                   ContentManager/ModelGeometryManager.cpp \
                   Input/AccelerometerState.cpp \
                   Graphics/ShaderParameter.cpp \
                   GameObject/GameObjectManager.cpp \
                   GameObject/RenderableGameObject.cpp \
                   GameObject/PhysicalGameObject.cpp \
                   Physics/Collider.cpp \
                   Utils/Hash.cpp \
                   Core/JNICommunication.cpp


LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2 -lOpenSLES -lz
LOCAL_SHARED_LIBRARIES :=  libpng box2d
LOCAL_STATIC_LIBRARIES := android_native_app_glue boost_thread freetype2

include $(BUILD_SHARED_LIBRARY)

$(call import-module, boost)
$(call import-module, freetype2)
$(call import-module, libpng)
$(call import-module, box2d)
$(call import-module, android/native_app_glue)
