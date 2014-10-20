//
//  TaggedFilePathResolver.cpp
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

#include <ChilliSource/Core/File/TaggedFilePathResolver.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <algorithm>

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
            //--------------------------------------------------------------
            /// Filter the list of file paths based on those that contain
            /// the names that start with the given text
            ///
            /// @author S Downie
            ///
            /// @param List to filter
            /// @param text
            ///
            /// @return Filtered list
            //--------------------------------------------------------------
            std::vector<std::string> FilterFileNameStartsWith(const std::vector<std::string>& in_list, const std::string& in_text)
            {
                std::vector<std::string> result;
                result.reserve(in_list.size());
                
                for(const auto& string : in_list)
                {
                    auto findIndex = string.find(in_text);
                    if(findIndex == 0 || (findIndex != std::string::npos && string[findIndex-1] == '/'))
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
            //--------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Case insensitive string representing tag group
            ///
            /// @return Tag group enum
            //--------------------------------------------------------------
            TaggedFilePathResolver::TagGroup ParseGroup(const std::string& in_group)
            {
                std::string lowercaseGroup = in_group;
                StringUtils::ToLowerCase(lowercaseGroup);
                
                if(lowercaseGroup == "language")
                {
                    return TaggedFilePathResolver::TagGroup::k_language;
                }
                
                if(lowercaseGroup == "platform")
                {
                    return TaggedFilePathResolver::TagGroup::k_platform;
                }
                
                if(lowercaseGroup == "resolution")
                {
                    return TaggedFilePathResolver::TagGroup::k_resolution;
                }
                
                if(lowercaseGroup == "aspectratio")
                {
                    return TaggedFilePathResolver::TagGroup::k_aspectRatio;
                }
                
                CS_LOG_FATAL("TaggedFilePathResolver: No such group: " + in_group);
                return TaggedFilePathResolver::TagGroup::k_platform;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(TaggedFilePathResolver);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        TaggedFilePathResolverUPtr TaggedFilePathResolver::Create()
        {
            return TaggedFilePathResolverUPtr(new TaggedFilePathResolver());
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool TaggedFilePathResolver::IsA(InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == InterfaceID;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedFilePathResolver::OnInit()
        {
            m_fileSystem = Application::Get()->GetFileSystem();
            CS_ASSERT(m_fileSystem != nullptr, "TaggedFilePathResolver must have access to FileSystem");
            m_screen = Application::Get()->GetSystem<Screen>();
            CS_ASSERT(m_screen != nullptr, "TaggedFilePathResolver must have access to Screen system");

            //Initialise the priority to the default
            for(u32 i=0; i<(u32)TagGroup::k_total; ++i)
            {
                m_priorityIndices[i] = i;
            }
            
            DetermineScreenDependentTags(m_screen->GetResolution());
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedFilePathResolver::SetFromJson(const Json::Value& in_json)
        {
            std::vector<std::string> supportedLanguages;
            const Json::Value& languages = in_json["Languages"];
            if(languages.isNull() == false)
            {
                for(u32 i=0; i<languages.size(); ++i)
                {
                    supportedLanguages.push_back(languages[i].asString());
                }
            }
            
            std::vector<RangeRule> supportedResolutions;
            const Json::Value& resolutions = in_json["Resolutions"];
            if(resolutions.isNull() == false)
            {
                for(auto it = resolutions.begin(); it != resolutions.end(); ++it)
                {
                    Vector2 res = ParseVector2((*it).asString());
                    RangeRule rule(it.memberName(), res.x * res.y);
                    supportedResolutions.push_back(rule);
                }
            }
            
            std::vector<RangeRule> supportedAspectRatios;
            const Json::Value& aspects = in_json["AspectRatios"];
            if(aspects.isNull() == false)
            {
                for(auto it = aspects.begin(); it != aspects.end(); ++it)
                {
                    RangeRule rule(it.memberName(), (f32)(*it).asDouble());
                    supportedAspectRatios.push_back(rule);
                }
            }
            
            SetTags(supportedLanguages, supportedResolutions, supportedAspectRatios);
            
            TagGroup priorityIndices[(u32)TagGroup::k_total];
            const Json::Value& priorities = in_json["Priorities"];
            if(priorities.isNull() == false)
            {
                CS_ASSERT(priorities.size() == (u32)TagGroup::k_total, "TaggedFilePathResolver: Need to specify all groups when setting priorities");
                for(u32 i=0; i<priorities.size(); ++i)
                {
                    priorityIndices[i] = ParseGroup(priorities[i].asString());
                }
            }
            
            SetPriority(priorityIndices[0], priorityIndices[1], priorityIndices[2], priorityIndices[3]);
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedFilePathResolver::SetTags(const std::vector<std::string>& in_supportedLanguages,
                                         const std::vector<RangeRule>& in_supportedResolutions,
                                         const std::vector<RangeRule>& in_supportedAspectRatios)
        {
            //---Languages
            auto device = Application::Get()->GetSystem<Device>();
            CS_ASSERT(device != nullptr, "TaggedFilePathResolver must have access to Device system");
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
            std::sort(std::begin(m_resolutionRules), std::end(m_resolutionRules), [](const RangeRule& in_a, const RangeRule& in_b)
            {
                return in_a.m_minExclusive > in_b.m_minExclusive;
            });
            
            for(const auto& rule : in_supportedResolutions)
            {
                m_groupTags[(u32)TagGroup::k_resolution].push_back("." + rule.m_name);
            }
            
            //---Aspect Ratio
            m_aspectRatioRules = in_supportedAspectRatios;
            std::sort(std::begin(m_aspectRatioRules), std::end(m_aspectRatioRules), [](const RangeRule& in_a, const RangeRule& in_b)
            {
                return in_a.m_minExclusive > in_b.m_minExclusive;
            });
            
            for(const auto& rule : in_supportedAspectRatios)
            {
                m_groupTags[(u32)TagGroup::k_aspectRatio].push_back("." + rule.m_name);
            }
            
            if(m_screen != nullptr)
            {
                DetermineScreenDependentTags(m_screen->GetResolution());
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedFilePathResolver::SetPriority(TagGroup in_high, TagGroup in_2, TagGroup in_3, TagGroup in_low)
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
                    CS_LOG_FATAL("TaggedFilePathResolver::SetPriority: Has a priority conflict. Check for duplicate priorities");
                }
            }
#endif
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void TaggedFilePathResolver::DetermineScreenDependentTags(const Vector2& in_size)
        {
            //---Resolution
            f32 resolution = in_size.x * in_size.y;
            for(const auto& rule : m_resolutionRules)
            {
                if(resolution > rule.m_minExclusive)
                {
                    m_activeTags[(u32)TagGroup::k_resolution] = "." + rule.m_name;
                    break;
                }
            }
            
            //---Aspect Ratio
            f32 aspectRatio = in_size.x / in_size.y;
            for(const auto& rule : m_aspectRatioRules)
            {
                if(aspectRatio > rule.m_minExclusive)
                {
                    m_activeTags[(u32)TagGroup::k_aspectRatio] = "." + rule.m_name;
                    break;
                }
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        std::string TaggedFilePathResolver::ResolveFilePath(StorageLocation in_location, const std::string& in_basePath) const
        {
            std::string filePath, fileName, fileExtension;
            StringUtils::SplitFullFilename(in_basePath, fileName, fileExtension, filePath);
            
            //Look for all files in the given folder with the given name and extension
            std::vector<std::string> pathsContaining = m_fileSystem->GetFilePathsWithExtension(in_location, filePath, false, fileExtension);
            
            //Filter on the filename
            pathsContaining = FilterFileNameStartsWith(pathsContaining, fileName + ".");
            
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
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        std::string TaggedFilePathResolver::GetActiveTag(TagGroup in_tagGroup) const
        {
            std::string output = m_activeTags[static_cast<u32>(in_tagGroup)];
            
            if (output.empty() == false)
            {
                if (output[0] == '.')
                {
                    output = output.substr(1);
                }
            }
            
            return output;
        }
    }
}
