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
		DEFINE_NAMED_INTERFACE(CMaterial);

		//------------------------------------------------
		/// Constructor
		//------------------------------------------------
		CMaterial::CMaterial() 
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
        bool CMaterial::IsCacheValid() const
        {
            return mbIsCacheValid;
        }
        //----------------------------------------------------------
        /// Is Variable Cache Valid
        //----------------------------------------------------------
        bool CMaterial::IsVariableCacheValid() const
        {
            return mbIsVariableCacheValid;
        }
        //----------------------------------------------------------
        /// Set Cache Valid
        //----------------------------------------------------------
        void CMaterial::SetCacheValid() const
        {
            mbIsCacheValid = true;
            mbIsVariableCacheValid = true;
        }
		//----------------------------------------------------------
		/// Clone
		//----------------------------------------------------------
		MaterialPtr CMaterial::Clone() const
		{
			return MaterialPtr(new CMaterial(*this));
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool CMaterial::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CMaterial::InterfaceID);
		}
        //----------------------------------------------------------
        /// Set Active Shader Program
        //----------------------------------------------------------
        void CMaterial::SetActiveShaderProgram(ShaderPass inePass)
        {
            mbIsCacheValid = false;
            mpActiveShaderProgram = maShaderPrograms[(u32)inePass];
        }
        //----------------------------------------------------------
        /// Get Active Shader Program
        //----------------------------------------------------------
        const ShaderPtr& CMaterial::GetActiveShaderProgram() const
        {
            return mpActiveShaderProgram;
        }
        //----------------------------------------------------------
        /// Set Shader Program
        //----------------------------------------------------------
        void CMaterial::SetShaderProgram(ShaderPass inePass, const ShaderPtr &inpShaderProgram)
        {
            mbIsCacheValid = false;
            maShaderPrograms[(u32)inePass] = inpShaderProgram;
        }
        //----------------------------------------------------------
        /// Clear Textures
        //----------------------------------------------------------
        void CMaterial::ClearTextures()
        {
            mTextures.clear();
            mbIsCacheValid = false;
        }
		//----------------------------------------------------------
		/// Set Texture
		//----------------------------------------------------------
		void CMaterial::SetTexture(const TexturePtr &inpTexture, u32 inudwIndex)
		{
            if(inpTexture == NULL)
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
		void CMaterial::AddTexture(const TexturePtr &inpTexture)
		{
            if(inpTexture == NULL)
                return;
            
			mTextures.push_back(inpTexture);
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Get Texture
		//----------------------------------------------------------
		const TexturePtr& CMaterial::GetTexture(u32 inudwIndex) const
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
		const DYNAMIC_ARRAY<TexturePtr>& CMaterial::GetTextures() const
		{
			return mTextures;
		}
        //----------------------------------------------------------
        /// Set Cubemap
        //----------------------------------------------------------
        void CMaterial::SetCubemap(const CubemapPtr &inpCubemap)
        {
            mpCubemap = inpCubemap;
            
            mbIsCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Cubemap
        //----------------------------------------------------------
        const CubemapPtr& CMaterial::GetCubemap() const
        {
            return mpCubemap;
        }
		//----------------------------------------------------------
		/// Is Transparent
		//----------------------------------------------------------
		bool CMaterial::IsTransparent() const
		{
			return mbIsAlphaBlended;
		}
		//----------------------------------------------------------
		/// Set Transparent
		//----------------------------------------------------------
		void CMaterial::SetTransparent(bool inbIsTransparent)
		{
			mbIsAlphaBlended = inbIsTransparent;
			mbIsDepthWriteEnabled = !inbIsTransparent;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Is Colour Write Enabled
		//----------------------------------------------------------
		bool CMaterial::IsColourWriteEnabled() const
		{
			return mbIsColourWriteEnabled;
		}
		//----------------------------------------------------------
		/// Set Colour Write Enabled
		//----------------------------------------------------------
		void CMaterial::SetColourWriteEnabled(bool inbIsColourhWrite)
		{
			mbIsColourWriteEnabled = inbIsColourhWrite;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Is Depth Write Enabled 
		//----------------------------------------------------------
		bool CMaterial::IsDepthWriteEnabled() const
		{
			return mbIsDepthWriteEnabled;
		}
		//----------------------------------------------------------
		/// Set Depth Write Enabled
		//----------------------------------------------------------
		void CMaterial::SetDepthWriteEnabled(bool inbIsDepthWrite)
		{
			mbIsDepthWriteEnabled = inbIsDepthWrite;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Is Depth Test Enabled
		//----------------------------------------------------------
		bool CMaterial::IsDepthTestEnabled() const
		{
			return mbIsDepthTestEnabled;
		}
		//----------------------------------------------------------
		/// Set Depth Test Enabled
		//----------------------------------------------------------
		void CMaterial::SetDepthTestEnabled(bool inbIsDepthTest)
		{
			mbIsDepthTestEnabled = inbIsDepthTest;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Is Culling Enabled
		//----------------------------------------------------------
		bool CMaterial::IsCullingEnabled() const
		{
			return mbIsCullingEnabled;
		}
		//----------------------------------------------------------
		/// Set Culling Enabled
		//----------------------------------------------------------
		void CMaterial::SetCullingEnabled(bool inbCullingEnabled)
		{
			mbIsCullingEnabled = inbCullingEnabled;
            
            mbIsCacheValid = false;
		}
        //----------------------------------------------------------
        /// Is Scissoring Enabled
        //----------------------------------------------------------
        bool CMaterial::IsScissoringEnabled() const
        {
            return mbIsScissoringEnabled;
        }
        //----------------------------------------------------------
        /// Set Scissoring Enabled
        //----------------------------------------------------------
        void CMaterial::SetScissoringEnabled(bool inbEnabled)
        {
            mbIsScissoringEnabled = inbEnabled;
            
            mbIsCacheValid = false;
        }
        //----------------------------------------------------------
        /// Set Scissoring Region
        //----------------------------------------------------------
        void CMaterial::SetScissoringRegion(const Core::CVector2& invPosition, const Core::CVector2& invSize)
        {
            mvScissorPos = invPosition;
            mvScissorSize = invSize;
            
            mbIsCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Scissoring Region Position
        //----------------------------------------------------------
        const Core::CVector2& CMaterial::GetScissoringRegionPosition() const
        {
            return mvScissorPos;
        }
        //----------------------------------------------------------
        /// Get Scissoring Region Size
        //----------------------------------------------------------
        const Core::CVector2& CMaterial::GetScissoringRegionSize() const
        {
            return mvScissorSize;
        }
		//----------------------------------------------------------
		/// Set Blend Function
		//----------------------------------------------------------
		void CMaterial::SetBlendFunction(AlphaBlend ineSource, AlphaBlend ineDest)
		{
			mSrcBlendFunc = ineSource;
			mDstBlendFunc = ineDest;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Get Source Blend Function
		//----------------------------------------------------------
		AlphaBlend CMaterial::GetSourceBlendFunction() const
		{
			return mSrcBlendFunc;
		}
		//----------------------------------------------------------
		/// Get Destination Blend Function
		//----------------------------------------------------------
		AlphaBlend CMaterial::GetDestBlendFunction() const
		{
			return mDstBlendFunc;
		}
        //----------------------------------------------------------
        /// Set Cull Face
        //----------------------------------------------------------
        void CMaterial::SetCullFace(CullFace ineCullFace)
        {
            meCullFace = ineCullFace;
        }
        //----------------------------------------------------------
        /// Get Cull Face
        //----------------------------------------------------------
        CullFace CMaterial::GetCullFace() const
        {
            return meCullFace;
        }
		//----------------------------------------------------------
		/// Set Emissive
		//----------------------------------------------------------
		void CMaterial::SetEmissive(const Core::CColour& inEmissive)
		{
			mEmissive = inEmissive;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Get Emissive
		//----------------------------------------------------------
		const Core::CColour& CMaterial::GetEmissive() const
		{
			return mEmissive;
		}
        //----------------------------------------------------------
		/// Set Ambient
		//----------------------------------------------------------
		void CMaterial::SetAmbient(const Core::CColour& inAmbient)
		{
			mAmbient = inAmbient;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Get Ambient
		//----------------------------------------------------------
		const Core::CColour& CMaterial::GetAmbient() const
		{
			return mAmbient;
		}
		//----------------------------------------------------------
		/// Set Diffuse
		//----------------------------------------------------------
		void CMaterial::SetDiffuse(const Core::CColour& inDiffuse)
		{
			mDiffuse = inDiffuse;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Get Diffuse
		//----------------------------------------------------------
		const Core::CColour& CMaterial::GetDiffuse() const
		{
			return mDiffuse;
		}
		//----------------------------------------------------------
		/// Set Specular
		//----------------------------------------------------------
		void CMaterial::SetSpecular(const Core::CColour& inSpecular)
		{
			mSpecular = inSpecular;
            
            mbIsCacheValid = false;
		}
		//----------------------------------------------------------
		/// Get Specular
		//----------------------------------------------------------
		const Core::CColour& CMaterial::GetSpecular() const
		{
			return mSpecular;
		}
		//-----------------------------------------------------------
		/// Set Shader Float Value
		//-----------------------------------------------------------
		void CMaterial::SetShaderFloatValue(const std::string& instrVarName, f32 infValue)
		{
            mMapFloatShaderVars[instrVarName] = infValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set Shader Vec2 Value
		//-----------------------------------------------------------
		void CMaterial::SetShaderVec2Value(const std::string& instrVarName, const Core::CVector2 &invValue)
		{
			mMapVec2ShaderVars[instrVarName] = invValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set Shader Vec3 Value
		//-----------------------------------------------------------
		void CMaterial::SetShaderVec3Value(const std::string& instrVarName, const Core::CVector3 &invValue)
		{
			mMapVec3ShaderVars[instrVarName] = invValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set Shader Vec4 Value
		//-----------------------------------------------------------
		void CMaterial::SetShaderVec4Value(const std::string& instrVarName, const Core::CVector4 &invValue)
		{
            mMapVec4ShaderVars[instrVarName] = invValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set Shader Matrix Value
		//-----------------------------------------------------------
		void CMaterial::SetShaderMatrixValue(const std::string& instrVarName, const Core::CMatrix4x4 &inmatValue)
		{
			mMapMat4ShaderVars[instrVarName] = inmatValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set Shader Matrix Array Value
		//-----------------------------------------------------------
		void CMaterial::SetShaderMatrixArrayValue(const std::string& instrVarName, const DYNAMIC_ARRAY<Core::CMatrix4x4>& inmatValue)
		{
			mMapMat4ArrayShaderVars[instrVarName] = inmatValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
		//-----------------------------------------------------------
		/// Set Shader Colour Value
		//-----------------------------------------------------------
		void CMaterial::SetShaderColourValue(const std::string& instrVarName, const Core::CColour &incolValue)
		{
            mMapColShaderVars[instrVarName] = incolValue;
            
            mbIsCacheValid = false;
            mbIsVariableCacheValid = false;
		}
	}
}

