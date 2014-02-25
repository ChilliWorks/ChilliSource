//
//  StandardMacros.h
//  Chilli Source
//
//  Created by Ian Copland on 25/02/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_BASE_STANDARDMACROS_H_
#define _CHILLISOURCE_CORE_BASE_STANDARDMACROS_H_

//TODO: Better way to not include the headers from here but still be able to swap libs easily
#include <memory>

//TODO: Remove the array deleter and put it in a more sutiable place. This is a functor for deleting shared arrays.
template <typename T> class ArrayDeleter
{
public:
    void operator () (T* in_arrayObject) const
    {
        delete[] in_arrayObject;
    }
};

#define SHARED_PTR std::shared_ptr
#define WEAK_PTR std::weak_ptr
#define WEAK_ARRAY_PTR std::weak_array
#define UNIQUE_PTR std::unique_ptr
#define UNIQUE_ARRAY_PTR std::unique_array
#define SHARED_PTR_CAST std::static_pointer_cast
#define ENABLE_SHARED_FROM_THIS std::enable_shared_from_this
#define SHARED_FUTURE std::shared_future
#define MAKE_SHARED_PTR std::make_shared

#include <ChilliSource/Core/Base/FastDelegate.h>

//TODO: Better way to not include the headers from here but still be able to swap libs easily i.e. if we want to use a different STL implementation
#include <unordered_map>
#define HASH_MAP std::unordered_map

//TODO: Better way to not include the headers from here but still be able to swap libs easily
#include <vector>
#define DYNAMIC_ARRAY std::vector

//Static Assertions
template <bool> struct CompileTimeChecker
{
    CompileTimeChecker(...);
};
template <> struct CompileTimeChecker<false>{};


//---Macros
#define SAFE_DELETE(x)						{if(x) delete(x); x = NULL;}
#define SAFE_DELETE_ARRAY(x)				{if(x) delete[] (x); x = NULL;}

#define PARSE_FLOAT(x)                      (Core::ParseF32(x))
#define PARSE_INT(x)                        (Core::ParseS32(x))
#define PARSE_UINT(x)                       (Core::ParseU32(x))
#define PARSE_BOOL(x)                       (Core::ParseBool(x))
#define PARSE_VECTOR2(x)                    (Core::ParseVector2(x))
#define PARSE_VECTOR3(x)                    (Core::ParseVector3(x))
#define PARSE_VECTOR4(x)                    (Core::ParseVector4(x))
#define PARSE_MATRIX4X4(x)                  (Core::ParseMatrix4(x))
#define PARSE_QUATERNION(x)                 (Core::ParseQuaternion(x))
#define PARSE_COLOUR(x)                     (Core::ParseColour(x))

#define STRING_HASH(x)						ChilliSource::Core::CHashCRC32::GenerateHashCode(x)
#define GET_LOCALISED_TEXT(x)               ChilliSource::Core::CLocalisedText::GetText(x)

#define DEBUG_LOG(x)						(ChilliSource::Core::CLogging::LogVerbose(x))
#define WARNING_LOG(x)						(ChilliSource::Core::CLogging::LogWarning(x))
#define ERROR_LOG(x)						(ChilliSource::Core::CLogging::LogError(x))
#define FATAL_LOG(x)						(ChilliSource::Core::CLogging::LogFatal(x))

#ifdef DEBUG
#define MOFLOW_ASSERT(x, msg) if((x) == 0){ChilliSource::Core::CLogging::LogFatal(msg);}
#else
#define MOFLOW_ASSERT(x, msg)
#endif

#ifdef DEBUG
#define SMART_CAST(to, from) dynamic_cast<to >(from); assert(dynamic_cast<to >(from) != NULL);
#else
#define SMART_CAST(to, from) static_cast<to >(from);
#endif

//---Missing OpenGL definitions
#define GL_GLEXT_PROTOTYPES
#ifdef TARGET_ANDROID
typedef char GLchar;
#endif



#endif
