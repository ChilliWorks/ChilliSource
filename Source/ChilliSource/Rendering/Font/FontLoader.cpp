/*
 * File: FontLoader.cpp
 * Date: 26/10/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>

#include <ChilliSource/Rendering/Font/FontLoader.h>
#include <ChilliSource/Rendering/Font/Font.h>

namespace ChilliSource
{
	namespace Rendering
	{
		const std::string kstrGlyphExtension("alphabet");
        const std::string kstrKerningExtension("kerninginfo");
		
		//-------------------------------------------------------------------------
		/// Is A
		///
		/// @param Interface to compare
		/// @return Whether the object implements the given interface
		//-------------------------------------------------------------------------
		bool FontLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource of Kind
		///
		/// @param Type to compare
		/// @return Whether the object can create a resource of given type
		//----------------------------------------------------------------------------
		bool FontLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == Font::InterfaceID);
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource From File With Extension
		///
		/// @param Type to compare
		/// @param Extension to compare
		/// @return Whether the object can create a resource with the given extension
		//----------------------------------------------------------------------------
		bool FontLoader::CanCreateResourceFromFileWithExtension(const std::string& inExtension) const
		{
			return (inExtension == kstrGlyphExtension);
		}
		//----------------------------------------------------------------------------
		/// Create Resource From File
		///
		/// Load the font from the external file
		///
        /// @param The storage location to load from
		/// @param Filename
		/// @param Out: Resource object
		/// @return Whether the resource was created successfully
		//----------------------------------------------------------------------------
		bool FontLoader::CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string& inFilePath, Core::ResourceSPtr& outpResource)
		{
            bool bResult = false;
            
			Font* pFont = (Font*)(outpResource.get());
			
			//Buffer for the character set
            std::string strGlyphs;
			
			//Open the characters for reading only
			Core::FileStreamSPtr pGlyphsStream = Core::Application::Get()->GetFileSystem()->CreateFileStream(ineStorageLocation, inFilePath, Core::FileMode::k_read);

			if(pGlyphsStream && !pGlyphsStream->IsBad())
			{
				//Read the contents into a string
				pGlyphsStream->GetLine(strGlyphs);
				pGlyphsStream->Close();
			
				//Create the font
				pFont->SetCharacterSet(strGlyphs);
				
                if(HasKerningInfo(ineStorageLocation, inFilePath))
                {
                    bResult = LoadKerningInfo(ineStorageLocation, inFilePath, pFont);
                }
                else
                {
                    bResult = true;
                }
			}
			
			return bResult;
		}
        //----------------------------------------------------------------------------
        /// Has Kerning Info
        //----------------------------------------------------------------------------
        const bool FontLoader::HasKerningInfo(const Core::StorageLocation ineStorageLocation, const std::string& inFilePath) const
        {
            // Get the kerning file
            std::string strFileName, strExtension;
			Core::StringUtils::SplitBaseFilename(inFilePath, strFileName, strExtension);
            const std::string stdKerningFilePath(strFileName+"."+kstrKerningExtension);
			ChilliSource::Core::FileStreamSPtr pKerningStream = Core::Application::Get()->GetFileSystem()->CreateFileStream(ineStorageLocation, stdKerningFilePath, Core::FileMode::k_read);
            
            return (pKerningStream && !pKerningStream->IsBad());
        }
        //----------------------------------------------------------------------------
        /// Load Kerning Info
        //----------------------------------------------------------------------------
        bool FontLoader::LoadKerningInfo(Core::StorageLocation ineStorageLocation, const std::string& inFilePath, Font* inpFont)
        {
            // Get the kerning file
            std::string strFileName, strExtension;
			Core::StringUtils::SplitBaseFilename(inFilePath, strFileName, strExtension);
            const std::string stdKerningFilePath(strFileName+"."+kstrKerningExtension);
			Core::FileStreamSPtr pKerningStream = Core::Application::Get()->GetFileSystem()->CreateFileStream(ineStorageLocation, stdKerningFilePath, Core::FileMode::k_read);
            
			if(pKerningStream && !pKerningStream->IsBad())
            {
				s32 dwKerningPairs;
				pKerningStream->Read((s8*)&dwKerningPairs, sizeof(s32));
				dwKerningPairs = Core::Utils::Endian4ByteSwap(&dwKerningPairs);
				
				// Kerning stored in a first character look up register
				std::vector<Font::CKernLookup> aKernLookup;
				std::vector<Font::CKernPair> aKernPairs;
				aKernPairs.reserve(dwKerningPairs);
                
				for(s32 dwPair = 0; dwPair < dwKerningPairs; dwPair++)
                {
                    s16 dwChar1;
					s16 dwChar2;
					s16 dwSpacing;
					
					pKerningStream->Read((s8*)&dwChar1, sizeof(s16));
					dwChar1 = Core::Utils::Endian2ByteSwap(&dwChar1);
					
					pKerningStream->Read((s8*)&dwChar2, sizeof(s16));
					dwChar2 = Core::Utils::Endian2ByteSwap(&dwChar2);
					
					pKerningStream->Read((s8*)&dwSpacing, sizeof(s16));
					dwSpacing = Core::Utils::Endian2ByteSwap(&dwSpacing);
					
					// Check if we are on a new c1
					if(aKernLookup.empty())
					{
						aKernLookup.push_back(Font::CKernLookup(dwChar1, aKernPairs.size(), 0));
					}
					else if(aKernLookup.back().wCharacter != dwChar1)
					{
						aKernLookup.back().uwLength = aKernPairs.size() - aKernLookup.back().uwStart;
						aKernLookup.push_back(Font::CKernLookup(dwChar1, aKernPairs.size(), 0));
					}
					aKernPairs.push_back(Font::CKernPair(dwChar2, static_cast<f32>(dwSpacing)));
				}
				inpFont->SetKerningInfo(aKernLookup, aKernPairs);
                
                return true;
			}
            
            return false;
        }
        
		//-------------------------------------------------------------------------
		/// Destructor
		///
		//-------------------------------------------------------------------------
		FontLoader::~FontLoader()
		{
		}
	}
}
