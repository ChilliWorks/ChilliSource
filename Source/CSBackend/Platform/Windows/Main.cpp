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

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <CSBackend/Platform/Windows/SFML/Base/SFMLWindow.h>

#include <Windows.h>

//----------------------------------------------------------------------------------
/// Includes the "main" function as required by Windows and will create
/// the inherited CS application using the exposed CreateApplication method that
/// the application code base must implement.
///
/// @author S Downie
///
/// @param Handle to current application instance
/// @param Handle to previous application instance
/// @param Command line
/// @param Controls how the window is to be shown (i.e hide, maximize, etc)
///
/// @return Exit status
//----------------------------------------------------------------------------------
int CALLBACK WinMain(_In_ HINSTANCE in_instance, _In_ HINSTANCE in_prevInstance, _In_ LPSTR in_cmdLine, _In_ int in_cmdShow)
{
	CSBackend::Windows::SFMLWindow::Create();
	CSBackend::Windows::SFMLWindow::Get()->Run();
	CSBackend::Windows::SFMLWindow::Destroy();
    return 0;
}

#endif
