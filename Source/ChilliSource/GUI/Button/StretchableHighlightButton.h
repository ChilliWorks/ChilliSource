//
//	StretchableHighlightButton.h
//  moFlo
//
//  Created by Ian Copland on 03/04/2012.
//  Copyright 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_DEFAULTS_STRETCHABLEHIGHLIGHTBUTTON_H_
#define _MOFLO_GUI_DEFAULTS_STRETCHABLEHIGHLIGHTBUTTON_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Button/Button.h>

namespace ChilliSource
{
    namespace GUI
    {
        //===========================================================
        /// Stretchable Highlight Button
        ///
        /// A button that functions similarly to a Highlight Button,
        /// but built out of 9 patches. This means it can scale in
        /// in the same was as a stretchable image.
        //===========================================================
        class StretchableHighlightButton : public Button
        {
        public:
			DECLARE_META_CLASS(StretchableHighlightButton)
            
            //-----------------------------------------------------------
            /// Sprite Sheet Index
            ///
            /// A container struct for the indices for the 9 patches.
            //-----------------------------------------------------------
            struct TextureAtlasIndex
            {
                u32 udwTopLeft;
                u32 udwTopRight;
                u32 udwBottomLeft;
                u32 udwBottomRight;
                u32 udwTopCentre;
                u32 udwBottomCentre;
                u32 udwLeftCentre;
                u32 udwRightCentre;
                u32 udwMiddleCentre;
            };
            //-----------------------------------------------------------
            /// Constructor
            //-----------------------------------------------------------
            StretchableHighlightButton();
            //-----------------------------------------------------------
            /// Constructor
            ///
            /// A constructor for creating the GUI Element from a param
            /// dictionary. This is used mostly for loading from mogui
            /// files.
            ///
            /// @param The param dictionary.
            //-----------------------------------------------------------
            StretchableHighlightButton(const Core::ParamDictionary& insParams);
            //-----------------------------------------------------------
            /// Set Normal Texture
            ///
            /// @param Texture with with normal image
            //-----------------------------------------------------------
            void SetNormalTexture(const Rendering::TextureSPtr& inpTexture);
			//-----------------------------------------------------------
			/// Set Highlight Texture
			///
			/// @param Texture with with highlight image
			//-----------------------------------------------------------
			void SetHighlightTexture(const Rendering::TextureSPtr& inpTexture);
            //-----------------------------------------------------------
            /// Get Normal Texture
            ///
            /// @return Texture with default image
            //-----------------------------------------------------------
			const Rendering::TextureSPtr& GetNormalTexture() const;
			//-----------------------------------------------------------
			/// Get Highlight Texture
			///
			/// @return Texture with highlight image
			//-----------------------------------------------------------
			const Rendering::TextureSPtr& GetHighlightTexture() const;
            //-----------------------------------------------------------
            /// Set Normal Sprite Sheet
            ///
            /// @param Sprite sheet with with normal image
            //-----------------------------------------------------------
            void SetNormalTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
			//-----------------------------------------------------------
			/// Set Highlight Sprite Sheet
			///
			/// @param Sprite sheet with with highlight image
			//-----------------------------------------------------------
			void SetHighlightTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
            //-----------------------------------------------------------
            /// Get Normal Sprite Sheet
            ///
            /// @return Sprite sheet with default image
            //-----------------------------------------------------------
			const Rendering::TextureAtlasCSPtr& GetNormalTextureAtlas() const;
			//-----------------------------------------------------------
			/// Get Highlight Sprite Sheet
			///
			/// @return Sprite sheet with highlight image
			//-----------------------------------------------------------
			const Rendering::TextureAtlasCSPtr& GetHighlightTextureAtlas() const;
			//-----------------------------------------------------------
			/// Set Base Normal Sprite Sheet Index ID
			///
			/// @param The first section of the index id's.
			//-----------------------------------------------------------
			void SetBaseNormalTextureAtlasID(const std::string& instrID);
			//-----------------------------------------------------------
			/// Set Base Highlight Sprite Sheet Index ID
			///
			/// @param The first section of the index id's.
			//-----------------------------------------------------------
			void SetBaseHighlightTextureAtlasID(const std::string& instrID);
			//-----------------------------------------------------------
			/// Get Normal Sprite Sheet Base Index ID
			///
			/// @return Index ID of default state on sprite sheet
			//-----------------------------------------------------------
			const std::string& GetBaseNormalTextureAtlasID() const;
			//-----------------------------------------------------------
			/// Get Highlight Sprite Sheet Base Index ID
			///
			/// @return Index ID of highlight state on sprite sheet
			//-----------------------------------------------------------
			const std::string& GetBaseHighlightTextureAtlasID() const;
            //---------------------------------------------------------
            /// Set Normal Sprite Sheet Indices
            ///
            /// @param Struct containing the tpage index of each patch
            //---------------------------------------------------------
            void SetNormalTextureAtlasIndices(const TextureAtlasIndex& insIndices);
            //---------------------------------------------------------
            /// Set Normal Sprite Sheet Indices
            ///
            /// Top Left
            /// Top Right
            /// Bottom Left
            /// Bottom Right
            /// Top Centre
            /// Bottom Centre
            /// Left Centre
            /// Right Centre
            /// Middle Centre
            ///
            /// @param Array containing the tpage index of each patch
            //---------------------------------------------------------
            void SetNormalTextureAtlasIndices(const u32* inpIndices);
            //---------------------------------------------------------
            /// Set Highlight Sprite Sheet Indices
            ///
            /// @param Struct containing the tpage index of each patch
            //---------------------------------------------------------
            void SetHighlightTextureAtlasIndices(const TextureAtlasIndex& insIndices);
            //---------------------------------------------------------
            /// Set Highlight Sprite Sheet Indices
            ///
            /// Top Left
            /// Top Right
            /// Bottom Left
            /// Bottom Right
            /// Top Centre
            /// Bottom Centre
            /// Left Centre
            /// Right Centre
            /// Middle Centre
            ///
            /// @param Array containing the tpage index of each patch
            //---------------------------------------------------------
            void SetHighlightTextureAtlasIndices(const u32* inpIndices);
            //-----------------------------------------------------------
            /// Set Select Audio Effect
            ///
            /// @param Audio effect played when the button is selected
            //-----------------------------------------------------------
            void SetSelectAudioEffect(const Audio::AudioComponentSPtr& inpEffect);
			//-----------------------------------------------------------
            /// Set De-Select Audio Effect
            ///
            /// @param Audio effect played when the button is selected
            //-----------------------------------------------------------
            void SetDeselectAudioEffect(const Audio::AudioComponentSPtr& inpEffect);
            //--------------------------------------------------------
			/// Set Highlight Colour
			///
			/// @param Tint colour to apply when the button is
			/// selected
			//--------------------------------------------------------
			void SetHighlightColour(const Core::Colour & inValue);
			//--------------------------------------------------------
			/// Get Highlight Colour
			///
			/// @return Tint colour to apply when the button is
			/// selected
			//--------------------------------------------------------
			const Core::Colour & GetHighlightColour() const;
            //-----------------------------------------------------------
			/// Draw
			///
			/// Overloaded draw method
			///
			/// @param Canvas renderer
			//-----------------------------------------------------------
			void Draw(Rendering::CanvasRenderer* inpCanvas);
            //--------------------------------------------------------
            /// Set Width Maintaining Aspect
            ///
            /// Change the current width of the image view and resize the height
            /// to maintain the aspect ratio of the image
            ///
            /// @param Unified width
            //--------------------------------------------------------
            void SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth);
            //--------------------------------------------------------
            /// Set Height Maintaining Aspect
            ///
            /// Change the current height of the image view and resize the width
            /// to maintain the aspect ratio of the image
            ///
            /// @param Unified height
            //--------------------------------------------------------
            void SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight);
            //--------------------------------------------------------
            /// Enable Width Maintaining Aspect
            ///
            /// Enables auto scaling of the height to maintain the aspect ratio
            ///
            /// @param boolean to disable or enable
            //--------------------------------------------------------
            void EnableWidthMaintainingAspect(bool inbEnabled);
			//--------------------------------------------------------
			/// Is Width Maintaining Aspect Enabled
			///
			/// @return Whether auto scaling of the width to maintain the aspect ratio
			//--------------------------------------------------------
			bool IsWidthMaintainingAspectEnabled() const;
            //--------------------------------------------------------
            /// Enable Height Maintaining Aspect
            ///
            /// Enables auto scaling of the height to maintain the aspect ratio
            ///
            /// @param boolean to disable or enable
            //--------------------------------------------------------
            void EnableHeightMaintainingAspect(bool inbEnabled);
			//--------------------------------------------------------
			/// Is Height Maintaining Aspect Enabled
			///
			/// @return Whether auto scaling of the height to maintain the aspect ratio
			//--------------------------------------------------------
			bool IsHeightMaintainingAspectEnabled() const;
            //-----------------------------------------------------------
            /// Destructor
            //-----------------------------------------------------------
            virtual ~StretchableHighlightButton();
            
        private:
            //-----------------------------------------------------------
            /// Triggered when a button event happens that should
            /// cause the button to highlight
            ///
            /// @author I Copland
            ///
            /// @param Button that triggered event
            /// @param The pointer.
            //-----------------------------------------------------------
            void OnButtonSelect(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer);
            //-----------------------------------------------------------
            /// Triggered when a button event happens that should
            /// cause the button to fire
            ///
            /// @author I Copland
            ///
            /// @param Button that triggered event
            /// @param The pointer.
            //-----------------------------------------------------------
            void OnButtonActivated(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer);
            //-----------------------------------------------------------
            /// Triggered when a button event happens that should
            /// cause the button to return to normal
            ///
            /// @author I Copland
            ///
            /// @param Button that triggered event
            /// @param The pointer.
            //-----------------------------------------------------------
            void OnButtonDeselect(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer);
            //-----------------------------------------------------------
            /// Triggered when a button event happens that should
            /// cause the button to return to normal
            ///
            /// @author I Copland
            ///
            /// @param Button that triggered event
            /// @param The pointer.
            //-----------------------------------------------------------
            void OnButtonDeselectThreshold(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer);
            
        private:
            Rendering::TextureSPtr mCurrentTexture;
            Rendering::TextureAtlasCSPtr mCurrentTextureAtlas;
            TextureAtlasIndex msCurrentIndices;
            TextureAtlasIndex msNormalIndices;
            TextureAtlasIndex msHighlightIndices;
            Audio::AudioComponentSPtr mpSelectAudioEffect;
			Audio::AudioComponentSPtr mpDeselectAudioEffect;
            Core::Colour mCurrentColour;
			bool mbSelected;
            
            Core::Vector2 mvSelectedPos;
            
            Core::ConnectionUPtr m_pressedInsideConnection;
            Core::ConnectionUPtr m_releasedInsideConnection;
            Core::ConnectionUPtr m_movedWithinConnection;
            Core::ConnectionUPtr m_movedOutsideConnection;
            
            //---Properties
			DECLARE_PROPERTY_A(Rendering::TextureSPtr, NormalTexture, SetNormalTexture, GetNormalTexture);
			DECLARE_PROPERTY_A(Rendering::TextureSPtr, HighlightTexture, SetHighlightTexture, GetHighlightTexture);
            DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, NormalTextureAtlas, SetNormalTextureAtlas, GetNormalTextureAtlas);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, HighlightTextureAtlas, SetHighlightTextureAtlas, GetHighlightTextureAtlas);
			DECLARE_PROPERTY_A(std::string, BaseNormalTextureAtlasID, SetBaseNormalTextureAtlasID, GetBaseNormalTextureAtlasID);
			DECLARE_PROPERTY_A(std::string, BaseHighlightTextureAtlasID, SetBaseHighlightTextureAtlasID, GetBaseHighlightTextureAtlasID);
			DECLARE_PROPERTY_A(Core::Colour, HighlightColour, SetHighlightColour, GetHighlightColour);
			DECLARE_PROPERTY_A(bool, HeightMaintain, EnableHeightMaintainingAspect, IsHeightMaintainingAspectEnabled);
			DECLARE_PROPERTY_A(bool, WidthMaintain, EnableWidthMaintainingAspect, IsWidthMaintainingAspectEnabled);
		};
    }
}

#endif