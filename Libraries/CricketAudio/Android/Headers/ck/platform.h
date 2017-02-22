// Copyright 2016 Cricket Technology
// www.crickettechnology.com
/** @file */

#pragma once

// software platform
#undef CK_PLATFORM_IOS
#undef CK_PLATFORM_TVOS
#undef CK_PLATFORM_ANDROID
#undef CK_PLATFORM_OSX
#undef CK_PLATFORM_WIN
#undef CK_PLATFORM_WP8
#undef CK_PLATFORM_LINUX

#ifdef __APPLE__
#  include <TargetConditionals.h>
#  if TARGET_OS_IPHONE
#    if TARGET_OS_IOS
#      define CK_PLATFORM_IOS 1
#    elif TARGET_OS_TV
#      define CK_PLATFORM_TVOS 1
#    endif
#  else
#    define CK_PLATFORM_OSX 1
#  endif
#elif defined(__ANDROID__)
#  define CK_PLATFORM_ANDROID 1
#elif defined(__linux__)
#  define CK_PLATFORM_LINUX 1
#elif defined(_WIN64) || defined(_WIN32)
#  if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#     define CK_PLATFORM_WP8 1
#  else
#     define CK_PLATFORM_WIN 1
#  endif
#endif

#if CK_PLATFORM_OSX && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1060)
#  error "OS X 10.6 required!"
#endif

#if !CK_PLATFORM_IOS && !CK_PLATFORM_ANDROID && !CK_PLATFORM_OSX && !CK_PLATFORM_WIN && !CK_PLATFORM_WP8 && !CK_PLATFORM_LINUX && !CK_PLATFORM_TVOS
#  error "Unsupported platform!"
#endif
