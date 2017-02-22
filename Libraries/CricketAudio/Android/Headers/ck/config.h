// Copyright 2016 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkConfig CkConfig */
/** @{ */

#pragma once
#include "ck/platform.h"

#if CK_PLATFORM_ANDROID
#  include <jni.h>
#endif

////////////////////////////////////////

/** Memory allocation function */
typedef void* (*CkAllocFunc)(int bytes);
/** Memory deallocation function */
typedef void (*CkFreeFunc)(void*);


/** Log message types */
typedef enum 
{
    /** Flag to enable INFO log messages */
    kCkLog_Info =  (1 << 0),

    /** Flag to enable WARNING log messages */
    kCkLog_Warning = (1 << 1),

    /** Flag to enable ERROR log messages */
    kCkLog_Error = (1 << 2),

    /** Flag to disable all log messages */
    kCkLog_None = 0,

    /** Flag to enable all log messages */
    kCkLog_All = (kCkLog_Info | kCkLog_Warning | kCkLog_Error)

} CkLogType;


/** Log message function */
typedef void (*CkLogFunc)(CkLogType, const char* msg);


/** Sample types used for internal DSP pipeline */
typedef enum
{
    /** Force floating-point samples */
    kCkSampleType_Float,

    /** Force fixed-point samples */
    kCkSampleType_Fixed,
    
    /** Use the default, based on the processor type */
    kCkSampleType_Default

} CkSampleType;


////////////////////////////////////////

/** Configuration options */
struct _CkConfig
{
#ifdef __cplusplus
#  if CK_PLATFORM_ANDROID
    /** Constructor to be used from JNI functions.
      @param env      A JNI environment pointer
      @param context  An android.content.Context object from Java (typically an
                       android.app.Activity or android.app.Service)
     */
    _CkConfig(JNIEnv* env, jobject context);

    /** This constructor should be used when there is no valid JNIEnv* pointer; for
      example, from android_main() in a native activity using the android_native_app_glue
      library.  See the code in src/samples/hellocricket/android-ndk-native for an example.
      @param vm       A Java VM pointer
      @param context  An android.content.Context object from Java (typically an
                       android.app.Activity or android.app.Service)
     */
    _CkConfig(JavaVM* vm, jobject context);
#  else
    _CkConfig();
#  endif
#endif // __cplusplus

#if CK_PLATFORM_ANDROID
    /** The JNIEnv pointer.
      @par Only available on Android. */
    JNIEnv* jni;

    /** The JavaVM pointer.
      @par Only available on Android. */
    JavaVM* vm;

    /** A Java reference to your app's Context object (typically an Activity or Service).
      @par Only available on Android. */
    jobject context;

    /** If true, always use Java AudioTrack for output of final mix, instead of OpenSL ES.
      This should usually be set to false, unless you are debugging specific issues
      related to low-level audio output.
      Default is false (use Java AudioTrack only when OpenSL ES is not available, i.e. Android versions prior to 2.3). 
      @par Only available on Android. */
    bool useJavaAudio;

    /** If true, software support for Ogg Vorbis stream sounds is enabled.
      If false, stream sounds will use the native Android decoder, which will not
      support all of the features available to sounds decoded in software.
      Default is true (Ogg Vorbis streams are decoded in software).
      @par Only available on Android. */
    bool enableOggVorbis;
#endif

#if CK_PLATFORM_IOS || CK_PLATFORM_TVOS
    /** If true, hardware-assisted decoding is enabled for decoding compressed streams
      when available.  See Apple's "Multimedia Programming Guide" for a list of codecs
      for which hardware support is available.  Note that streams that use the hardware
      decoder take slightly longer (~300ms) to initialize, and our tests have shown
      a negligible effect on CPU usage, so the default value is false.
      @par Only available on iOS. */
    bool enableHardwareDecoding;
#endif

    /** Memory allocation function.
      Default is NULL (system new will be used). 
      The allocator must be thread-safe. */
    CkAllocFunc allocFunc;

    /** Memory deallocation function.
      Default is NULL (system delete will be used). 
      The allocator must be thread-safe. */
    CkFreeFunc freeFunc;


    /** Bitmask for which types of messages are logged. 
      Default is kCkLog_All (all messages logged). */
    unsigned int logMask;

    /** Function to handle log messages.
      Default is NULL (printf or equivalent will be used). */
    CkLogFunc logFunc;


    /** Audio processing frame duration, in ms.
      Shorter times result in lower latency, but require more CPU.
      Default is 5 ms. */
    float audioUpdateMs;

    /** Size of buffer for stream sounds, in ms.
      Larger buffers require more memory, but are less likely
      to skip due to slow file reads.
      Default is 500 ms. */
    float streamBufferMs;

    /** Interval between file reads for stream sounds, in ms.
      Shorter times will allow for smaller stream buffer sizes, but require more CPU.
      This value should be set to significantly less than streamBufferMs, or your buffers will always underrun!
      Default is 100 ms. */
    float streamFileUpdateMs;

    /** Initial size of the audio task buffer.  If you see a "Reallocating
      audio task buffer" warning, consider increasing this from its default value
      of 500 to avoid additional memory allocations at runtime. */
    int maxAudioTasks;

    /** Maximum render load. When the value of CkGetRenderLoad() exceeds this value,
      some sounds will not be rendered, to keep the CPU usage down.
      Default is 0.8. */
    float maxRenderLoad;

    /** Sample type to be used internally for processing.  The default is 
      kCkSampleType_Default, which means that fixed-point samples are used on Android 
      armv5 and armv6 devices, which may lack hardware support for floating-point 
      operations, and that floating-point samples are used on all other devices. */
    CkSampleType sampleType;
};

typedef struct _CkConfig CkConfig;

/** Default value of CkConfig.audioUpdateMs */
extern const float CkConfig_audioUpdateMsDefault;

/** Default value of CkConfig.streamBufferMs */
extern const float CkConfig_streamBufferMsDefault;

/** Default value of CkConfig.streamFileUpdateMs */
extern const float CkConfig_streamFileUpdateMsDefault;

/** Default value of CkConfig.maxAudioTasks */
extern const int CkConfig_maxAudioTasksDefault;

/** Default value of CkConfig.maxRenderLoad */
extern const float CkConfig_maxRenderLoadDefault;

////////////////////////////////////////

#ifdef __OBJC__

/** If you are using Objective-C, call CkConfigInit() to initialize the CkConfig
  struct to its default values.
  (If you are using C++, the CkConfig struct is initialized by its constructor.) */

#ifdef __cplusplus
extern "C"
#endif
void CkConfigInit(CkConfig*);

#endif

/** @} */
