//
//  TaggedPathResolver.cpp
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

#include <ChilliSource/Core/File/TaggedPathResolver.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Event/IConnectableEvent.h>
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
            /// the given text
            ///
            /// @author S Downie
            ///
            /// @param List to filter
            /// @param text
            ///
            /// @return Filtered list
            //--------------------------------------------------------------
            std::vector<std::string> FilterContains(const std::vector<std::string>& in_list, const std::string& in_text)
            {
                std::vector<std::string> result;
                result.reserve(in_list.size());
                
                for(const auto& string : in_list)
                {
                    if(string.find(in_text) != std::string::npos)
                    {
                        result.push_back(string);
                    }
                }
                
                return result;
            }
            //--------------------------------------------------------------
            /// Filter the list of strings based on those that contain
            /// the given tag or do not conflict with the other tags in the group
            ///
            /// @author S Downie
            ///
            /// @param List to filter
            /// @param Tag to find
            /// @param Other tags in the same group
            ///
            /// @return Filtered list
            //--------------------------------------------------------------
            std::vector<std::string> FilterContainsExclusive(const std::vector<std::string>& in_list, const std::string& in_tag, const std::vector<std::string>& in_groupTags)
            {
                std::vector<std::string> result;
                result.reserve(in_list.size());
                
                for(const auto& string : in_list)
                {
                    if(string.find(in_tag) != std::string::npos)
                    {
                        result.push_back(string);
                    }
                    else
                    {
                        bool ignoreString = false;
                        
                        for(const auto& otherTag : in_groupTags)
                        {
                            if(string.find(otherTag) != std::string::npos)
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
                }
                
                return result;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(TaggedPathResolver);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        TaggedPathResolverUPtr TaggedPathResolver::Create()
        {
            return TaggedPathResolverUPtr(new TaggedPathResolver());
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool TaggedPathResolver::IsA(InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == InterfaceID;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedPathResolver::OnInit()
        {
            m_fileSystem = Application::Get()->GetFileSystem();
            CS_ASSERT(m_fileSystem != nullptr, "TaggedPathResolver must have access to FileSystem");
            m_screen = Application::Get()->GetSystem<Screen>();
            CS_ASSERT(m_screen != nullptr, "TaggedPathResolver must have access to Screen system");
            m_screenResizeConnection = m_screen->GetResolutionChangedEvent().OpenConnection(MakeDelegate(this, &TaggedPathResolver::OnScreenResized));
            
            //Initialise the priority to the default
            for(u32 i=0; i<(u32)TagGroup::k_total; ++i)
            {
                m_priorityIndices[i] = i;
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedPathResolver::SetTags(const std::vector<std::string>& in_supportedLanguages,
                                             const std::vector<RangeRule>& in_supportedResolutions,
                                             const std::vector<RangeRule>& in_supportedAspectRatios)
        {
            //---Languages
            auto device = Application::Get()->GetSystem<Device>();
            CS_ASSERT(device != nullptr, "TaggedPathResolver must have access to Device system");
            for(const auto& language : in_supportedLanguages)
            {
                m_groupTags[(u32)TagGroup::k_language].push_back("." + language);
            }
            m_activeTags[(u32)TagGroup::k_language] = "." + device->GetLanguage();
            
            //---Platforms
            m_groupTags[(u32)TagGroup::k_platform] = {".ios", ".android", ".windows"};
#if defined CS_TARGETPLATFORM_IOS
            m_activeTags[(u32)TagGroup::k_platform] = ".ios";
#elif defined CS_TARGETPLATFORM_ANDROID
            m_activeTags[(u32)TagGroup::k_platform] = ".android";
#elif defined CS_TARGETPLATFORM_WINDOWS
            m_activeTags[(u32)TagGroup::k_platform] = ".windows";
#endif
            
            //---Resolution
            m_resolutionRules = in_supportedResolutions;
            for(const auto& rule : in_supportedResolutions)
            {
                m_groupTags[(u32)TagGroup::k_resolution].push_back("." + rule.m_name);
            }
            
            //---Aspect Ratio
            m_aspectRatioRules = in_supportedAspectRatios;
            for(const auto& rule : in_supportedAspectRatios)
            {
                m_groupTags[(u32)TagGroup::k_aspectRatio].push_back("." + rule.m_name);
            }
            
            OnScreenResized(m_screen->GetResolution());
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedPathResolver::SetPriority(TagGroup in_high, TagGroup in_2, TagGroup in_3, TagGroup in_low)
        {
            m_priorityIndices[0] = (u32)in_high;
            m_priorityIndices[1] = (u32)in_2;
            m_priorityIndices[2] = (u32)in_3;
            m_priorityIndices[3] = (u32)in_low;
            
#ifdef DEBUG
            u32 priorityIndicesSorted[(u32)TagGroup::k_total];
            std::copy(m_priorityIndices, m_priorityIndices + (u32)TagGroup::k_total, priorityIndicesSorted);
            std::sort(priorityIndicesSorted, priorityIndicesSorted + (u32)TagGroup::k_total);
            
            u32 limit = (u32)(TagGroup::k_total) - 1;
            for(u32 i=0; i<limit; ++i)
            {
                if(priorityIndicesSorted[i] == priorityIndicesSorted[i+1])
                {
                    CS_LOG_FATAL("TaggedPathResolver::SetPriority: Has a priority conflict. Check for duplicate priorities");
                }
            }
#endif
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedPathResolver::OnScreenResized(const Vector2& in_size)
        {
            //---Resolution
            f32 resolution = in_size.x * in_size.y;
            for(const auto& rule : m_resolutionRules)
            {
                if(resolution > rule.m_minExclusive && resolution <= rule.m_maxInclusive)
                {
                    m_activeTags[(u32)TagGroup::k_resolution] = "." + rule.m_name;
                }
            }
            
            //---Aspect Ratio
            f32 aspectRatio = in_size.x / in_size.y;
            for(const auto& rule : m_aspectRatioRules)
            {
                if(aspectRatio > rule.m_minExclusive && aspectRatio <= rule.m_maxInclusive)
                {
                    m_activeTags[(u32)TagGroup::k_aspectRatio] = "." + rule.m_name;
                }
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        std::string TaggedPathResolver::ResolvePath(StorageLocation in_location, const std::string& in_basePath) const
        {
            std::string filePath, fileName, fileExtension;
            StringUtils::SplitFullFilename(in_basePath, fileName, fileExtension, filePath);
            
            //Look for all files in the given folder with the given name and extension
            std::vector<std::string> pathsContaining = m_fileSystem->GetFilePathsWithExtension(in_location, filePath, false, fileExtension);
            
            //Filter on the filename
            pathsContaining = FilterContains(pathsContaining, fileName + ".");
            
            if(pathsContaining.empty() == true)
            {
                return "";
            }

            //Filter the paths based on the priority tag - if it doesn't contain the tag check to make sure it doesn't have an excluding tag in this group
            //Filter those filtered paths based on the next priority tag
            //Keep going until we have a single path or we have exhausted all tags
            for(u32 tagIndex=0; tagIndex<(u32)TagGroup::k_total; ++tagIndex)
            {
                pathsContaining = FilterContainsExclusive(pathsContaining, m_activeTags[m_priorityIndices[tagIndex]], m_groupTags[m_priorityIndices[tagIndex]]);
            }
            
            //We have exhausted all the tags and now have a list of all the paths applicable to this
            //device. Pick the most applicable path by finding the one that matches the highest priority tag.
            std::vector<std::string> finalPaths = pathsContaining;
            for(u32 tagIndex=0; tagIndex<(u32)TagGroup::k_total; ++tagIndex)
            {
                pathsContaining.clear();
                for(const auto& path : finalPaths)
                {
                    if(path.find(m_activeTags[m_priorityIndices[tagIndex]]) != std::string::npos)
                    {
                        pathsContaining.push_back(path);
                    }
                }
                
                if(pathsContaining.empty() == false)
                {
                    finalPaths = pathsContaining;
                }
            }

            return finalPaths.empty() == false ? filePath + finalPaths[0] : "";
        }
    }
}
