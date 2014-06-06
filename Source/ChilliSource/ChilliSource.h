//
//  ChilliSource.h
//  Chilli Source
//
//  Created by Tag Games on 25/02/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_CHILLISOURCE_H_
#define _CHILLISOURCE_CHILLISOURCE_H_

#include <memory>
#include <cstdint>

//------------------------------------------------------------
/// Primitive types. These should be prefered over the standard
/// primitive types as they guarantee their size.
//------------------------------------------------------------
typedef char         		s8;
typedef int_least16_t		s16;
typedef int_least32_t		s32;
typedef int_least64_t		s64;

typedef uint_least8_t		u8;
typedef uint_least16_t		u16;
typedef uint_least32_t		u32;
typedef uint_least64_t		u64;

typedef float				f32;
typedef double				f64;

typedef u64                 TimeIntervalSecs;
typedef u64                 TimeIntervalMs;

#include <ChilliSource/Core/Base/StandardMacros.h>
//------------------------------------------------------------
/// The forward declarations for each section of the Chilli
/// Source libraries.
//------------------------------------------------------------
#include <ChilliSource/Audio/ForwardDeclarations.h>
#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Debugging/ForwardDeclarations.h>
#include <ChilliSource/Input/ForwardDeclarations.h>
#include <ChilliSource/GUI/ForwardDeclarations.h>
#include <ChilliSource/Networking/ForwardDeclarations.h>
#include <ChilliSource/Rendering/ForwardDeclarations.h>
#include <ChilliSource/Social/ForwardDeclarations.h>
#include <ChilliSource/UI/ForwardDeclarations.h>
#include <ChilliSource/Video/ForwardDeclarations.h>
#include <ChilliSource/Web/ForwardDeclarations.h>
//------------------------------------------------------------
/// The base include files.
//------------------------------------------------------------
#include <ChilliSource/Core/String/ToString.h>
#include <ChilliSource/Core/Base/Logging.h>
//------------------------------------------------------------
/// Alternative, shorter namespace aliases for each of the
/// Chilli Source libraries.
//------------------------------------------------------------
namespace ChilliSource
{
    namespace Audio{}
    namespace Core{}
    namespace Debugging{}
    namespace Input{}
    namespace GUI{}
    namespace Networking{}
    namespace Rendering{}
    namespace Social{}
    namespace UI{}
    namespace Video{}
    namespace Web{}
}
namespace CSAudio = ChilliSource::Audio;
namespace CSCore = ChilliSource::Core;
namespace CSDebugging = ChilliSource::Debugging;
namespace CSInput = ChilliSource::Input;
namespace CSGUI = ChilliSource::GUI;
namespace CSNetworking = ChilliSource::Networking;
namespace CSRendering = ChilliSource::Rendering;
namespace CSSocial = ChilliSource::Social;
namespace CSUI = ChilliSource::UI;
namespace CSVideo = ChilliSource::Video;
namespace CSWeb = ChilliSource::Web;

#endif
