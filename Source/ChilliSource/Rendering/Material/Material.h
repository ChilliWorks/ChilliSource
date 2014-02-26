/*
 *  Material.h
 *  moFlo
 *
 *  Created by Scott Downie on 14/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_MATERIAL_H_
#define _MO_FLO_RENDERING_MATERIAL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Shader/Shader.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Math/Vector2.h>

#include <unordered_map>

namespace ChilliSource
{
	namespace Rendering
	{
		typedef std::unordered_map<std::string, f32> MapStringToFloat;
		typedef std::unordered_map<std::string, Core::CVector2> MapStringToVec2;
		typedef std::unordered_map<std::string, Core::CVector3> MapStringToVec3;
		typedef std::unordered_map<std::string, Core::CVector4> MapStringToVec4;
		typedef std::unordered_map<std::string, Core::CMatrix4x4> MapStringToMat4;
		typedef std::unordered_map<std::string, std::vector<Core::CMatrix4x4> > MapStringToMat4Array;
		typedef std::unordered_map<std::string, Core::CColour> MapStringToCol;
        
        //============================================
		/// Alpha Blend
		///
		/// Destination and source types for defining
		/// blend functions
		//============================================
		enum class AlphaBlend
		{
			k_unknown               = -1,
			k_zero					= (1 << 0),
			k_one					= (1 << 1),
			k_sourceCol				= (1 << 2),
			k_oneMinusSourceCol		= (1 << 3),
			k_sourceAlpha			= (1 << 4),
			k_oneMinusSourceAlpha	= (1 << 5),
			k_destAlpha				= (1 << 6),
			k_oneMinusDestAlpha		= (1 << 7),
		};
        
        //============================================
		/// Cull Face
		//============================================
		enum class CullFace
		{
            k_front,
            k_back
		};
        //============================================
		/// Depth Function
		//============================================
        enum class DepthFunction
        {
            k_less,
            k_equal,
            k_lequal
        };
        //============================================
		/// Shader Pass
		//============================================
        enum class ShaderPass
        {
            k_ambient,
            k_directional,
            k_point,
            k_total
        };
		
		class CMaterial : public Core::IResource
		{
		public:
			DECLARE_NAMED_INTERFACE(CMaterial);
		
            CMaterial();
			
			//----------------------------------------------------------
			/// Clone
			///
			/// Make a deep copy of the material
			///
			/// @return Cloned instance
			//----------------------------------------------------------
			MaterialPtr Clone() const;
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //----------------------------------------------------------
			/// Set Active Shader Program
			///
			/// @param Shader pass type
			//----------------------------------------------------------
			void SetActiveShaderProgram(ShaderPass inePass);
			//----------------------------------------------------------
			/// Get Active Shader Program
			///
			/// @return Shader resource for rendering with 
			//----------------------------------------------------------
			const ShaderPtr& GetActiveShaderProgram() const;
            //----------------------------------------------------------
			/// Set Shader Program
			///
			/// @param Shader resource
			//----------------------------------------------------------
			void SetShaderProgram(ShaderPass inePass, const ShaderPtr &inpShaderProgram);
            //----------------------------------------------------------
			/// Clear Textures
			//----------------------------------------------------------
			void ClearTextures();
			//----------------------------------------------------------
			/// Set Texture
			///
			/// @param Texture 
			/// @param At index
			//----------------------------------------------------------
			void SetTexture(const TexturePtr &inpTexture, u32 inudwIndex = 0);
			//----------------------------------------------------------
			/// Add Texture
			///
			/// @param Texture 
			//----------------------------------------------------------
			void AddTexture(const TexturePtr &inpTexture);
			//----------------------------------------------------------
			/// Get Texture
			///
			/// @param Index to check
			/// @return Texture 
			//----------------------------------------------------------
			const TexturePtr& GetTexture(u32 inudwIndex = 0) const;
			//----------------------------------------------------------
			/// Get Textures
			///
			/// @return Texture array
			//----------------------------------------------------------
			const std::vector<TexturePtr>& GetTextures() const;
            //----------------------------------------------------------
			/// Set Cubemap
			///
			/// @param Cubemap
			//----------------------------------------------------------
			void SetCubemap(const CubemapPtr &inpCubemap);
			//----------------------------------------------------------
			/// Get Cubemap
			///
			/// @return Cubemap
			//----------------------------------------------------------
			const CubemapPtr& GetCubemap() const;
			//----------------------------------------------------------
			/// Is Transparent
			///
			/// @return Whether the object has transparency enabled 
			//----------------------------------------------------------
			bool IsTransparent() const;
			//----------------------------------------------------------
			/// Set Transparent
			///
			/// @param Whether transparency is enabled
			//----------------------------------------------------------
			void SetTransparent(bool inbIsTransparent);
            //----------------------------------------------------------
			/// Is Colour Write Enabled
			///
			/// @return Whether the object has depth write enabled 
			//----------------------------------------------------------
			bool IsColourWriteEnabled() const;
			//----------------------------------------------------------
			/// Set Colour Write Enabled
			///
			/// @param Whether depth write is enabled
			//----------------------------------------------------------
			void SetColourWriteEnabled(bool inbIsColourTest);
			//----------------------------------------------------------
			/// Is Depth Write Enabled
			///
			/// @return Whether the object has depth write enabled 
			//----------------------------------------------------------
			bool IsDepthWriteEnabled() const;
			//----------------------------------------------------------
			/// Set Depth Write Enabled
			///
			/// @param Whether depth write is enabled
			//----------------------------------------------------------
			void SetDepthWriteEnabled(bool inbIsDepthTest);
			//----------------------------------------------------------
			/// Is Depth Test Enabled
			///
			/// @return Whether the object has depth test enabled 
			//----------------------------------------------------------
			bool IsDepthTestEnabled() const;
			//----------------------------------------------------------
			/// Set Depth Test Enabled
			///
			/// @param Whether depth test is enabled
			//----------------------------------------------------------
			void SetDepthTestEnabled(bool inbIsDepthTest);
			//----------------------------------------------------------
			/// Is Culling Enabled
			///
			/// @return Whether the object has face culling enabled 
			//----------------------------------------------------------
			bool IsCullingEnabled() const;
			//----------------------------------------------------------
			/// Set Culling Enabled
			///
			/// @param Whether face culling is enabled
			//----------------------------------------------------------
			void SetCullingEnabled(bool inbCullingEnabled);
            //----------------------------------------------------------
			/// Is Scissoring Enabled
			///
			/// @return Whether the object has scissor testing enabled 
			//----------------------------------------------------------
            bool IsScissoringEnabled() const;
            //----------------------------------------------------------
			/// Set Scissoring Enabled
			///
			/// @param Whether scissor testing is enabled
			//----------------------------------------------------------
            void SetScissoringEnabled(bool inbEnabled);
            //----------------------------------------------------------
            /// Set Scissoring Region
            ///
            /// @param Position of lower left corner
            /// @param Dimensions
            //----------------------------------------------------------
            void SetScissoringRegion(const Core::CVector2& invPosition, const Core::CVector2& invSize);
            //----------------------------------------------------------
            /// Get Scissoring Region Position
            ///
            /// @return Position of lower left corner
            //----------------------------------------------------------
            const Core::CVector2& GetScissoringRegionPosition() const;
            //----------------------------------------------------------
            /// Get Scissoring Region Size
            ///
            /// @return Dimensions
            //----------------------------------------------------------
            const Core::CVector2& GetScissoringRegionSize() const;
			//----------------------------------------------------------
			/// Set Blend Function
			///
			/// Tells the render system how to blend transparent pixels
			///
			/// @param Source function
			/// @param Destination function
			//----------------------------------------------------------
			void SetBlendFunction(AlphaBlend ineSource, AlphaBlend ineDest);
			//----------------------------------------------------------
			/// Get Source Blend Function
			///
			/// @return Source function of blending functions
			//----------------------------------------------------------
			AlphaBlend GetSourceBlendFunction() const;
			//----------------------------------------------------------
			/// Get Destination Blend Function
			///
			/// @return Dest function of blending functions
			//----------------------------------------------------------
			AlphaBlend GetDestBlendFunction() const;
            //----------------------------------------------------------
            /// Set Cull Face
            ///
            /// @param The face of the polygon that should be culled
			//----------------------------------------------------------
			void SetCullFace(CullFace ineCullFace);
			//----------------------------------------------------------
			/// Get Cull Face
			///
			/// @return The face of the polygon that should be culled
			//----------------------------------------------------------
			CullFace GetCullFace() const;
			//----------------------------------------------------------
			/// Set Emissive
			///
			/// Set the emissive colour
			/// @param emissive colour
			//----------------------------------------------------------
			void SetEmissive(const Core::CColour& inEmissive);
			//----------------------------------------------------------
			/// Get Emissive
			///
			/// Get the emissive colour of the material
			/// @return Ambient light colour
			//----------------------------------------------------------
			const Core::CColour& GetEmissive() const;
            //----------------------------------------------------------
			/// Set Ambient
			///
			/// Set the default colour of the material
			/// @param Ambient light colour
			//----------------------------------------------------------
			void SetAmbient(const Core::CColour& inAmbient);
			//----------------------------------------------------------
			/// Get Ambient
			///
			/// Get the default colour of the material
			/// @return Ambient light colour
			//----------------------------------------------------------
			const Core::CColour& GetAmbient() const;
			//----------------------------------------------------------
			/// Set Diffuse
			///
			/// Set the diffuse colour of the material that is affected
			/// by light position
			/// @param Diffuse light colour
			//----------------------------------------------------------
			void SetDiffuse(const Core::CColour& inDiffuse);
			//----------------------------------------------------------
			/// Get Diffuse
			///
			/// Get the diffuse colour of the material
			/// @return Diffuse light colour
			//----------------------------------------------------------
			const Core::CColour& GetDiffuse() const;
			//----------------------------------------------------------
			/// Set Specular
			///
			/// Set the specular colour of the material highlight
			/// @param Specular light colour
			//----------------------------------------------------------
			void SetSpecular(const Core::CColour& inSpecular);
			//----------------------------------------------------------
			/// Get Specular
			///
			/// Get the specular colour of the material
			/// @return Specular light colour
			//----------------------------------------------------------
			const Core::CColour& GetSpecular() const;
			//-----------------------------------------------------------
			/// Set Shader Float Value
			///
			/// Set the value of the variable with the given name
			///
			/// @param Variable name
			/// @param Float data
			//-----------------------------------------------------------
			void SetShaderFloatValue(const std::string& instrVarName, f32 infValue);
			//-----------------------------------------------------------
			/// Set Shader Vec2 Value
			///
			/// Set the value of the variable with the given name
			///
			/// @param Variable name
			/// @param Vector2 data
			//-----------------------------------------------------------
			void SetShaderVec2Value(const std::string& instrVarName, const Core::CVector2 &invValue);
			//-----------------------------------------------------------
			/// Set Shader Vec3 Value
			///
			/// Set the value of the variable with the given name
			///
			/// @param Variable name
			/// @param Vector3 data
			//-----------------------------------------------------------
			void SetShaderVec3Value(const std::string& instrVarName, const Core::CVector3 &invValue);
			//-----------------------------------------------------------
			/// Set Shader Vec4 Value
			///
			/// Set the value of the variable with the given name
			///
			/// @param Variable name
			/// @param Vector4 data
			//-----------------------------------------------------------
			void SetShaderVec4Value(const std::string& instrVarName, const Core::CVector4 &invValue);
			//-----------------------------------------------------------
			/// Set Shader Matrix Value
			///
			/// Set the value of the variable with the given name
			///
			/// @param Variable name
			/// @param Matrix data
			//-----------------------------------------------------------
			void SetShaderMatrixValue(const std::string& instrVarName, const Core::CMatrix4x4 &inmatValue);
			//-----------------------------------------------------------
			/// Set Shader Matrix Array Value
			///
			/// Set the value of the variable with the given name
			///
			/// @param Variable name
			/// @param Matrix data
			//-----------------------------------------------------------
			void SetShaderMatrixArrayValue(const std::string& instrVarName, const std::vector<Core::CMatrix4x4>& inmatValue);
			//-----------------------------------------------------------
			/// Set Shader Colour Value
			///
			/// Set the value of the variable with the given name
			///
			/// @param Variable name
			/// @param Colour data
			//-----------------------------------------------------------
			void SetShaderColourValue(const std::string& instrVarName, const Core::CColour &incolValue);
            //----------------------------------------------------------
            /// Is Cache Valid
            ///
            /// @return Whether this material has been dirtied
            //----------------------------------------------------------
            bool IsCacheValid() const;
            //----------------------------------------------------------
            /// Is Variable Cache Valid
            ///
            /// @return Whether this materials variables has been dirtied
            //----------------------------------------------------------
            bool IsVariableCacheValid() const;
            //----------------------------------------------------------
            /// Set Cache Valid
            ///
            /// @param This material has been applied and is no longer
            /// dirty
            //----------------------------------------------------------
            void SetCacheValid() const;
            
            MapStringToFloat mMapFloatShaderVars;
			MapStringToVec2 mMapVec2ShaderVars;
			MapStringToVec3 mMapVec3ShaderVars;
			MapStringToVec4 mMapVec4ShaderVars;
			MapStringToMat4 mMapMat4ShaderVars;
			MapStringToMat4Array mMapMat4ArrayShaderVars;
			MapStringToCol mMapColShaderVars;

		private:
            
            std::vector<TexturePtr> mTextures;
            
            CubemapPtr mpCubemap;
            
            ShaderPtr maShaderPrograms[(u32)ShaderPass::k_total];
            ShaderPtr mpActiveShaderProgram;
			
			Core::CColour mEmissive;
            Core::CColour mAmbient;
			Core::CColour mDiffuse;
			Core::CColour mSpecular;
            
            Core::CVector2 mvScissorPos;
            Core::CVector2 mvScissorSize;
			
			AlphaBlend mSrcBlendFunc;
			AlphaBlend mDstBlendFunc;
            CullFace meCullFace;
            
            mutable bool mbIsCacheValid;
            mutable bool mbIsVariableCacheValid;

            bool mbIsScissoringEnabled;
            bool mbIsAlphaBlended;
            bool mbIsColourWriteEnabled;
			bool mbIsDepthWriteEnabled;
			bool mbIsDepthTestEnabled;
            bool mbIsCullingEnabled;
            
            friend class IMaterialManager;
			friend class CMaterialLoader;
		};
	}
}

#endif
