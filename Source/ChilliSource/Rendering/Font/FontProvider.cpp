//
//  FontProvider.cpp
//  Chilli Source
//
//  Created by Scott Downie on 26/10/2010.
//  Copyright 2010 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Rendering/Font/FontProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Rendering/Font/Font.h>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            const std::string k_charFileExtension("alphabet");
            const std::string k_kerningFileExtension("kerninginfo");
            
            //----------------------------------------------------------------------------
            /// Load the font kerning information from the external file
            ///
            /// @author R Henning
			///
            /// @param The storage location to load from
			/// @param Filename of font
			/// @param [Out] Pointer to font resource
            //----------------------------------------------------------------------------
            void LoadKerningInfo(const Core::FileStreamSPtr& in_kerningStream, Font* out_font)
            {
                u32 numKerningPairs;
                in_kerningStream->Read((s8*)&numKerningPairs, sizeof(u32));
                numKerningPairs = Core::Utils::Endian4ByteSwap(&numKerningPairs);
                
                // Kerning stored in a first character look up register
                std::vector<Font::KernLookup> kernLookups;
                std::vector<Font::KernPair> kernPairs;
                kernPairs.reserve(numKerningPairs);
                
                for(u32 pair=0; pair<numKerningPairs; ++pair)
                {
                    s16 char1;
                    in_kerningStream->Read((s8*)&char1, sizeof(s16));
                    char1 = Core::Utils::Endian2ByteSwap(&char1);
                    
                    s16 char2;
                    in_kerningStream->Read((s8*)&char2, sizeof(s16));
                    char2 = Core::Utils::Endian2ByteSwap(&char2);
                    
                    s16 spacing;
                    in_kerningStream->Read((s8*)&spacing, sizeof(s16));
                    spacing = Core::Utils::Endian2ByteSwap(&spacing);
                    
                    // Check if we are on a new c1
                    if(kernLookups.empty())
                    {
                        kernLookups.push_back(Font::KernLookup(char1, kernPairs.size()));
                    }
                    else if(kernLookups.back().wCharacter != char1)
                    {
                        kernLookups.back().uwLength = kernPairs.size() - kernLookups.back().uwStart;
                        kernLookups.push_back(Font::KernLookup(char1, kernPairs.size()));
                    }
                    kernPairs.push_back(Font::KernPair(char2, static_cast<f32>(spacing)));
                }
                
                out_font->SetKerningInfo(kernLookups, kernPairs);
            }
        }
		
        CS_DEFINE_NAMEDTYPE(FontProvider);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        FontProviderUPtr FontProvider::Create()
        {
            return FontProviderUPtr(new FontProvider());
        }
		//-------------------------------------------------------------------------e
		//-------------------------------------------------------------------------
		bool FontProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ResourceProviderOld::InterfaceID || in_interfaceId == FontProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool FontProvider::CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == Font::InterfaceID;
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool FontProvider::CanCreateResourceFromFileWithExtension(const std::string& in_extension) const
		{
			return in_extension == k_charFileExtension;
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool FontProvider::CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceOldSPtr& out_resource)
		{
            out_resource->SetLoaded(false);
            
			Core::FileStreamSPtr characterStream = Core::Application::Get()->GetFileSystem()->CreateFileStream(in_location, in_filePath, Core::FileMode::k_read);
            if(characterStream == nullptr || characterStream->IsBad())
            {
                return false;
            }

            //Buffer for the character set
            std::string characters;
            characterStream->GetLine(characters);
            characterStream->Close();
			
            Font* font = (Font*)(out_resource.get());
            font->SetCharacterSet(characters);
            
            // Get the kerning file
            std::string fileName, fileExtension;
            Core::StringUtils::SplitBaseFilename(in_filePath, fileName, fileExtension);
            const std::string kerningFilePath(fileName + "." + k_kerningFileExtension);
            Core::FileStreamSPtr kerningStream = Core::Application::Get()->GetFileSystem()->CreateFileStream(in_location, kerningFilePath, Core::FileMode::k_read);
            
            if(kerningStream != nullptr && kerningStream->IsBad() == false)
            {
                LoadKerningInfo(kerningStream, font);
            }
            
            out_resource->SetLoaded(true);
			
			return true;
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        bool FontProvider::AsyncCreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceOldSPtr& out_resource)
        {
            //TODO: Implement this once we have a way of resolving whether loading of kerning and alphabet file on separate threads
            //has worked
            
            out_resource->SetLoaded(false);
            
            CS_LOG_WARNING("Async Font Loading Not Implemented");
            return false;
        }
	}
}
