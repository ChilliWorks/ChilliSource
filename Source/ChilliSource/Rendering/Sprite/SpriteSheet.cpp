/*
 * File: SpriteSheet.cpp
 * Date: 22/10/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

namespace ChilliSource
{
	namespace Rendering
	{
        DEFINE_NAMED_INTERFACE(CSpriteSheet);
		//-------------------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------------------------
		CSpriteSheet::CSpriteSheet()
        :mwSpriteSheetWidth(0)
        ,mwSpriteSheetHeight(0)
		{
		}
		//---------------------------------------------------------------------
		/// Is A
		///
		/// @return Whether this object is of given type
		//---------------------------------------------------------------------
		bool CSpriteSheet::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CSpriteSheet::InterfaceID;
		}
		//---------------------------------------------------------------------
		/// Add Sprite Frame
		///
		/// @param Sprite frame containing width, height, UV's and offsets
		//---------------------------------------------------------------------
		void CSpriteSheet::AddSpriteFrame(const CSpriteSheet::Frame& inFrame)
		{
			mFrames.push_back(inFrame);
		}
		//---------------------------------------------------------------------
		/// Get Sprite Frame By ID
		///
		/// @param Index from pre-generated header
		/// @return Sprite frame data
		//---------------------------------------------------------------------
		const CSpriteSheet::Frame& CSpriteSheet::GetSpriteFrameByID(u32 inID) const
		{
			return mFrames[inID];
		}
        //---------------------------------------------------------------------
        /// Get Sprite Frame Index By ID
        ///
        /// @param ID from generated string list
        /// @return Sprite index ID
        //---------------------------------------------------------------------
        u32 CSpriteSheet::GetFrameIndexByID(const std::string& inID) const
        {
            u32 udwHashedString = Core::CHashCRC32::GenerateHashCode(inID);
            for(u32 i=0; i<mIDLookups.size(); ++i)
            {
                if(mIDLookups[i] == udwHashedString)
                {
                    return i;
                }
            }
            
//            WARNING_LOG("Sprite not found for ID: " + inID);
            return 0;
        }
        
        //---------------------------------------------------------------------
        /// Has Frame With ID
        ///
        /// @param ID from generated string list
        /// @return true if sprite exists
        //---------------------------------------------------------------------
        bool CSpriteSheet::HasFrameWithID(const std::string& inID) const
        {
            u32 udwHashedString = Core::CHashCRC32::GenerateHashCode(inID);
            for(u32 i=0; i<mIDLookups.size(); ++i)
            {
                if(mIDLookups[i] == udwHashedString)
                {
                    return true;
                }
            }
            
            return false;
        }
        
		//---------------------------------------------------------------------
        /// Get Sprite ID by Frame Index
        ///
        /// @param Sprite index ID
        /// @return ID from generated string list
        //---------------------------------------------------------------------
        const std::string& CSpriteSheet::GetIDFromFrameIndex(u32 inudwFrameIndex) const
        {
            if(inudwFrameIndex < mStringIDLookups.size())
            {
                return mStringIDLookups[inudwFrameIndex];
            }
            
            WARNING_LOG("Sprite not found for Frame Index: " + Core::ToString(inudwFrameIndex));
            return Core::CStringUtils::BLANK;
        }		
		//---------------------------------------------------------------------
		/// Get Num Sprite Frames
		///
		/// @return Number of sprite frames loaded
		//---------------------------------------------------------------------
		u32 CSpriteSheet::GetNumSpriteFrames() const
        {
			return mFrames.size();
		}
		//---------------------------------------------------------------------
		/// Set Num Sprite Frames
		///
		/// @param Number of sprite frames loaded
		//---------------------------------------------------------------------
		void CSpriteSheet::SetNumSpriteFrames(u32 inudwNumFrame)
		{
			mFrames.reserve(inudwNumFrame);
		}
		//---------------------------------------------------------------------
		/// GetUVsForFrame
		///
		/// @param ID for frame 
		/// @return Rect containing UV coords for the given frame
		//---------------------------------------------------------------------
		Core::CRect CSpriteSheet::GetUVsForFrame(u32 inudwFrameID) const
        {
			Core::CRect sResult;
			GetUVsForFrame(inudwFrameID, sResult);
			return sResult;
		}
		//---------------------------------------------------------------------
		/// GetUVsForFrame
		///
		/// @param ID for frame 
		/// @param Rect set to UV coords for the given frame
		//---------------------------------------------------------------------
		void CSpriteSheet::GetUVsForFrame(u32 inudwFrameID, Core::CRect & outsUVRect) const
        {
			const Frame & sFrame = mFrames[inudwFrameID];
			
            f32 fInvWidth = 1.0f / (mwSpriteSheetWidth == 0 ? mpTexture->GetWidth() : mwSpriteSheetWidth);
            f32 fInvHeight = 1.0f / (mwSpriteSheetHeight == 0 ? mpTexture->GetHeight() : mwSpriteSheetHeight);
            
			outsUVRect.vOrigin.x = (f32)(sFrame.U + kHalfTexelOffset) * fInvWidth;
			outsUVRect.vOrigin.y = (f32)(sFrame.V + kHalfTexelOffset) * fInvHeight;
			outsUVRect.vSize.x = (f32)(sFrame.Width - kTexelOffset) * fInvWidth;
			outsUVRect.vSize.y = (f32)(sFrame.Height - kTexelOffset) * fInvHeight;
		}
		//---------------------------------------------------------------------
		/// GetSizeForFrame
		///
		/// @param ID for frame 
		/// param Vec2 containing size for the frame
		//---------------------------------------------------------------------
		Core::CVector2 CSpriteSheet::GetSizeForFrame(u32 inudwFrameID) const
        {
			Core::CVector2 sResult;
			GetSizeForFrame(inudwFrameID, sResult);
			return sResult;
		}
		//---------------------------------------------------------------------
		/// GetSizeForFrame
		///
		/// @param ID for frame 
		/// @param Vec2 set to size for the frame
		//---------------------------------------------------------------------
		void CSpriteSheet::GetSizeForFrame(u32 inudwFrameID, Core::CVector2 & outvSize) const
        {
			outvSize.x = mFrames[inudwFrameID].Width;
			outvSize.y = mFrames[inudwFrameID].Height;
		}
		//---------------------------------------------------------------------
		/// GetOffsetForFrame
		///
		/// @param ID for frame 
		/// return Vec2 containing offset for the frame
		//---------------------------------------------------------------------
		Core::CVector2 CSpriteSheet::GetOffsetForFrame(u32 inudwFrameID) const
        {
			Core::CVector2 vResult;
			GetOffsetForFrame(inudwFrameID,vResult);
			return vResult;
		}
		//---------------------------------------------------------------------
		/// GetOffsetForFrame
		///
		/// @param ID for frame 
		/// @param Vec2 set to offset for the frame
		//---------------------------------------------------------------------
		void CSpriteSheet::GetOffsetForFrame(u32 inudwFrameID, Core::CVector2 & outvOffset) const
        {
			outvOffset.x = mFrames[inudwFrameID].OffsetX;
			outvOffset.y = mFrames[inudwFrameID].OffsetY;
		}
        //---------------------------------------------------------------------
        /// Get Size For Frame
        ///
        /// @param ID for frame 
        /// param Vec2 containing density independent size for the frame
        //---------------------------------------------------------------------
        Core::CVector2 CSpriteSheet::GetDPISizeForFrame(u32 inudwFrameID) const
        {
            Core::CVector2 sResult;
			GetDPISizeForFrame(inudwFrameID, sResult);
			return sResult;
        }
        //---------------------------------------------------------------------
        /// Get Size For Frame
        ///
        /// @param ID for frame 
        /// param Vec2 containing density independent size for the frame
        //---------------------------------------------------------------------
        void CSpriteSheet::GetDPISizeForFrame(u32 inudwFrameID, Core::CVector2 & outvSize) const
        {
            outvSize.x = mFrames[inudwFrameID].Width * (1.0f/Core::CScreen::GetDensity());
			outvSize.y = mFrames[inudwFrameID].Height * (1.0f/Core::CScreen::GetDensity());
        }
        //---------------------------------------------------------------------
        /// Get DPI Offset For Frame
        ///
        /// @param ID for frame 
        /// @param Vec2 set to teh density independent offset for the frame
        //---------------------------------------------------------------------
        Core::CVector2 CSpriteSheet::GetDPIOffsetForFrame(u32 inudwFrameID) const
        {
            Core::CVector2 vResult;
			GetDPIOffsetForFrame(inudwFrameID,vResult);
			return vResult;
        }
        //---------------------------------------------------------------------
        /// Get DPI Offset For Frame
        ///
        /// @param ID for frame 
        /// @param Vec2 set to teh density independent offset for the frame
        //---------------------------------------------------------------------
        void CSpriteSheet::GetDPIOffsetForFrame(u32 inudwFrameID, Core::CVector2 & outvOffset) const
        {
            outvOffset.x = mFrames[inudwFrameID].OffsetX * (1.0f/Core::CScreen::GetDensity());
			outvOffset.y = mFrames[inudwFrameID].OffsetY * (1.0f/Core::CScreen::GetDensity());
        }
		//---------------------------------------------------------------------
		/// Set Texture
		///
		/// @param Texture
		//---------------------------------------------------------------------
		void CSpriteSheet::SetTexture(const TexturePtr& inpTex)
		{
			mpTexture = inpTex;
		}
		//---------------------------------------------------------------------
		/// Get Texture
		///
		/// return Texture
		//---------------------------------------------------------------------
		const TexturePtr& CSpriteSheet::GetTexture() const
		{
			return mpTexture;
		}
		
		u32 CSpriteSheet::GetNumAnimations() const
		{
			return mAnimationNames.size();
		}
		u32 CSpriteSheet::GetAnimationIndexByName(const std::string & instrName) const
		{
			
			for (u32 nAnim = 0; nAnim < mAnimationNames.size(); nAnim++){
			
				if (mAnimationNames[nAnim] == instrName)
				{
					return nAnim;
				}
			}
			
			return UINT_MAX;
		}
		const std::string & CSpriteSheet::GetNameForAnimation(u32 inudwIndex) const
		{
			
			if (inudwIndex < mAnimationNames.size())
				return mAnimationNames[inudwIndex];
			
			return Core::CStringUtils::BLANK;
		}
		const DYNAMIC_ARRAY<u32> * CSpriteSheet::GetFramesForAnimation(u32 inudwAnimIndex) const
		{
			
			if (inudwAnimIndex < mAnimationFrames.size())
				return &mAnimationFrames[inudwAnimIndex];
			
			return nullptr;
		}
		
		bool CSpriteSheet::AddAnimation(std::string & instrName, const DYNAMIC_ARRAY<u32> & instrFrames)
		{
			if (GetAnimationIndexByName(instrName) != UINT_MAX)
			{
				return false;
			}
			
			mAnimationNames.push_back(instrName);
			mAnimationFrames.push_back(instrFrames);
			
			return true;
		}
        //---------------------------------------------------------------------
        /// Set ID Lookups
        ///
        /// @param The hashed string lookups into the sprite array
        /// @param The raw string lookups
        //---------------------------------------------------------------------
        void CSpriteSheet::SetIDLookups(const DYNAMIC_ARRAY<u32>& inSpriteHashedIDLookups, const DYNAMIC_ARRAY<std::string>& inSpriteStringIDLookups)
        {
            mIDLookups = inSpriteHashedIDLookups;
            mStringIDLookups = inSpriteStringIDLookups;
        }
        //---------------------------------------------------------------------
        /// Set SpriteSheet Size
        ///
        /// Sets the spritesheet width and height to be used to calculate
        /// UVs from frame sizes
        ///
        /// @param Width
        /// @param Height
        //---------------------------------------------------------------------
        void CSpriteSheet::SetSpriteSheetSize(s16 inwSpriteSheetWidth, s16 inwSpriteSheetHeight)
        {
            mwSpriteSheetWidth = inwSpriteSheetWidth;
            mwSpriteSheetHeight = inwSpriteSheetHeight;
        }
	}
}
