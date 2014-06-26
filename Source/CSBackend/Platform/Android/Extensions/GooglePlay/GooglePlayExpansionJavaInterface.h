//
//  GooglePlayExpansionSystem.h
//  Chilli Source
//
//  Created by Ian Copland on 09/04/2014.
//  Copyright (c) 2014 Tag Games. All rights reserved.
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
