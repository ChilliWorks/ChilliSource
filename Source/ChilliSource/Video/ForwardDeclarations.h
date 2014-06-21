//
//  ForwardDeclarations.h
//  Chilli Source
//  Created by Scott Downie on 12/05/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_VIDEO_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_VIDEO_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource
{
    namespace Video
    {
        //------------------------------------------------------
        /// Base
        //------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(CSSubtitlesProvider);
        CS_FORWARDDECLARE_CLASS(Subtitles);
        CS_FORWARDDECLARE_CLASS(SubtitlesManager);
        CS_FORWARDDECLARE_CLASS(VideoPlayer);
    }
}

#endif