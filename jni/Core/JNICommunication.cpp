#ifdef ANDROID
#include "JNICommunication.h"
#include <jni.h>

android_app* JNICommunication::app=0;

void JNICommunication::SendResult(float distance) {
    JavaVM* lJavaVM = app->activity->vm;
    JNIEnv* lJNIEnv = app->activity->env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = NULL;

    jint lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
    if (lResult == JNI_ERR) {

    }

    jobject lNativeActivity = app->activity->clazz;
    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

    jmethodID method = lJNIEnv->GetMethodID(ClassNativeActivity, "SendToServer", "(I)V");
    lJNIEnv->CallVoidMethod(lNativeActivity, method, (int)(distance*10));

    lJavaVM->DetachCurrentThread();
}

#endif
