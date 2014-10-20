//
//  TaggedFilePathResolver.h
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

#ifndef _CHILLISOURCE_CORE_FILE_TAGGEDFILEPATHRESOLVER_H_
#define _CHILLISOURCE_CORE_FILE_TAGGEDFILEPATHRESOLVER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <json/json.h>

namespace ChilliSource
{
	namespace Core
	{
		//-----------------------------------------------------------------
		/// A system that resolves file paths based on the device configuration.
        ///
        /// Devices have the following configs:
        ///     * Language
        ///     * Platform
        ///     * Resolution
        ///     * Aspect ratio
        ///
        /// Each config has associated tags and rules that can be applied
        /// to filenames. This system will return the path that best
        /// suits the current device based on the given base file path and
        /// the assets that are currently on device.
        ///
        /// Example:
        ///
        /// MyImage.png
        /// MyImage.en.png
        /// MyImage.high.wide.en.png
        /// MyImage.med.low.png
        ///
        /// NOTE: Only the rules and tags for resolution and ratio are
        /// configurable as the platform and language are fixed.
        ///
        /// @author S Downie
		//-----------------------------------------------------------------
		class TaggedFilePathResolver : public AppSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(TaggedFilePathResolver);
            
            //--------------------------------------------------------------
            /// The list of supported groups by which assets can be loaded
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            enum class TagGroup
            {
                k_platform,
                k_resolution,
                k_aspectRatio,
                k_language,
                k_total
            };
            //--------------------------------------------------------------
            /// Holds the information for a range rule. A range rule
            /// holds true for any value greater than the minimum
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            struct RangeRule
            {
                //--------------------------------------------------------------
                /// Constructor
                ///
                /// @author S Downie
                ///
                /// @param Tag name
                /// @param Min exclusive
                //--------------------------------------------------------------
                RangeRule(const std::string& in_name, f32 in_minExclusive)
                : m_name(in_name), m_minExclusive(in_minExclusive)
                {
                    
                }
                std::string m_name;
                f32 m_minExclusive = 0;
            };
            //--------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface Id
            ///
            /// @return Whether this object is of the given type
            //--------------------------------------------------------------
            bool IsA(InterfaceIDType in_interfaceId) const override;
            //--------------------------------------------------------------
            /// Add the tags and the rules for when to apply them
            /// based on the current device resolution, platform, language
            /// and aspect ratio. Set the priority.
            ///
            /// @author S Downie
            ///
            /// @param Json object
            //--------------------------------------------------------------
            void SetFromJson(const Json::Value& in_json);
            //--------------------------------------------------------------
            /// Add the tags and the rules for when to apply them
            /// based on the current device resolution, platform, language
            /// and aspect ratio.
            ///
            /// @author S Downie
            ///
            /// @param List of supported language codes i.e. {"en", "fr", "de"}
            /// @param List of supported resolutions i.e. {"low", 0*0, 480*320}, {"medium", 480*320, 960*640}
            /// @param List of supported aspect ratios i.e. {"std", 1/1, 3/2}, {"wide", 16/9, 25/9}
            //--------------------------------------------------------------
            void SetTags(const std::vector<std::string>& in_supportedLanguages,
                         const std::vector<RangeRule>& in_supportedResolutions,
                         const std::vector<RangeRule>& in_supportedAspectRatios);
            //--------------------------------------------------------------
            /// Set the priority in which the tag groups are applied. This
            /// is used to manage conflicts between shared assets.
            ///
            /// Example:
            ///     MyImage.wide.png
            ///     MyImage.high.png
            ///
            /// The priority governs which is loaded on a wide and high device.
            ///
            /// Default is: Platform, Res, Ratio, Language
            ///
            /// @author S Downie
            ///
            /// @param Highest priority
            /// @param 2nd highest
            /// @param 3rd highest
            /// @param Lowest
            //--------------------------------------------------------------
            void SetPriority(TagGroup in_high, TagGroup in_2, TagGroup in_3, TagGroup in_low);
            //--------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Storage location
            /// @param Base file path (i.e. Textures/MyImage.png)
            ///
            /// @return The best-fit filename based on the assets in the
            /// filesystem, the tags, the device and the priority (i.e. Textures/MyImage.high.png)
            //--------------------------------------------------------------
            std::string ResolveFilePath(StorageLocation in_location, const std::string& in_basePath) const;
            //--------------------------------------------------------------
            /// Returns the active tag for the given Tag Group.
            ///
            /// @author Ian Copland
            ///
            /// @param The tag group for which to get the active tag.
            ///
            /// @return The active tag.
            //--------------------------------------------------------------
            std::string GetActiveTag(TagGroup in_tagGroup) const;
        private:
            
            friend class Application;
            //--------------------------------------------------------------
            /// Factory create method
            ///
            /// @author S Downie
            ///
            /// @return New backend instance
            //--------------------------------------------------------------
            static TaggedFilePathResolverUPtr Create();
            //--------------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            TaggedFilePathResolver() = default;
            //--------------------------------------------------------------
            /// Called when the system is created
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            void OnInit() override;
            //--------------------------------------------------------------
            /// Assigns the correct tags based on the screen size
            ///
            /// @author S Downie
            ///
            /// @param Screen size
            //--------------------------------------------------------------
            void DetermineScreenDependentTags(const Vector2& in_size);
            
        private:
            
            FileSystem* m_fileSystem = nullptr;
            Screen* m_screen = nullptr;
            
            std::vector<RangeRule> m_resolutionRules;
            std::vector<RangeRule> m_aspectRatioRules;
            
            std::vector<std::string> m_groupTags[(u32)TagGroup::k_total];
            std::string m_activeTags[(u32)TagGroup::k_total];
            
            u32 m_priorityIndices[(u32)TagGroup::k_total];
		};
	}
		
}

#endif
