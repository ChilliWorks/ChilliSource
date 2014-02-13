/*
 *  TagFoundation.h
 *  moFlo
 *
 *  Created by Scott Downie on 22/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

//========================================================
// Description:
//
// Holds the type defines for integral data types as set
// out in the Tag Games coding standards.
//
// Holds required header files
//========================================================

#ifndef _TAG_FOUNDATION_H_
#define _TAG_FOUNDATION_H_

#include <moFlo/Core/boost/bind.hpp>
#include <moFlo/Core/boost/tr1/memory.hpp>
#include <moFlo/Core/boost/unordered_map.hpp>
#include <moFlo/Core/boost/circular_buffer.hpp>
#include <moFlo/Core/boost/static_assert.hpp>
#include <moFlo/Core/boost/smart_ptr/shared_array.hpp>
#include <moFlo/Core/boost/smart_ptr/shared_ptr.hpp>
#include <moFlo/Core/boost/enable_shared_from_this.hpp>
#include <moFlo/Core/boost/thread/future.hpp>

#define SHARED_PTR boost::shared_ptr
#define SHARED_ARRAY_PTR boost::shared_array
#define WEAK_PTR boost::weak_ptr
#define WEAK_ARRAY_PTR boost::weak_array
#define UNIQUE_PTR boost::scoped_ptr
#define UNIQUE_ARRAY_PTR boost::scoped_array
#define SHARED_PTR_CAST boost::static_pointer_cast
#define ENABLE_SHARED_FROM_THIS boost::enable_shared_from_this
#define SHARED_FUTURE boost::shared_future
#define MAKE_SHARED_PTR boost::make_shared
#define HASH_MAP boost::unordered_map
#define RING_BUFFER boost::circular_buffer

#ifdef RDE_VECTOR
#include <moFlo/RDESTL/vector.h>
#define DYNAMIC_ARRAY rde::vector
#else
#include <vector>
#define DYNAMIC_ARRAY std::vector
#endif

#include <string>
#include <boost/cstdint.hpp>

typedef char         		s8;
typedef int_least16_t		s16;
typedef int_least32_t		s32;
typedef int_least64_t		s64;

typedef unsigned char		u8;
typedef uint_least16_t		u16;
typedef uint_least32_t		u32;
typedef uint_least64_t		u64;

typedef float				f32;
typedef double				f64;

typedef u64                 TimeIntervalSecs;
typedef u64                 TimeIntervalMs;
typedef	const s8*			cString;

namespace moFlo
{
    namespace Core{}
    namespace Rendering{}
    namespace Networking{}
    namespace GUI{}
    namespace Audio{}
    namespace Input{}
    namespace Physics2D{}
    namespace Physics3D{}
}

namespace moCore = moFlo::Core;
namespace moRendering = moFlo::Rendering;
namespace moNetworking = moFlo::Networking;
namespace moGUI = moFlo::GUI;
namespace moAudio = moFlo::Audio;
namespace moPhysics2D = moFlo::Physics2D;
namespace moPhysics3D= moFlo::Physics3D;
namespace moInput = moFlo::Input;


//Static Assertions
template <bool> struct CompileTimeChecker
{
    CompileTimeChecker(...);
};
template <> struct CompileTimeChecker<false>{};

#define STATIC_ASSERT(EXPR, MSG)\
{\
    class ASSERT_ERROR_##MSG; \
    (void)sizeof(CompileTimeChecker<(EXPR) != 0>((ASSERT_ERROR_##MSG)));\
}

//---Macros
#define SAFE_DELETE(x)						{if(x) delete(x); x = NULL;}
#define SAFE_DELETE_ARRAY(x)				{if(x) delete[] (x); x = NULL;}

#define STRING_CAST(x)						(moFlo::Core::CStringConverter::ToString(x))

#define PARSE_FLOAT(x)                      (moCore::CStringConverter::ParseFloat(x))
#define PARSE_INT(x)                        (moCore::CStringConverter::ParseInt(x))
#define PARSE_UINT(x)                       (moCore::CStringConverter::ParseUnsignedInt(x))
#define PARSE_BOOL(x)                       (moCore::CStringConverter::ParseBool(x))
#define PARSE_VECTOR2(x)                    (moCore::CStringConverter::ParseVector2(x))
#define PARSE_VECTOR3(x)                    (moCore::CStringConverter::ParseVector3(x))
#define PARSE_VECTOR4(x)                    (moCore::CStringConverter::ParseVector4(x))
#define PARSE_MATRIX4X4(x)                  (moCore::CStringConverter::ParseMatrix4(x))
#define PARSE_QUATERNION(x)                 (moCore::CStringConverter::ParseQuaternion(x))
#define PARSE_COLOUR(x)                     (moCore::CStringConverter::ParseColourValue(x))

#define STRING_HASH(x)						moFlo::CHashCRC32::GenerateHashCode(x)
#define GET_LOCALISED_TEXT(x)               moFlo::Core::CLocalisedText::GetText(x)

#define DEBUG_LOG(x)						(moFlo::CLogging::LogVerbose(x))
#define WARNING_LOG(x)						(moFlo::CLogging::LogWarning(x))
#define ERROR_LOG(x)						(moFlo::CLogging::LogError(x))
#define FATAL_LOG(x)						(moFlo::CLogging::LogFatal(x))


#ifdef DEBUG
    #define MOFLOW_ASSERT(x, msg) if((x) == 0){moFlo::CLogging::LogFatal(msg);}
#else
    #define MOFLOW_ASSERT(x, msg)
#endif

#ifdef DEBUG
    #define SMART_CAST(to, from) dynamic_cast<to >(from); assert(dynamic_cast<to >(from) != NULL);  
#else
    #define SMART_CAST(to, from) static_cast<to >(from);
#endif


#include <moFlo/Core/Logging.h>
#include <moFlo/BuildProperties.h>
#include <moFlo/Core/StringConverter.h>
#include <moFlo/Core/LocalisedText.h>

//---Missing OpenGL definitions
#define GL_GLEXT_PROTOTYPES
#ifdef TARGET_ANDROID
	typedef char GLchar;
#endif

#endif
