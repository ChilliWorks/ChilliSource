/*
 * File: AudioLoader.cpp
 * Date: 16/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Audio/Base/AudioLoader.h>
#include <ChilliSource/Audio/Base/AudioResource.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

namespace moFlo
{
	namespace Audio
	{
		//-------------------------------------------------------------------------
		/// Constructor
		///
		/// Register this object as a model provider
		//-------------------------------------------------------------------------
		CAudioLoader::CAudioLoader(IAudioSystem* inpAudioSystem) : mAudioManager(inpAudioSystem->GetAudioManager())
		{
            Core::CResourceManagerDispenser::GetSingletonPtr()->RegisterResourceManager(const_cast<IAudioManager*>(&mAudioManager));
		}
		//-------------------------------------------------------------------------
		/// Is A
		///
		/// @param Interface to compare
		/// @return Whether the object implements the given interface
		//-------------------------------------------------------------------------
		bool CAudioLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource of Kind
		///
		/// @param Type to compare
		/// @return Whether the object can create a resource of given type
		//----------------------------------------------------------------------------
		bool CAudioLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == Audio::IAudioResource::InterfaceID);
		}
		//-------------------------------------------------------------------------
		/// Destructor
		///
		//-------------------------------------------------------------------------
		CAudioLoader::~CAudioLoader() 
		{

		}
	}
}