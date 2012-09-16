#include "JNIHelper.h"
#include <Utils/Log.h>

JavaVM* JNIHelper::javaVM = 0;
JNIEnv* JNIHelper::javaEnv = 0;

void JNIHelper::setJavaVM(JavaVM* vm) {
	javaVM = vm;
}

void JNIHelper::setJavaEnv(JNIEnv* env) {
	javaEnv = env;
}

JavaVM* JNIHelper::getJavaVM() {
	return javaVM;
}

JNIEnv* JNIHelper::getJavaEnv() {
	return javaEnv;
}

bool JNIHelper::getStaticMethodInfo(JNIMethod& methodInfo, const char *className, const char *methodName, const char *paramCode) {
	jmethodID methodID = 0;
	bool result = false;

	if(javaEnv) {
		jclass classID = javaEnv->FindClass(className);
		if(classID!=0) {
			methodID = javaEnv->GetStaticMethodID(classID, methodName, paramCode);
			if(methodID) {
				methodInfo.classID = classID;
				methodInfo.env = javaEnv;
				methodInfo.methodID = methodID;
				result = true;
			}
			else {
                                Logger::Log(1, "cannot find method %s", methodName);
			}
		}
		else {
                        Logger::Log(1, "cannot find class %s", className);
		}

	}
	else {
                Logger::Log(1, "No valid java environment");
	}

	return result;
}
