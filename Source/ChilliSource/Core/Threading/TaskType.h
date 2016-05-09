//
//  TaskType.h
//  ChilliSource
//  Created by Ian Copland on 05/04/2016.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_THREADING_TASKTYPE_H_
#define _CHILLISOURCE_CORE_THREADING_TASKTYPE_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// An enum describing the various types of tasks which are available to be
    /// scheduled.
    ///
    /// Small task: A task with a small execution time which should be processed
    /// within a single frame. These have a high priority. Note that small tasks
    /// which take a long time to execute will have a significant negative impact
    /// on performance. Use large tasks where a longer execution time is required.
    ///
    /// Large task: A task with a large execution time which can be processed over
    /// a number of frames and can take several seconds to process. These have a
    /// low priority.
    ///
    /// Main thread task: A small task which is executed on the main thread. This
    /// occurs after the game logic for each frame is executed.
    ///
    /// Game logic task: A small task which is executed on a background thread and
    /// is guaranteed to be processed within the game logic stage, prior to
    /// executing the main thread tasks.
    ///
    /// File Task: A large task specifically for processing file input or output.
    /// All background processing of files should use this rather than standard
    /// large tasks.
    ///
    /// @author Ian Copland
    //------------------------------------------------------------------------------
    enum class TaskType
    {
        k_small,
        k_large,
        k_mainThread,
        k_gameLogic,
        k_file
    };
}

#endif
