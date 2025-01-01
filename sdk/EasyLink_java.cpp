//
// Created by nightrider on 31.12.2024.
//

#include "EasyLink.h"
#include "EasyLink_java.h"
#include <assert.h>

jstring JNICALL Java_com_chessnut_EasyLink_version
  (JNIEnv *env, jclass)
{
    return env->NewStringUTF(ChessLink::CL_VERSION.c_str());
}

jint JNICALL Java_com_chessnut_EasyLink_connect (JNIEnv *, jclass)
{
  return ChessLink::instance()->connect();
}

void JNICALL Java_com_chessnut_EasyLink_disconnect (JNIEnv *, jclass)
{
  ChessLink::instance()->disconnect();
}

jint JNICALL Java_com_chessnut_EasyLink_switchRealTimeMode (JNIEnv *, jclass)
{
  return ChessLink::instance()->switchRealTimeMode();
}

jint JNICALL Java_com_chessnut_EasyLink_switchUploadMode (JNIEnv *, jclass)
{
  return ChessLink::instance()->switchUploadMode();
}

void JNICALL Java_com_chessnut_EasyLink_cl_1set_1realtime_1callback (JNIEnv *env, jclass, jobject callback)
{
  static JNIEnv* jniEnv = env;
  static jclass callBackInterface = env->FindClass("com/chessnut/EasyLink$IRealTimeCallback");
  assert(callBackInterface);
  static jmethodID callBackMethod = env->GetMethodID(callBackInterface, "realTimeCallback", "([B)V");
  assert(callBackMethod);
  static jobject callbackObject = callback;
  assert(callbackObject);

  ChessLink::instance()->setRealTimeCallback([](string fen) {
    jstring arg = jniEnv->NewStringUTF(fen.c_str());
    jniEnv->CallVoidMethod(callbackObject, callBackMethod, arg);
  });
}

jint JNICALL Java_com_chessnut_EasyLink_cl_beep (JNIEnv *, jclass, jint frequency, jint ms)
{
  return ChessLink::instance()->beep(frequency, ms);
}

jint JNICALL Java_com_chessnut_EasyLink_led
  (JNIEnv *env, jclass, jstring map)
{
    const char* chars = env->GetStringUTFChars(map,NULL);
    bool result = ChessLink::instance()->setLed(chars);
    env->ReleaseStringUTFChars(map,chars);
    return result;
}

jstring JNICALL Java_com_chessnut_EasyLink_getMcuVersion (JNIEnv *env, jclass)
{
  const char* ver = ChessLink::instance()->getMcuVersion().c_str();
  return env->NewStringUTF(ver);
}


jstring JNICALL Java_com_chessnut_EasyLink_getBleVersion (JNIEnv *env, jclass)
{
  const char* ver = ChessLink::instance()->getBleVersion().c_str();
  return env->NewStringUTF(ver);
}

jint JNICALL Java_com_chessnut_EasyLink_getBattery (JNIEnv *, jclass)
{
  return ChessLink::instance()->getBattery();
}

jint JNICALL Java_com_chessnut_EasyLink_getFileCount
  (JNIEnv *, jclass)
{
  return ChessLink::instance()->getFileCount();
}

jobjectArray JNICALL Java_com_chessnut_EasyLink_getFile
  (JNIEnv *env, jclass, jboolean andDelete)
{
  static jclass stringClass = env->FindClass("java/lang/String");
  assert(stringClass);
  const vector<string> file = ChessLink::instance()->getFile(andDelete);
  jobjectArray array = env->NewObjectArray(file.size(),stringClass,NULL);
  for (int i = 0; i < file.size(); i++)
    env->SetObjectArrayElement(array,i,env->NewStringUTF(file[i].c_str()));
  return array;
}
