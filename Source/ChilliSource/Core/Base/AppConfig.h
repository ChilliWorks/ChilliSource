//
//  AppConfig.h
//  Chilli Source
//  Created by Ian Copland on 02/06/2014.
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

#ifndef _CHILLISOURCE_CORE_BASE_APPCONFIG_H_
#define _CHILLISOURCE_CORE_BASE_APPCONFIG_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource
{
    namespace Core
    {
        //---------------------------------------------------------------
        /// A system for managing application configuration settings. The
        /// settings are read from an App.config file in the assets
        /// directory. Settings include things like the displayable name
        /// of the project and the preferred FPS.
        ///
        /// @author I Copland
        //---------------------------------------------------------------
        class AppConfig final : public AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(AppConfig);
			//---------------------------------------------------------
            /// Allows querying of whether or not the system implements
            /// the interface with the associated interface Id.
            ///
            /// @author I Copland
            ///
            /// @param The interface Id.
            ///
            /// @return Whether or not the interface is implemented.
            //--------------------------------------------------------
            bool IsA(InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The displayable name of the application.
            //--------------------------------------------------------
            const std::string& GetDisplayableName() const;
            //---------------------------------------------------------
            /// @author I Copland
            ///
            /// @param The prefered FPS.
            //--------------------------------------------------------
            u32 GetPreferredFPS() const;
            //---------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The file path of the default text resource.
            //--------------------------------------------------------
            const std::string& GetDefaultLocalisedTextFilePath() const;
            //---------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The storage location of the default text resource.
            //--------------------------------------------------------
            StorageLocation GetDefaultLocalisedTextStorageLocation() const;
            //---------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The file path of the default font resource.
            //--------------------------------------------------------
            const std::string& GetDefaultFontFilePath() const;
            //---------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The storage location of the default font resource.
            //--------------------------------------------------------
            StorageLocation GetDefaultFontStorageLocation() const;
            //---------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The file path of the default material resource.
            //--------------------------------------------------------
            const std::string& GetDefaultMaterialFilePath() const;
            //---------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The storage location of the default material
            /// resource.
            //--------------------------------------------------------
            StorageLocation GetDefaultMaterialStorageLocation() const;
            //---------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The file path of the default mesh resource.
            //--------------------------------------------------------
            const std::string& GetDefaultMeshFilePath() const;
            //---------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The storage location of the default mesh resource.
            //--------------------------------------------------------
            StorageLocation GetDefaultMeshStorageLocation() const;
        private:
            friend class Application;
            //---------------------------------------------------------
            /// Factory method for creating a new instance of the class.
            /// This will be called by CreateSystem() in Application.
            ///
            /// @author I Copland
            ///
            /// @return New instance of the system.
            //---------------------------------------------------------
            static AppConfigUPtr Create();
            //---------------------------------------------------------
            /// Constructor. Declared private to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            //---------------------------------------------------------
            AppConfig();
            //---------------------------------------------------------
            /// Reads the contents of the app config from file.
            ///
            /// @author I Copland
            //---------------------------------------------------------
            void Load();
            
            std::string m_displayableName;
            u32 m_preferredFPS;
            std::string m_defaultLocalisedTextFilePath;
            StorageLocation m_defaultLocalisedTextStorageLocation;
            std::string m_defaultFontFilePath;
            StorageLocation m_defaultFontStorageLocation;
            std::string m_defaultMaterialFilePath;
            StorageLocation m_defaultMaterialStorageLocation;
            std::string m_defaultMeshFilePath;
            StorageLocation m_defaultMeshStorageLocation;
        };
    }
}

#endif
