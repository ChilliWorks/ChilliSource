// Copyright 2011 Cricket Technology
// www.crickettechnology.com
/** @file */
/** @defgroup CkPathType CkPathType */
/** @{ */

#pragma once
#include "ck/platform.h"


/** Indicates how a file path should be interpreted. */
typedef enum 
{
    /** A full path to a file on the file system. */
    kCkPathType_FileSystem = 0,

#if CK_PLATFORM_ANDROID
    /** A path to an asset in the .apk. 
      Assets are read-only.
     @par Only available on Android. */
    kCkPathType_Asset,

    /** A path relative to the private files directory.
     @par Only available on Android. */
    kCkPathType_PrivateFiles,

    /** A path relative to the external storage directory.
     @par Only available on Android. */
    kCkPathType_ExternalStorage,
#endif

#if CK_PLATFORM_IOS || CK_PLATFORM_OSX
    /** A path relative to the application's resource directory. 
     @par Only available on iOS and OS X. */
    kCkPathType_Resource,
#endif

#if CK_PLATFORM_WIN || CK_PLATFORM_LINUX
    /** A path relative to the executable's directory. 
     @par Only available on Windows and Linux. */
    kCkPathType_ExeDir,
#endif

#if CK_PLATFORM_WP8
    /** A path relative to the local app data store.
     @par Only available on Windows Phone 8. */
    kCkPathType_LocalDir,

    /** A path relative to the folder containing your app's package files.
      Files in your Visual Studio project with the "Content" property set to true will be installed here.
      This folder is read-only.
     @par Only available on Windows Phone 8. */
    kCkPathType_InstallationDir,
#endif

    /** The default path type. */
#if CK_PLATFORM_ANDROID
    kCkPathType_Default = kCkPathType_Asset,
#elif CK_PLATFORM_IOS || CK_PLATFORM_OSX
    kCkPathType_Default = kCkPathType_Resource,
#elif CK_PLATFORM_WIN || CK_PLATFORM_LINUX
    kCkPathType_Default = kCkPathType_ExeDir,
#elif CK_PLATFORM_WP8
    kCkPathType_Default = kCkPathType_InstallationDir
#endif
} CkPathType;

/** @} */
