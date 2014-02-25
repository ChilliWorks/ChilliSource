/*
 * File: SpriteSheet.h
 * Date: 22/10/2010 2010 
 * Description: Container for animation data and texture data for each sprite in a tpage
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_RENDERING_SPRITE_DATA_H_
#define _MO_FLO_RENDERING_SPRITE_DATA_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Resource/Resource.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class CSpriteSheet : public Core::IResource
		{
		public:
			
			struct Frame
			{
				s16 U;
				s16 V;
				s16 Width;
				s16 Height;
				s16 OffsetX;
				s16 OffsetY;
                s16 OriginalWidth;
                s16 OriginalHeight;
			};
			
			typedef Frame SpriteFrame;
			
			DECLARE_NAMED_INTERFACE(CSpriteSheet);
			
			CSpriteSheet();
			virtual ~CSpriteSheet(){}
			
			//---------------------------------------------------------------------
			/// Is A
			///
			/// @return Whether this object is of given type
			//---------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//---------------------------------------------------------------------
			/// Add Sprite Frame
			///
			/// @param Sprite frame containing width, height, UV's and offsets
			//---------------------------------------------------------------------
			void AddSpriteFrame(const Frame& inFrame);
			//---------------------------------------------------------------------
			/// Get Sprite Frame By ID
			///
			/// @param Index from pre-generated header
			/// @return Sprite frame data
			//---------------------------------------------------------------------
			const Frame& GetSpriteFrameByID(u32 inID) const;
            //---------------------------------------------------------------------
            /// Get Sprite Frame Index By ID
            ///
            /// @param ID from generated string list
            /// @return Sprite index ID
            //---------------------------------------------------------------------
            u32 GetFrameIndexByID(const std::string& inID) const;
            //---------------------------------------------------------------------
            /// Has Frame With ID
            ///
            /// @param ID from generated string list
            /// @return true if sprite exists
            //---------------------------------------------------------------------
            bool HasFrameWithID(const std::string& inID) const;
			//---------------------------------------------------------------------
			/// Set Num Sprite Frames
			///
			/// @param Number of sprite frames loaded
			//---------------------------------------------------------------------
			void SetNumSpriteFrames(u32 inudwNumFrame);
			//---------------------------------------------------------------------
            /// Get ID by Sprite Frame Index
            ///
            /// @param Sprite index ID
            /// @return ID from generated string list
            //---------------------------------------------------------------------
			const std::string& GetIDFromFrameIndex(u32 inudwFrameIndex) const;
			//---------------------------------------------------------------------
			/// Get Num Sprite Frames
			///
			/// @return Number of sprite frames loaded
			//---------------------------------------------------------------------
			u32 GetNumSpriteFrames() const;			
			//---------------------------------------------------------------------
			/// GetUVsForFrame
			///
			/// @param ID for frame 
			/// @return Rect containing UV coords for the given frame
			//---------------------------------------------------------------------
			Core::CRect GetUVsForFrame(u32 inudwFrameID) const;
			//---------------------------------------------------------------------
			/// GetUVsForFrame
			///
			/// @param ID for frame 
			/// @param Rect set to UV coords for the given frame
			//---------------------------------------------------------------------
			void GetUVsForFrame(u32 inudwFrameID, Core::CRect & outsUVRect) const;			
			//---------------------------------------------------------------------
			/// Get Size For Frame
			///
			/// @param ID for frame 
			/// param Vec2 containing size for the frame
			//---------------------------------------------------------------------
			Core::CVector2 GetSizeForFrame(u32 inudwFrameID) const;
			//---------------------------------------------------------------------
			/// Get Size For Frame
			///
			/// @param ID for frame 
			/// @param Vec2 set to size for the frame
			//---------------------------------------------------------------------
			void GetSizeForFrame(u32 inudwFrameID, Core::CVector2 & outvSize) const;
			//---------------------------------------------------------------------
			/// Get Offset For Frame
			///
			/// @param ID for frame 
			/// return Vec2 containing offset for the frame
			//---------------------------------------------------------------------
			Core::CVector2 GetOffsetForFrame(u32 inudwFrameID) const;
			//---------------------------------------------------------------------
			/// Get Offset For Frame
			///
			/// @param ID for frame 
			/// @param Vec2 set to offset for the frame
			//---------------------------------------------------------------------
			void GetOffsetForFrame(u32 inudwFrameID, Core::CVector2 & outvOffset) const;	
            //---------------------------------------------------------------------
			/// Get Size For Frame
			///
			/// @param ID for frame 
			/// param Vec2 containing density independent size for the frame
			//---------------------------------------------------------------------
			Core::CVector2 GetDPISizeForFrame(u32 inudwFrameID) const;
			//---------------------------------------------------------------------
			/// Get Size For Frame
			///
			/// @param ID for frame 
			/// param Vec2 containing density independent size for the frame
			//---------------------------------------------------------------------
			void GetDPISizeForFrame(u32 inudwFrameID, Core::CVector2 & outvSize) const;
			//---------------------------------------------------------------------
			/// Get DPI Offset For Frame
			///
			/// @param ID for frame 
			/// @param Vec2 set to teh density independent offset for the frame
			//---------------------------------------------------------------------
			Core::CVector2 GetDPIOffsetForFrame(u32 inudwFrameID) const;
			//---------------------------------------------------------------------
			/// Get DPI Offset For Frame
			///
			/// @param ID for frame 
			/// @param Vec2 set to teh density independent offset for the frame
			//---------------------------------------------------------------------
			void GetDPIOffsetForFrame(u32 inudwFrameID, Core::CVector2 & outvOffset) const;	
			//---------------------------------------------------------------------
			/// Set Texture
			///
			/// @param Texture
			//---------------------------------------------------------------------
			void SetTexture(const TexturePtr& inpTex);
			//---------------------------------------------------------------------
			/// Get Texture
			///
			/// return Texture
			//---------------------------------------------------------------------
			const TexturePtr& GetTexture() const;			
			//---------------------------------------------------------------------
			/// Animations
			//---------------------------------------------------------------------		
			u32 GetNumAnimations() const;
			//---------------------------------------------------------------------
			/// GetAnimationIndexByName
			/// @param Name of the animation to find 
			/// @return Integer index for the animation with that name if such an animatione xists or a value >= then GetNumAnimations if not
			//---------------------------------------------------------------------
			u32 GetAnimationIndexByName(const std::string & instrName) const;
			//---------------------------------------------------------------------
			/// GetNameForAnimation
			/// @param Integer index for which animation to return
			/// @return Reference to a string with the name of the animation with the given index or a blank string if inudwAnimIndex is invalid
			//---------------------------------------------------------------------
			const std::string & GetNameForAnimation(u32 inudwIndex) const;
			//---------------------------------------------------------------------
			/// GetFramesForAnimation
			/// @param Integer index for which animation to return
			/// @return Pointer to a vector containing the frame indices for the given animation or NULL if inudwAnimIndex is invalid
			//---------------------------------------------------------------------
			const DYNAMIC_ARRAY<u32> * GetFramesForAnimation(u32 inudwAnimIndex) const;
			//---------------------------------------------------------------------
			/// AddAnimation
			/// @param Name for this animation, must be unique
			/// @param Array of frame indices describing this animation
			/// @return true if successful or false if the parameter constraints above are not respected
			//---------------------------------------------------------------------			
			bool AddAnimation(std::string & instrName, const DYNAMIC_ARRAY<u32> & instrFrames);
            //---------------------------------------------------------------------
            /// Set ID Lookups
            ///
            /// @param The hashed string lookups into the sprite array
            /// @param The raw string lookups
            //---------------------------------------------------------------------
            void SetIDLookups(const DYNAMIC_ARRAY<u32>& inSpriteHashedIDLookups, const DYNAMIC_ARRAY<std::string>& inSpriteStringIDLookups);
            //---------------------------------------------------------------------
            /// Set SpriteSheet Size
            ///
            /// Sets the spritesheet width and height to be used to calculate
            /// UVs from frame sizes
            ///
            /// @param Width
            /// @param Height
            //---------------------------------------------------------------------
            void SetSpriteSheetSize(s16 inwSpriteSheetWidth, s16 inwSpriteSheetHeight);
			//---------------------------------------------------------------------
            /// Get SpriteSheet Width
			///
            /// @param Width of spritesheet
            //---------------------------------------------------------------------
            s16 GetSpriteSheetWidth() { return mwSpriteSheetWidth; }
            //---------------------------------------------------------------------
            /// Get SpriteSheet Height
			///
            /// @param Height of spritesheet
            //---------------------------------------------------------------------
            s16 GetSpriteSheetHeight() { return mwSpriteSheetHeight; }
		private:
			TexturePtr mpTexture;
			
            s16        mwSpriteSheetWidth;
            s16        mwSpriteSheetHeight;
            
			DYNAMIC_ARRAY<Frame> mFrames;
			DYNAMIC_ARRAY<std::string> mAnimationNames;
			DYNAMIC_ARRAY<DYNAMIC_ARRAY<u32> > mAnimationFrames;
            DYNAMIC_ARRAY<u32> mIDLookups;
            DYNAMIC_ARRAY<std::string> mStringIDLookups;
			
			friend class ISpriteSheetManager;
		};
	}
}

#endif