//
//  JNIHelper.h
//  Engine
//
//  Created by Mateusz P—¸rola on 07.08.2012.
//  Copyright (c) 2012 Vivid Games. All rights reserved.
//
#ifndef JNIHELPER_H
#define JNIHELPER_H
#include <jni.h>
#include <string>


struct JNIMethod {
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
};

class JNIHelper {
public:
	static void setJavaVM(JavaVM* vm);
	static void setJavaEnv(JNIEnv* env);

	static JavaVM* getJavaVM();
	static JNIEnv* getJavaEnv();

	static bool getStaticMethodInfo(JNIMethod &methodInfo, const char* className, const char* methodName, const char* paramCode);

private:
	static JavaVM* javaVM;
	static JNIEnv* javaEnv;
};


#endif
