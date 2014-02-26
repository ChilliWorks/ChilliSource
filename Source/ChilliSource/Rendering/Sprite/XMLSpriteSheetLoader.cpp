/*
 *  XMLSpriteSheetLoader.cpp
 *  CMMatchDay
 *
 *  Created by Stuart McGaw on 13/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Sprite/XMLSpriteSheetLoader.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>

#include <ChilliSource/Core/XML/XMLUtils.h>
#include <ChilliSource/Core/Base/Screen.h>

namespace ChilliSource
{
	namespace Rendering
	{
		const std::string kstrTagXmlSpriteExtension("mosprites");
	
		//-------------------------------------------------------------------------
		/// Constructor
		///
		/// @param Handle to moFlo App
		//-------------------------------------------------------------------------
		XMLSpriteSheetLoader::XMLSpriteSheetLoader() 
		{
			
		}
		//-------------------------------------------------------------------------
		/// Is A
		///
		/// @param Interface to compare
		/// @return Whether the object implements the given interface
		//-------------------------------------------------------------------------
		bool XMLSpriteSheetLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource of Kind
		///
		/// @param Type to compare
		/// @return Whether the object can create a resource of given type
		//----------------------------------------------------------------------------
		bool XMLSpriteSheetLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == SpriteSheet::InterfaceID);
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource From File With Extension
		///
		/// @param Type to compare
		/// @param Extension to compare
		/// @return Whether the object can create a resource with the given extension
		//----------------------------------------------------------------------------
		bool XMLSpriteSheetLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
		{
			return (inExtension == kstrTagXmlSpriteExtension);
		}
		//----------------------------------------------------------------------------
		/// Create Resource From File
		///
        /// @param The storage location to load from
		/// @param Filename
		/// @param Out: Resource object
		/// @return Whether the resource loaded
		//----------------------------------------------------------------------------
		bool XMLSpriteSheetLoader::CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)
		{
			using namespace Core;
            
			SpriteSheet* pData = (SpriteSheet*)(outpResource.get());
            
			TiXmlDocument Document;
            Document.LoadFile(ineStorageLocation, inFilePath);
	
			if(!Document.RootElement())
			{
				return false;
			}
			
			TiXmlElement * pFramesEl = Core::XMLUtils::FirstChildElementWithName(Document.RootElement(), "Frames");
			
			if (pFramesEl != nullptr)
            {
				for (TiXmlElement * pFrameEl = Core::XMLUtils::FirstChildElementWithName(pFramesEl, "Frame"); pFrameEl != nullptr; pFrameEl = XMLUtils::NextSiblingElementWithName(pFrameEl)){
					SpriteSheet::Frame sNewFrame;	
					sNewFrame.U = XMLUtils::GetAttributeValueOrDefault<s32>(pFrameEl, "x", 0);
					sNewFrame.V = XMLUtils::GetAttributeValueOrDefault<s32>(pFrameEl, "y", 0);
					sNewFrame.OffsetX = XMLUtils::GetAttributeValueOrDefault<s32>(pFrameEl, "ox", 0);
					sNewFrame.OffsetY = XMLUtils::GetAttributeValueOrDefault<s32>(pFrameEl, "oy", 0);
					sNewFrame.Width = XMLUtils::GetAttributeValueOrDefault<s32>(pFrameEl, "w", 0);
					sNewFrame.Height = XMLUtils::GetAttributeValueOrDefault<s32>(pFrameEl, "h", 0);
					
					pData->AddSpriteFrame(sNewFrame);
				}				
			}
			
			TiXmlElement * pAnimsEl = Core::XMLUtils::FirstChildElementWithName(Document.RootElement(), "Animations");
			
			if (pAnimsEl)
			{
				for (TiXmlElement * pAnimEl = Core::XMLUtils::FirstChildElementWithName(pAnimsEl, "Animation"); pAnimEl != nullptr; pAnimEl = XMLUtils::NextSiblingElementWithName(pAnimEl))
				{
					
					std::string strName = XMLUtils::GetAttributeValueOrDefault<std::string>(pAnimEl,"name",CStringUtils::BLANK);
					std::string strFrames = XMLUtils::GetAttributeValueOrDefault<std::string>(pAnimEl,"frames",CStringUtils::BLANK);
					
                    std::vector<std::string> astrFrames = CStringUtils::Split(strFrames," ");
					//TODO: WTF?
					std::vector<u32> aIndices;
				}
			}

			return true;
		}
		//-------------------------------------------------------------------------
		/// Destructor
		///
		//-------------------------------------------------------------------------
		XMLSpriteSheetLoader::~XMLSpriteSheetLoader()
		{
		}
	}
}

