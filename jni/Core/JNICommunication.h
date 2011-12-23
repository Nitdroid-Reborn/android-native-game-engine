#ifdef ANDROID
#ifndef JNICOMMUNICATION_H
#define JNICOMMUNICATION_H
#include <android_native_app_glue.h>

class JNICommunication
{
public:
    JNICommunication(android_app* app) {this->app=app;}

    static void SendResult(float distance);
    static void Vibrate(unsigned int miliseconds);

private:
    static android_app* app;
};

#endif // JNICOMMUNICATION_H
#endif
