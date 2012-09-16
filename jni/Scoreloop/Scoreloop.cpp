#include "Scoreloop.h"
#include "JNIHelper.h"

namespace Scoreloop {

	void Client::initialize(const char *gameId, const char *gameSecret, TOSCallback tosCallback) {
		JNIMethod method;
		if(JNIHelper::getStaticMethodInfo(method, "com.vividgames.engine.Scoreloop.Scoreloop", "clientInitialize", "(J)V")) {
			jlong arg = (long)(void*)tosCallback;
			method.env->CallStaticVoidMethod(method.classID, method.methodID, arg);
			method.env->DeleteLocalRef(method.classID);
		}
	}

	void Client::shutdown() {

	}

	bool Client::isInitialized() {
		JNIMethod method;
		if(JNIHelper::getStaticMethodInfo(method, "com.vividgames.engine.Scoreloop.Scoreloop", "clientIsInitialized", "()Z")) {
			jboolean ret = method.env->CallStaticBooleanMethod(method.classID, method.methodID);
			method.env->DeleteLocalRef(method.classID);
			return (ret == JNI_TRUE);
		}

		return false;
	}
}


extern "C"
{
	void Java_com_vividgames_engine_Scoreloop_Scoreloop_tosCallback(JNIEnv* env, jobject thiz, jlong callbackAdress)
	{
		if (callbackAdress)
			(*reinterpret_cast<Scoreloop::TOSCallback>(callbackAdress))();
	}

	void Java_com_vividgames_engine_Scoreloop_Scoreloop_requestCallback(JNIEnv* env, jobject thiz, jobject controller, jint errorCode, jlong callbackAdress)
	{
		if (callbackAdress)
			(*reinterpret_cast<Scoreloop::RequestCallback>(callbackAdress))(errorCode);
	}
}
