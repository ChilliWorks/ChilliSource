/*
 *  Material.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 14/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Material/Material.h>

#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Shader/ShaderManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Core/Math/Matrix4x4.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

namespace ChilliSource
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(Material);

		//------------------------------------------------
		/// Constructor
		//------------------------------------------------
		Material::Material() 
        :
        mSrcBlendFunc(AlphaBlend::k_one), mDstBlendFunc(AlphaBlend::k_oneMinusSourceAlpha), meCullFace(CullFace::k_front),
        mAmbient(0.2f, 0.2f, 0.2f, 1.0f), mEmissive(1.0f, 1.0f, 1.0f, 1.0f), mSpecular(1.0f, 1.0f, 1.0f, 0.05f),
        mbIsScissoringEnabled(false), mbIsAlphaBlended(false), mbIsColourWriteEnabled(true), mbIsDepthWriteEnabled(true), mbIsDepthTestEnabled(true), mbIsCullingEnabled(true),
        mbIsCacheValid(false)
		{

		}
        //----------------------------------------------------------
        /// Is Cache Valid
        //----------------------------------------------------------
        bool Material::IsCacheValid() const
        {
            return mbIsCacheValid;
        }
        //----------------------------------------------------------
        /// Is Variable Cache Valid
        //----------------------------------------------------------
        bool Material::IsVariableCacheValid() const
        {
            return mbIsVariableCacheValid;
        }
        //----------------------------------------------------------
        /// Set Cache Valid
        //----------------------------------------------------------
        void Material::SetCacheValid() const
        {
            mbIsCacheValid = true;
            mbIsVariableCacheValid = true;
        }
		//----------------------------------------------------------
		/// Clone
		//----------------------------------------------------------
		MaterialPtr Material::Clone() const
		{
			return MaterialPtr(new Material(*this));
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool Material::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == Material::InterfaceID);
		}
        //----------------------------------------------------------
        /// Set Active Shader Program
        //----------------------------------------------------------
        void Material::SetActiveShaderProgram(ShaderPass inePass)
        {
            mbIsCacheValid = false;
            mpActiveShaderProgram = maShaderPrograms[(u32)inePass];
        }
        //----------------------------------------------------------
        /// Get Active Shader Program
        //----------------------------------------------------------
        const ShaderPtr& Material::GetActiveShaderProgram() const
        {
            return mpActiveShaderProgram;
        }
        //----------------------------------------------------------
        /// Set Shader Program
        //----------------------------------------------------------
        void Material::SetShaderProgram(ShaderPass inePass, const ShaderPtr &inpShaderProgram)
        {
            mbIsCacheValid = false;
            maShaderPrograms[(u32)inePass] = inpShaderProgram;
        }
        //----------------------------------------------------------
        /// Clear Textures
        //----------------------------------------------------------
        void Material::ClearTextures()
        {
            mTextures.clear();
            mbIsCacheValid = false;
        }
		//----------------------------------------------------------
		/// Set Texture
		//----------------------------------------------------------
		void Material::SetTexture(const TexturePtr &inpTexture, u32 inudwIndex)
		{
            if(inpTexture == nullptr)
                return;
            
            if(mTextures.size() > inudwIndex)
            {
                mTextures[inudwIndex] = inpTexture;
            }
            else
            {
                mTextures.push_back(inpTexture);
            }

            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Add Texture
		//----------------------------------------------------------
		void Material::AddTexture(const TexturePtr &inpTexture)
		{
            if(inpTexture == nullptr)
                return;
            
			mTextures.push_back(inpTexture);
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Get Texture
		//----------------------------------------------------------
		const TexturePtr& Material::GetTexture(u32 inudwIndex) const
		{
			if(mTextures.size() <= inudwIndex)
			{
                ITextureManager* pTextureManager = GET_RESOURCE_MANAGER(ITextureManager);
				return pTextureManager->GetDefaultTexture();
			}
			return mTextures[inudwIndex];
		}
		//----------------------------------------------------------
		/// Get Textures
		//----------------------------------------------------------
		const std::vector<TexturePtr>& Material::GetTextures() const
		{
			return mTextures;
		}
        //----------------------------------------------------------
        /// Set Cubemap
        //----------------------------------------------------------
        void Material::SetCubemap(const CubemapPtr &inpCubemap)
        {
            mpCubemap = inpCubemap;
            
            mbIsCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Cubemap
        //----------------------------------------------------------
        const CubemapPtr& Material::GetCubemap() const
        {
            return mpCubemap;
        }
		//----------------------------------------------------------
		/// Is Transparent
		//----------------------------------------------------------
		bool Material::IsTransparent() const
		{
			return mbIsAlphaBlended;
		}
		//----------------------------------------------------------
		/// Set Transparent
		//----------------------------------------------------------
		void Material::SetTransparent(bool inbIsTransparent)
		{
			mbIsAlphaBlended = inbIsTransparent;
			mbIsDepthWriteEnabled = !inbIsTransparent;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Is Colour Write Enabled
		//----------------------------------------------------------
		bool Material::IsColourWriteEnabled() const
		{
			return mbIsColourWriteEnabled;
		}
		//----------------------------------------------------------
		/// Set Colour Write Enabled
		//----------------------------------------------------------
		void Material::SetColourWriteEnabled(bool inbIsColourhWrite)
		{
			mbIsColourWriteEnabled = inbIsColourhWrite;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Is Depth Write Enabled 
		//----------------------------------------------------------
		bool Material::IsDepthWriteEnabled() const
		{
			return mbIsDepthWriteEnabled;
		}
		//----------------------------------------------------------
		/// Set Depth Write Enabled
		//----------------------------------------------------------
		void Material::SetDepthWriteEnabled(bool inbIsDepthWrite)
		{
			mbIsDepthWriteEnabled = inbIsDepthWrite;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Is Depth Test Enabled
		//----------------------------------------------------------
		bool Material::IsDepthTestEnabled() const
		{
			return mbIsDepthTestEnabled;
		}
		//----------------------------------------------------------
		/// Set Depth Test Enabled
		//----------------------------------------------------------
		void Material::SetDepthTestEnabled(bool inbIsDepthTest)
		{
			mbIsDepthTestEnabled = inbIsDepthTest;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Is Culling Enabled
		//----------------------------------------------------------
		bool Material::IsCullingEnabled() const
		{
			return mbIsCullingEnabled;
		}
		//----------------------------------------------------------
		/// Set Culling Enabled
		//----------------------------------------------------------
		void Material::SetCullingEnabled(bool inbCullingEnabled)
		{
			mbIsCullingEnabled = inbCullingEnabled;
            
            mbIsCacheValid = false;
		}
        //----------------------------------------------------------
        /// Is Scissoring Enabled
        //----------------------------------------------------------
        bool Material::IsScissoringEnabled() const
        {
            return mbIsScissoringEnabled;
        }
        //----------------------------------------------------------
        /// Set Scissoring Enabled
        //----------------------------------------------------------
        void Material::SetScissoringEnabled(bool inbEnabled)
        {
            mbIsScissoringEnabled = inbEnabled;
            
            mbIsCacheValid = false;
        }
        //----------------------------------------------------------
        /// Set Scissoring Region
        //----------------------------------------------------------
        void Material::SetScissoringRegion(const Core::CVector2& invPosition, const Core::CVector2& invSize)
        {
            mvScissorPos = invPosition;
            mvScissorSize = invSize;
            
            mbIsCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Scissoring Region Position
        //----------------------------------------------------------
        const Core::CVector2& Material::GetScissoringRegionPosition() const
        {
            return mvScissorPos;
        }
        //----------------------------------------------------------
        /// Get Scissoring Region Size
        //----------------------------------------------------------
        const Core::CVector2& Material::GetScissoringRegionSize() const
        {
            return mvScissorSize;
        }
		//----------------------------------------------------------
		/// Set Blend Function
		//----------------------------------------------------------
		void Material::SetBlendFunction(AlphaBlend ineSource, AlphaBlend ineDest)
		{
			mSrcBlendFunc = ineSource;
			mDstBlendFunc = ineDest;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Get Source Blend Function
		//----------------------------------------------------------
		AlphaBlend Material::GetSourceBlendFunction() const
		{
			return mSrcBlendFunc;
		}
		//----------------------------------------------------------
		/// Get Destination Blend Function
		//----------------------------------------------------------
		AlphaBlend Material::GetDestBlendFunction() const
		{
			return mDstBlendFunc;
		}
        //----------------------------------------------------------
        /// Set Cull Face
        //----------------------------------------------------------
        void Material::SetCullFace(CullFace ineCullFace)
        {
            meCullFace = ineCullFace;
        }
        //----------------------------------------------------------
        /// Get Cull Face
        //----------------------------------------------------------
        CullFace Material::GetCullFace() const
        {
            return meCullFace;
        }
		//----------------------------------------------------------
		/// Set Emissive
		//----------------------------------------------------------
		void Material::SetEmissive(const Core::CColour& inEmissive)
		{
			mEmissive = inEmissive;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Get Emissive
		//----------------------------------------------------------
		const Core::CColour& Material::GetEmissive() const
		{
			return mEmissive;
		}
        //----------------------------------------------------------
		/// Set Ambient
		//----------------------------------------------------------
		void Material::SetAmbient(const Core::CColour& inAmbient)
		{
			mAmbient = inAmbient;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Get Ambient
		//----------------------------------------------------------
		const Core::CColour& Material::GetAmbient() const
		{
			return mAmbient;
		}
		//----------------------------------------------------------
		/// Set Diffuse
		//----------------------------------------------------------
		void Material::SetDiffuse(const Core::CColour& inDiffuse)
		{
			mDiffuse = inDiffuse;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Get Diffuse
		//----------------------------------------------------------
		const Core::CColour& Material::GetDiffuse() const
		{
			return mDiffuse;
		}
		//----------------------------------------------------------
		/// Set Specular
		//----------------------------------------------------------
		void Material::SetSpecular(const Core::CColour& inSpecular)
		{
			mSpecular = inSpecular;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Get Specular
		//----------------------------------------------------------
		const Core::CColour& Material::GetSpecular() const
		{
			return mSpecular;
		}
		//-----------------------------------------------------------
		/// Set Shader Float Value
		//-----------------------------------------------------------
		void Material::SetShaderFloatValue(const std::string& instrVarName, f32 infValue)
		{
            mMapFloatShaderVars[instrVarName] = infValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set Shader Vec2 Value
		//-----------------------------------------------------------
		void Material::SetShaderVec2Value(const std::string& instrVarName, const Core::CVector2 &invValue)
		{
			mMapVec2ShaderVars[instrVarName] = invValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set Shader Vec3 Value
		//-----------------------------------------------------------
		void Material::SetShaderVec3Value(const std::string& instrVarName, const Core::CVector3 &invValue)
		{
			mMapVec3ShaderVars[instrVarName] = invValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set Shader Vec4 Value
		//-----------------------------------------------------------
		void Material::SetShaderVec4Value(const std::string& instrVarName, const Core::CVector4 &invValue)
		{
            mMapVec4ShaderVars[instrVarName] = invValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set Shader Matrix Value
		//-----------------------------------------------------------
		void Material::SetShaderMatrixValue(const std::string& instrVarName, const Core::CMatrix4x4 &inmatValue)
		{
			mMapMat4ShaderVars[instrVarName] = inmatValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set Shader Matrix Array Value
		//-----------------------------------------------------------
		void Material::SetShaderMatrixArrayValue(const std::string& instrVarName, const std::vector<Core::CMatrix4x4>& inmatValue)
		{
			mMapMat4ArrayShaderVars[instrVarName] = inmatValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set Shader Colour Value
		//-----------------------------------------------------------
		void Material::SetShaderColourValue(const std::string& instrVarName, const Core::CColour &incolValue)
		{
            mMapColShaderVars[instrVarName] = incolValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
	}
}

