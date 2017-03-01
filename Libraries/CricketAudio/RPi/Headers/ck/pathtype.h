// Copyright 2016 Cricket Technology
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

#if CK_PLATFORM_IOS || CK_PLATFORM_OSX || CK_PLATFORM_TVOS
    /** A path relative to the application's resource directory.
      On iOS, the resource directory is the application bundle, and is read-only.
      On OS X, the resource directory is the directory containing the executable.
     @par Only available on iOS and OS X. */
    kCkPathType_Resource,
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

#if CK_PLATFORM_IOS || CK_PLATFORM_OSX || CK_PLATFORM_TVOS || CK_PLATFORM_WIN || CK_PLATFORM_LINUX
    /** A path relative to the directory used for temporary files.
     @par Only available on iOS, OS X, Windows, and Linux. */
    kCkPathType_Temp,
#endif

#if CK_PLATFORM_IOS || CK_PLATFORM_OSX || CK_PLATFORM_WIN || CK_PLATFORM_LINUX
    /** A path relative to the application's documents directory.
      On iOS, this is the app's Documents directory.
      On OS X and Windows, this is the user's Documents directory.
      On Linux, this is the user's home directory.
     @par Only available on iOS, OS X, Windows, and Linux. */
    kCkPathType_Documents,
#endif

#if CK_PLATFORM_WIN || CK_PLATFORM_LINUX || CK_PLATFORM_OSX
    /** A path relative to the executable's directory. 
     (On OS X, this is the same as kCkPathType_Resource.) 
     @par Only available on Windows, Linux, and OS X.  */
    kCkPathType_ExeDir,
#endif

    /** The default path type for reading files. */
#if CK_PLATFORM_ANDROID
    kCkPathType_ReadDefault = kCkPathType_Asset,
#elif CK_PLATFORM_IOS || CK_PLATFORM_OSX || CK_PLATFORM_TVOS
    kCkPathType_ReadDefault = kCkPathType_Resource,
#elif CK_PLATFORM_WIN || CK_PLATFORM_LINUX
    kCkPathType_ReadDefault = kCkPathType_ExeDir,
#elif CK_PLATFORM_WP8
    kCkPathType_ReadDefault = kCkPathType_InstallationDir,
#endif

    /** The default path type for writing files. */
#if CK_PLATFORM_ANDROID
    kCkPathType_WriteDefault = kCkPathType_PrivateFiles,
#elif CK_PLATFORM_IOS || CK_PLATFORM_OSX 
    kCkPathType_WriteDefault = kCkPathType_Documents,
#elif CK_PLATFORM_TVOS
    kCkPathType_WriteDefault = kCkPathType_Temp,
#elif CK_PLATFORM_WIN || CK_PLATFORM_LINUX
    kCkPathType_WriteDefault = kCkPathType_ExeDir,
#elif CK_PLATFORM_WP8
    kCkPathType_WriteDefault = kCkPathType_LocalDir,
#endif

    /** The default path type. */
    kCkPathType_Default = kCkPathType_ReadDefault

} CkPathType;

/** @} */
