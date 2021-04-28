
#ifndef _JAVA_FRAG_ENGINE_LIB_JNI_H_
#define _JAVA_FRAG_ENGINE_LIB_JNI_H_ 1
#include <android/log.h>
#include <jni.h>


extern "C"{
    JNIEXPORT void JNICALL Java_org_fragcore_frag_FragLib_initilize(JNIEnv* env, jclass clazz, jobject activity);
}

#endif
