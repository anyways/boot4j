#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <io.h>

#include "jni.h"

typedef jint(JNICALL *JNI_CreateJavaVM_Typedef)(JavaVM **pvm, void **penv, void *args);

int main(int argc, char *argv[]) {
   char workHome[MAX_PATH];
   GetCurrentDirectory(MAX_PATH, workHome);
   char dll[MAX_PATH];
   sprintf(dll, "%s\\jre\\bin\\server\\jvm.dll", workHome);
   if (access(dll, W_OK) == -1) {
      char *javaHome = getenv("JAVA_HOME");
      sprintf(dll, "%s\\jre\\bin\\server\\jvm.dll", javaHome);
   }
   if (access(dll, W_OK) == -1) {
      return JNI_ERR;
   }
   HINSTANCE hInstance;
   hInstance = LoadLibrary(dll);
   if (hInstance == NULL) {
      return JNI_ERR;
   } else {
      JNI_CreateJavaVM_Typedef CreateJavaVM = (JNI_CreateJavaVM_Typedef)GetProcAddress(hInstance, "JNI_CreateJavaVM");
      JavaVMOption options[1];
      options[0].optionString = "-Djava.class.path=.;boot4j.jar";
      JavaVMInitArgs args;
      args.version = JNI_VERSION_1_8;
      args.nOptions = 1;
      args.options = options;
      args.ignoreUnrecognized = JNI_TRUE;
      JNIEnv *env;
      JavaVM *jvm;
      CreateJavaVM(&jvm, (void **)&env, &args);
      jthrowable jth;
      jclass Launcher = (*env)->FindClass(env, "org/codehaus/plexus/classworlds/launcher/Launcher");
      jth = (*env)->ExceptionOccurred(env);
      if (jth != NULL) {
         (*env)->ExceptionDescribe(env);
      }
      if (Launcher == NULL) {
         return JNI_ERR;
      }
      jmethodID static_void_main = (*env)->GetStaticMethodID(env, Launcher, "main", "([Ljava/lang/String;)V");
      jth = (*env)->ExceptionOccurred(env);
      if (jth != NULL) {
         (*env)->ExceptionDescribe(env);
      }
      if (static_void_main == NULL) {
         return JNI_ERR;
      }
      printf("\r");
      jobjectArray main_args = (*env)->NewObjectArray(env, 0, (*env)->FindClass(env, "java/lang/String"), NULL);
      (*env)->CallStaticVoidMethod(env, Launcher, static_void_main, main_args);
      (*jvm)->DestroyJavaVM(jvm);
      FreeLibrary(hInstance);
   }
   return JNI_OK;
}
