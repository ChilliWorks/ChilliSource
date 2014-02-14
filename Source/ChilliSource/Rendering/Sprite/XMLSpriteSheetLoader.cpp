/*
 *  XMLSpriteSheetLoader.cpp
 *  CMMatchDay
 *
 *  Created by Stuart McGaw on 13/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/XMLSpriteSheetLoader.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>

#include <ChilliSource/Core/XML/XMLUtils.h>
#include <ChilliSource/Core/Main/Screen.h>

namespace moFlo
{
	namespace Rendering
	{
		const std::string kstrTagXmlSpriteExtension("mosprites");
	
		//-------------------------------------------------------------------------
		/// Constructor
		///
		/// @param Handle to moFlo App
		//-------------------------------------------------------------------------
		CXMLSpriteSheetLoader::CXMLSpriteSheetLoader() 
		{
			
		}
		//-------------------------------------------------------------------------
		/// Is A
		///
		/// @param Interface to compare
		/// @return Whether the object implements the given interface
		//-------------------------------------------------------------------------
		bool CXMLSpriteSheetLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource of Kind
		///
		/// @param Type to compare
		/// @return Whether the object can create a resource of given type
		//----------------------------------------------------------------------------
		bool CXMLSpriteSheetLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == Rendering::CSpriteSheet::InterfaceID);
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource From File With Extension
		///
		/// @param Type to compare
		/// @param Extension to compare
		/// @return Whether the object can create a resource with the given extension
		//----------------------------------------------------------------------------
		bool CXMLSpriteSheetLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
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
		bool CXMLSpriteSheetLoader::CreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)
		{
			using namespace Core;
            
			CSpriteSheet* pData = (CSpriteSheet*)(outpResource.get());
            
			TiXmlDocument Document;
            Document.LoadFile(ineStorageLocation, inFilePath);
	
			if(!Document.RootElement())
			{
				return false;
			}
			
			TiXmlElement * pFramesEl = Core::XMLUtils::FirstChildElementWithName(Document.RootElement(), "Frames");
			
			if (pFramesEl != NULL)
            {
				for (TiXmlElement * pFrameEl = Core::XMLUtils::FirstChildElementWithName(pFramesEl, "Frame"); pFrameEl != NULL; pFrameEl = XMLUtils::NextSiblingElementWithName(pFrameEl)){
					CSpriteSheet::Frame sNewFrame;	
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
				for (TiXmlElement * pAnimEl = Core::XMLUtils::FirstChildElementWithName(pAnimsEl, "Animation"); pAnimEl != NULL; pAnimEl = XMLUtils::NextSiblingElementWithName(pAnimEl))
				{
					
					std::string strName = XMLUtils::GetAttributeValueOrDefault<std::string>(pAnimEl,"name",CStringUtils::BLANK);
					std::string strFrames = XMLUtils::GetAttributeValueOrDefault<std::string>(pAnimEl,"frames",CStringUtils::BLANK);
					
                    DYNAMIC_ARRAY<std::string> astrFrames = CStringUtils::Split(strFrames," ");
					//TODO: WTF?
					DYNAMIC_ARRAY<u32> aIndices;
				}
			}

			return true;
		}
		//-------------------------------------------------------------------------
		/// Destructor
		///
		//-------------------------------------------------------------------------
		CXMLSpriteSheetLoader::~CXMLSpriteSheetLoader()
		{
		}
	}
}

