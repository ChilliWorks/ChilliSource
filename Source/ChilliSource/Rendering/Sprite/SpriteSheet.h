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
		class SpriteSheet : public Core::Resource
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
			
			CS_DECLARE_NAMEDTYPE(SpriteSheet);
			
			SpriteSheet();
			virtual ~SpriteSheet(){}
			
			//---------------------------------------------------------------------
			/// Is A
			///
			/// @return Whether this object is of given type
			//---------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
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
            /// @author S Downie
            ///
            /// @return The number of atlas map keys
            //---------------------------------------------------------------------
            u32 GetNumSpriteLookupKeys() const;
			//---------------------------------------------------------------------
			/// GetUVsForFrame
			///
			/// @param ID for frame 
			/// @return Rect containing UV coords for the given frame
			//---------------------------------------------------------------------
			Core::Rectangle GetUVsForFrame(u32 inudwFrameID) const;
			//---------------------------------------------------------------------
			/// GetUVsForFrame
			///
			/// @param ID for frame 
			/// @param Rect set to UV coords for the given frame
			//---------------------------------------------------------------------
			void GetUVsForFrame(u32 inudwFrameID, Core::Rectangle & outsUVRect) const;			
			//---------------------------------------------------------------------
			/// Get Size For Frame
			///
			/// @param ID for frame 
			/// param Vec2 containing size for the frame
			//---------------------------------------------------------------------
			Core::Vector2 GetSizeForFrame(u32 inudwFrameID) const;
			//---------------------------------------------------------------------
			/// Get Size For Frame
			///
			/// @param ID for frame 
			/// @param Vec2 set to size for the frame
			//---------------------------------------------------------------------
			void GetSizeForFrame(u32 inudwFrameID, Core::Vector2 & outvSize) const;
			//---------------------------------------------------------------------
			/// Get Offset For Frame
			///
			/// @param ID for frame 
			/// return Vec2 containing offset for the frame
			//---------------------------------------------------------------------
			Core::Vector2 GetOffsetForFrame(u32 inudwFrameID) const;
			//---------------------------------------------------------------------
			/// Get Offset For Frame
			///
			/// @param ID for frame 
			/// @param Vec2 set to offset for the frame
			//---------------------------------------------------------------------
			void GetOffsetForFrame(u32 inudwFrameID, Core::Vector2 & outvOffset) const;	
            //---------------------------------------------------------------------
			/// Get Size For Frame
			///
			/// @param ID for frame 
			/// param Vec2 containing density independent size for the frame
			//---------------------------------------------------------------------
			Core::Vector2 GetDPISizeForFrame(u32 inudwFrameID) const;
			//---------------------------------------------------------------------
			/// Get Size For Frame
			///
			/// @param ID for frame 
			/// param Vec2 containing density independent size for the frame
			//---------------------------------------------------------------------
			void GetDPISizeForFrame(u32 inudwFrameID, Core::Vector2 & outvSize) const;
			//---------------------------------------------------------------------
			/// Get DPI Offset For Frame
			///
			/// @param ID for frame 
			/// @param Vec2 set to teh density independent offset for the frame
			//---------------------------------------------------------------------
			Core::Vector2 GetDPIOffsetForFrame(u32 inudwFrameID) const;
			//---------------------------------------------------------------------
			/// Get DPI Offset For Frame
			///
			/// @param ID for frame 
			/// @param Vec2 set to teh density independent offset for the frame
			//---------------------------------------------------------------------
			void GetDPIOffsetForFrame(u32 inudwFrameID, Core::Vector2 & outvOffset) const;	
			//---------------------------------------------------------------------
			/// Set Texture
			///
			/// @param Texture
			//---------------------------------------------------------------------
			void SetTexture(const TextureSPtr& inpTex);
			//---------------------------------------------------------------------
			/// Get Texture
			///
			/// return Texture
			//---------------------------------------------------------------------
			const TextureSPtr& GetTexture() const;
            //---------------------------------------------------------------------
            /// Set ID Lookups
            ///
            /// @param The hashed string lookups into the sprite array
            /// @param The raw string lookups
            //---------------------------------------------------------------------
            void SetIDLookups(const std::vector<u32>& inSpriteHashedIDLookups, const std::vector<std::string>& inSpriteStringIDLookups);
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
			TextureSPtr mpTexture;
			
            s16        mwSpriteSheetWidth;
            s16        mwSpriteSheetHeight;
            
			std::vector<Frame> mFrames;
            std::vector<u32> mIDLookups;
            std::vector<std::string> mStringIDLookups;
			
			friend class SpriteSheetManager;
		};
	}
}

#endif