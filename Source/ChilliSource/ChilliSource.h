//
//  ChilliSource.h
//  Chilli Source
//  Created by Ian Copland on 25/02/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
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
#include <ChilliSource/Input/ForwardDeclarations.h>
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
    namespace Input{}
    namespace Networking{}
    namespace Rendering{}
    namespace Social{}
    namespace UI{}
    namespace Video{}
    namespace Web{}
}
namespace CSAudio = ChilliSource::Audio;
namespace CSCore = ChilliSource::Core;
namespace CSInput = ChilliSource::Input;
namespace CSNetworking = ChilliSource::Networking;
namespace CSRendering = ChilliSource::Rendering;
namespace CSSocial = ChilliSource::Social;
namespace CSUI = ChilliSource::UI;
namespace CSVideo = ChilliSource::Video;
namespace CSWeb = ChilliSource::Web;

#endif
