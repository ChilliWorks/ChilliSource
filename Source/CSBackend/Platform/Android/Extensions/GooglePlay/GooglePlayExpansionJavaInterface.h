//
//  GooglePlayExpansionJavaInterface.h
//  Chilli Source
//  Created by Ian Copland on 09/04/2014.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifdef CS_ANDROIDEXTENSION_GOOGLEPLAY

#ifndef _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_GOOGLEPLAYEXPANSIONJAVAINTERFACE_H_
#define _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_GOOGLEPLAYEXPANSIONJAVAINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>

namespace CSBackend
{
	namespace Android
	{
        class GooglePlayExpansionJavaInterface : public CSBackend::Android::IJavaInterface
        {
        public:

        	CS_DECLARE_NAMEDTYPE(GooglePlayExpansionJavaInterface);

        	GooglePlayExpansionJavaInterface();

        	bool IsA(CSCore::InterfaceIDType inInterfaceID) const;
        	void SetGooglePlayExpansionSystem(GooglePlayExpansionSystem* in_expansionSystem);
        	void Init();

        	void Download();
        	void PauseDownload();
        	void ResumeDownload();
        	f32 GetDownloadProgress();

        	bool DoExpansionFilesExist();
        	u64 GetExternalFreeStorageInBytes();

        	void KeepAppAwake();
        	void AllowAppToSleep();

        	u32 GetNumExpansions();
        	u32 GetExpansionVersionCode(u32 inudwIndex);
        	u64 GetExpansionFileSizeInBytes(u32 inudwIndex);
        	std::string GetExpansionPath(u32 inudwIndex);
        };
	}
}

#endif

#endif

#endif
