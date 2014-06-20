//
//  TaggedFilenameResolver.h
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

#ifndef _CHILLISOURCE_CORE_FILE_TAGGEDFILENAMERESOLVER_H_
#define _CHILLISOURCE_CORE_FILE_TAGGEDFILENAMERESOLVER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource
{
	namespace Core
	{
		//-----------------------------------------------------------------
		/// A system that resolves filenames based on the device configuration.
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
		class TaggedFilenameResolver : public AppSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(TaggedFilenameResolver);
            
            //--------------------------------------------------------------
            /// The list of supported groups by which assets can be loaded
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            enum class TagGroups
            {
                k_language,
                k_platform,
                k_resolution,
                k_aspectRatio
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
            /// Add a new resolution tag and the rule for when to apply it
            /// based on the current device resolution.
            ///
            /// NOTE: Resolution rules should be mutually exclusive otherwise
            /// conflicts will occur (no overlaps).
            ///
            /// @author S Downie
            ///
            /// @param Tag name
            /// @param Minimum inclusive resolution (i.e. 960*640)
            /// @param Maximum exclusive resolution (i.e. 2048*1536)
            //--------------------------------------------------------------
            void AddResolutionTag(const std::string& in_name, u32 in_minResIncl, u32 in_maxResExcl);
            //--------------------------------------------------------------
            /// Add a new aspect ratio tag and the rule for when to apply it
            /// based on the current device aspect ratio.
            ///
            /// NOTE: Aspect ratio rules should be mutually exclusive otherwise
            /// conflicts will occur (no overlaps).
            ///
            /// @author S Downie
            ///
            /// @param Tag name
            /// @param Minimum inclusive ratio (i.e. 3.0/2.0)
            /// @param Maximum exclusive resolution (i.e. 16.0/9.0)
            //--------------------------------------------------------------
            void AddAspectRatioTag(const std::string& in_name, u32 in_minRatioIncl, u32 in_maxRatioExcl);
            //--------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Delimitter that is used to separate tags in file name
            /// (default is '.')
            //--------------------------------------------------------------
            void SetDelimitter(const std::string& in_delimitter);
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
            //--------------------------------------------------------------
            void SetPriority();
            //--------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Storage location
            /// @param Base file path (i.e. Textures/MyImage.png)
            ///
            /// @return The best-fit filename based on the assets in the
            /// filesystem, the tags, the device and the priority (i.e. Textures/MyImage.high.png)
            //--------------------------------------------------------------
            std::string ResolvePath(StorageLocation in_location, const std::string& in_basePath) const;

        private:
            
            friend class Application;
            //--------------------------------------------------------------
            /// Factory create method
            ///
            /// @author S Downie
            ///
            /// @return New backend instance
            //--------------------------------------------------------------
            static TaggedFilenameResolverUPtr Create();
            //--------------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            TaggedFilenameResolver() = default;
            //--------------------------------------------------------------
            /// Called when the system is created
            ///
            /// @author S Downie
            //--------------------------------------------------------------
            void OnInit() override;
            
        private:
            
            std::string m_delimitter = ".";
            
            FileSystem* m_fileSystem = nullptr;
            Screen* m_screen = nullptr;
            Device* m_device = nullptr;
		};
	}
		
}

#endif
