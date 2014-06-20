//
//  TaggedFilenameResolver.cpp
//  Chilli Source
//  Created by Scott Downie on 19/06/2014.
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

#include <ChilliSource/Core/File/TaggedFilenameResolver.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/File/FileSystem.h>

#include <array>

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
            //--------------------------------------------------------------
            /// Filter the list of strings based on those that contain
            /// the given tag and not the ignored tags
            ///
            /// @author S Downie
            ///
            /// @param List to filter
            /// @param Tag to find
            ///
            /// @return Filtered list
            //--------------------------------------------------------------
            std::vector<std::string> FilterByTag(const std::vector<std::string>& in_list, const std::string& in_tag)
            {
                std::vector<std::string> result;
                result.reserve(in_list.size());
                
                for(const auto& string : in_list)
                {
                    if(string.find(in_tag) != std::string::npos)
                    {
                        result.push_back(string);
                    }
                }
                
                return result;
            }
            //--------------------------------------------------------------
            /// Filter the list of strings based on those that contain
            /// the given tag and not the ignored tags
            ///
            /// @author S Downie
            ///
            /// @param List to filter
            /// @param List of tags to ignore
            ///
            /// @return Filtered list
            //--------------------------------------------------------------
            std::vector<std::string> FilterByIgnoredTags(const std::vector<std::string>& in_list, const std::vector<std::string>& in_ignoredTags)
            {
                std::vector<std::string> result;
                result.reserve(in_list.size());
                
                for(const auto& string : in_list)
                {
                    bool ignoreString = false;
                    
                    for(const auto& ignore : in_ignoredTags)
                    {
                        if(string.find(ignore) != std::string::npos)
                        {
                            ignoreString = true;
                            break;
                        }
                    }
                    
                    if(ignoreString == false)
                    {
                        result.push_back(string);
                    }
                }
                
                return result;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(TaggedFilenameResolver);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        TaggedFilenameResolverUPtr TaggedFilenameResolver::Create()
        {
            return TaggedFilenameResolverUPtr(new TaggedFilenameResolver());
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool TaggedFilenameResolver::IsA(InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == InterfaceID;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedFilenameResolver::OnInit()
        {
            m_screen = Application::Get()->GetSystem<Screen>();
            CS_ASSERT(m_screen != nullptr, "TaggedFilenameResolver must have access to Screen system");
            m_device = Application::Get()->GetSystem<Device>();
            CS_ASSERT(m_device != nullptr, "TaggedFilenameResolver must have access to Device system");
            m_fileSystem = Application::Get()->GetFileSystem();
            CS_ASSERT(m_fileSystem != nullptr, "TaggedFilenameResolver must have access to FileSystem");
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedFilenameResolver::AddResolutionTag(const std::string& in_name, u32 in_minResIncl, u32 in_maxResExcl)
        {
            
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedFilenameResolver::AddAspectRatioTag(const std::string& in_name, u32 in_minRatioIncl, u32 in_maxRatioExcl)
        {
            
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedFilenameResolver::SetDelimitter(const std::string& in_delimitter)
        {
            m_delimitter = in_delimitter;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedFilenameResolver::SetPriority()
        {
            
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        std::string TaggedFilenameResolver::ResolvePath(StorageLocation in_location, const std::string& in_basePath) const
        {
            std::string filePath, fileName, fileExtension;
            StringUtils::SplitFullFilename(in_basePath, fileName, fileExtension, filePath);
            
            //Look for all files in the given folder with the given name and extension
            std::vector<std::string> previousPaths = m_fileSystem->GetFilePathsWithExtension(in_location, filePath, false, fileExtension);
            //TODO: Filter on name
            
            std::vector<std::string> tags = {".fr", ".ios", ".low", ".wide"};
            std::vector<std::vector<std::string>> alltags = {{".ios"}, {".high", ".low", ".med"}, {".wide"}, {".fr", ".en"}};
            
            std::vector<std::string> ignoredTags;
            
            u32 tagIndex = 0;
            std::vector<std::string> pathsContaining;
            
            while(pathsContaining.size() != 1)
            {
                if(tagIndex >= tags.size())
                {
                    //We have exhausted all tags and should return the base path
                    //TODO: Should we check the base path exists
                    return in_basePath;
                }
                
                pathsContaining = FilterByTag(previousPaths, tags[tagIndex]);
                
                if(pathsContaining.empty() == false)
                {
                    previousPaths = pathsContaining;
                }
                else
                {
                    //Ignore all of the tags for this group
                    ignoredTags.insert(ignoredTags.end(), alltags[tagIndex].begin(), alltags[tagIndex].end());
                    pathsContaining = FilterByIgnoredTags(previousPaths, ignoredTags);
                }
                
                ++tagIndex;
            }
            
            //Based on the priority check if any of them contain the priority tag
            
            //If multiple files contain the priority tag then check which one contains the next priority and so on...
            
            //If none of them contain the priority tag then move to the next priority
            
            return pathsContaining[0];
        }
    }
}
