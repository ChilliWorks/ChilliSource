//
//  Main.cpp
//  Chilli Source
//  Created by Ian Copland on 06/07/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_IOS

#include <ChilliSource/Core/Base/Application.h>

#import <UIKit/UIKit.h>

//----------------------------------------------------------------------------------
/// Includes the "main" function as required by iOS and will create
/// the inherited CS application using the exposed CreateApplication method that
/// the application code base must implement.
///
/// @author S Downie
///
/// @param Num arguments from command line
/// @param Arguments array from command line
///
/// @return Exit status
//----------------------------------------------------------------------------------
int main(int in_argc, char* in_argv[])
{
    @autoreleasepool
    {
        s32 retVal = UIApplicationMain(in_argc, in_argv, nil, @"CSAppDelegate");
        return retVal;
    }
}

#endif