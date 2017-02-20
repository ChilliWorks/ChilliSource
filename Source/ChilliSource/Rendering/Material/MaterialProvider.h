//
//  MaterialProvider.h
//  ChilliSource
//  Created by Scott Downie on 22/11/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_MATERIAL_MATERIALPROVIDER_H_
#define _CHILLISOURCE_RENDERING_MATERIAL_MATERIALPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Rendering/Model/VertexFormat.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
    //-------------------------------------------------------------------------
    /// Factory for creating material resources from material files
    ///
    /// @author S Downie
    //-------------------------------------------------------------------------
    class MaterialProvider final : public ResourceProvider
    {
    public:
        
        CS_DECLARE_NAMEDTYPE(MaterialProvider);
        
        //-------------------------------------------------------------------------
        /// Holds the description of a shader as required by the material
        /// including which vertex format it applies to.
        ///
        /// @author Ian Copland
        //-------------------------------------------------------------------------
        struct ShaderDesc
        {
            std::string m_filePath;
            StorageLocation m_location;
            RenderPasses m_pass;
        };
        //-------------------------------------------------------------------------
        /// Holds the description of a texture as required by the material
        /// including where the texture file is
        ///
        /// @author S Downie
        //-------------------------------------------------------------------------
        struct TextureDesc
        {
            std::string m_filePath;
            StorageLocation m_location;
            bool m_shouldMipMap;
            TextureWrapMode m_wrapModeU;
            TextureWrapMode m_wrapModeV;
            TextureFilterMode m_filterMode;
            TextureType m_type;
        };
        //-------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Interface to compare
        ///
        /// @return Whether the object implements the given interface
        //-------------------------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //----------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @return The resource type this provider creates
        //----------------------------------------------------------------------------
        InterfaceIDType GetResourceType() const override;
        //----------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @param Extension to compare
        ///
        /// @return Whether the object can create a resource with the given extension
        //----------------------------------------------------------------------------
        bool CanCreateResourceWithFileExtension(const std::string& in_extension) const override;
        
    private:
        friend class Application;
        //-------------------------------------------------------------------------
        /// Factory method
        ///
        /// @author S Downie
        ///
        /// @param The render capabilities.
        ///
        /// @return New instance with ownership transferred
        //-------------------------------------------------------------------------
        static MaterialProviderUPtr Create();
        //-------------------------------------------------------
        /// Private constructor to force use of factory method
        ///
        /// @author S Downie
        ///
        /// @param The render capabilities.
        //-------------------------------------------------------
        MaterialProvider() = default;
        //----------------------------------------------------------------------------
        /// Load the material resource from the given material file. Check the
        /// resource load state for success or failure.
        ///
        /// @author S Downie
        ///
        /// @param The storage location to load from
        /// @param File path
        /// @param Options to customise the creation
        /// @param [Out] Resource object
        //----------------------------------------------------------------------------
        void CreateResourceFromFile(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource) override;
        //----------------------------------------------------------------------------
        /// Load the material resource from the given material file on a background
        /// thread. Calls the delegate on completion. Check the
        /// resource load state for success or failure.
        ///
        /// @author S Downie
        ///
        /// @param The storage location to load from
        /// @param File path
        /// @param Options to customise the creation
        /// @param Completion delegate
        /// @param [Out] Resource object
        //----------------------------------------------------------------------------
        void CreateResourceFromFileAsync(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource) override;
        //----------------------------------------------------------------------------
        /// Build Material Task
        ///
        /// @param The storage location to load from
        /// @param File path
        /// @param Completion delegate
        ///
        /// @param [Out] Resource object
        //----------------------------------------------------------------------------
        void BuildMaterialTask(StorageLocation in_location, const std::string& in_filePath, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource);
        //----------------------------------------------------------------------------
        /// Build Material From File
        ///
        /// @param The storage location to load from
        /// @param File path
        /// @param [Out] Shaders to load
        /// @param [Out] Textures to load
        /// @param [Out] Material resource
        ///
        /// @return Whether the resource was loaded 
        //----------------------------------------------------------------------------
        bool BuildMaterialFromFile(StorageLocation in_location, const std::string& in_filePath,
                                   std::vector<ShaderDesc>& out_shaderFiles,
                                   std::vector<TextureDesc>& out_textureFiles,
                                   Material* out_material);
    };
}

#endif
