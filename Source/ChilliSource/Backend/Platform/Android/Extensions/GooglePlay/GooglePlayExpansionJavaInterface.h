//
//  GooglePlayExpansionSystem.h
//  Chilli Source
//
//  Created by I Copland on 09/04/2014.
//  Copyright (c) 2014 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_GOOGLEPLAYEXPANSIONJAVAINTERFACE_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_GOOGLEPLAYEXPANSIONJAVAINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>

namespace ChilliSource
{
	namespace Android
	{
        class GooglePlayExpansionJavaInterface : public ChilliSource::Android::IJavaInterface
        {
        public:

        	CS_DECLARE_NAMEDTYPE(GooglePlayExpansionJavaInterface);

        	GooglePlayExpansionJavaInterface();

        	bool IsA(Core::InterfaceIDType inInterfaceID) const;
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
