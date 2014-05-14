//
//	HighlightButton.h
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_DEFAULTS_HIGHLIGHTBUTTON_H_
#define _MOFLO_GUI_DEFAULTS_HIGHLIGHTBUTTON_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Button/Button.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>

namespace ChilliSource
{
    namespace GUI
    {
        class HighlightButton : public Button
        {
        public:

			DECLARE_META_CLASS(HighlightButton)

            HighlightButton();
            HighlightButton(const Core::ParamDictionary& insParams);
            virtual ~HighlightButton();
            //-----------------------------------------------------------
            /// Get Background Image View
            ///
            /// @return Image view
            //-----------------------------------------------------------
            const ImageViewSPtr& GetBackgroundImageView() const;
            //-----------------------------------------------------------
            /// Set Normal Image
            ///
            /// Image for unselected button
            ///
            /// @param Texture shared pointer
            //-----------------------------------------------------------
            void SetNormalImage(const Rendering::TextureCSPtr& inpTexture);
			//-----------------------------------------------------------
			/// Get Normal Image
			///
			/// @return Texture for non-selected button
			//-----------------------------------------------------------
			const Rendering::TextureCSPtr& GetNormalImage() const;
            //-----------------------------------------------------------
            /// Set Highlight Image
            ///
            /// Image for selected button
            ///
            /// @param Texture shared pointer
            //-----------------------------------------------------------
            void SetHighlightImage(const Rendering::TextureCSPtr& inpTexture);
			//-----------------------------------------------------------
			/// Get Highlight Image
			///
			/// @return Texture for selected button
			//-----------------------------------------------------------
			const Rendering::TextureCSPtr& GetHighlightImage() const;
            //-----------------------------------------------------------
            /// Set Default UVs
            ///
            /// Sets the UV coordinates for the default texture
            ///
            /// @param Rectangle for the UVs
            //-----------------------------------------------------------
            void SetDefaultUVs(Core::Rectangle insUVs);
            //-----------------------------------------------------------
            /// Set Highlight UVs
            ///
            /// Sets the UV coordinates for the Highlight texture
            ///
            /// @param Rectangle for the UVs
            //-----------------------------------------------------------
            void SetHighlightUVs(Core::Rectangle insUVs);
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
			/// Set Normal Sprite Sheet Index
			///
			/// @param Index of default state on sprite sheet
			//-----------------------------------------------------------
			void SetNormalTextureAtlasID(const std::string& instrID);
			//-----------------------------------------------------------
			/// Set Highlight Sprite Sheet Index ID
			///
			/// @param Index ID of highlight state on sprite sheet
			//-----------------------------------------------------------
			void SetHighlightTextureAtlasID(const std::string& instrID);
			//-----------------------------------------------------------
			/// Get Normal Sprite Sheet Index ID
			///
			/// @return Index ID of default state on sprite sheet
			//-----------------------------------------------------------
			const std::string& GetNormalTextureAtlasID() const;
			//-----------------------------------------------------------
			/// Get Highlight Sprite Sheet Index ID
			///
			/// @return Index ID of highlight state on sprite sheet
			//-----------------------------------------------------------
			const std::string& GetHighlightTextureAtlasID() const;
			//--------------------------------------------------------
			/// Enable Size From Image
			///
			/// When this is enabled the image view's size will be 
			/// based on the size of the image
			///
			/// @param Enable/disable
			//--------------------------------------------------------
			void EnableSizeFromImage(bool inbEnable);
			//--------------------------------------------------------
			/// Is Size From Image Enabled
			///
			/// When this is enabled the image view's size will be 
			/// based on the size of the image
			///
			/// @return Whether the image view's size will be 
			/// based on the size of the image
			//--------------------------------------------------------
			bool IsSizeFromImageEnabled() const;
			//--------------------------------------------------------
            /// Set Size From Image
            ///
            /// Sets the of the image view to the size of the image
            /// without requiring a draw to call. Useful for working
            /// with relative sizes and aspect ratios.
            //--------------------------------------------------------
            void SetSizeFromImage();
            //--------------------------------------------------------
            /// Update Size Maintaining Aspect
            ///
            /// Image view can be sized from assets etc. This function
            /// calculates the size based on these factors
            //--------------------------------------------------------
            void UpdateSizeMaintainingAspect();
            //--------------------------------------------------------
            /// Enable Width From Image
            ///
            /// When this is enabled the image view's width will be
            /// based on the width of the image
            ///
            /// @param Enable/disable
            //--------------------------------------------------------
            void EnableWidthFromImage(bool inbEnable);
			//--------------------------------------------------------
			/// Is Width From Image Enabled
			///
			/// When this is enabled the image view's width will be
			/// based on the width of the image
			///
			/// @return Whether the image view's width will be
			/// based on the width of the image
			//--------------------------------------------------------
			bool IsWidthFromImageEnabled() const;
            //--------------------------------------------------------
            /// Set Width From Image
            ///
            /// Sets the of the image view to the width of the image
            /// without requiring a draw to call. Useful for working
            /// with relative sizes and aspect ratios.
            //--------------------------------------------------------
            void SetWidthFromImage();
            //--------------------------------------------------------
            /// Enable Height From Image
            ///
            /// When this is enabled the image view's height will be
            /// based on the height of the image
            ///
            /// @param Enable/disable
            //--------------------------------------------------------
            void EnableHeightFromImage(bool inbEnable);
			//--------------------------------------------------------
			/// Is Height From Image Enabled
			///
			/// When this is enabled the image view's height will be
			/// based on the height of the image
			///
			/// @return Whether the image view's height will be
			/// based on the height of the image
			//--------------------------------------------------------
			bool IsHeightFromImageEnabled() const;
            //--------------------------------------------------------
            /// Set Height From Image
            ///
            /// Sets the of the image view to the height of the image
            /// without requiring a draw to call. Useful for working
            /// with relative sizes and aspect ratios.
            //--------------------------------------------------------
            void SetHeightFromImage();
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
            //--------------------------------------------------------
            /// Set Fill Maintaining Aspect
            ///
            /// Based on the size of the image and the target size
            /// resize the image view to be the minimum of the target
            /// size while maintaining the aspect ratio
            ///
            /// @param Unified size
            //--------------------------------------------------------
            void SetFillMaintainingAspect(f32 infRelWidth, f32 infRelHeight, f32 infAbsWidth, f32 infAbsHeight);
            //--------------------------------------------------------
            /// Set Fit Maintaining Aspect
            ///
            /// Based on the size of the image and the target size
            /// resize the image view to be the maximum of the target
            /// size while maintaining the aspect ratio
            ///
            /// @param Unified size
            //--------------------------------------------------------
            void SetFitMaintainingAspect(f32 infRelWidth, f32 infRelHeight, f32 infAbsWidth, f32 infAbsHeight);
			//--------------------------------------------------------
			/// Get Size From Image
			///
			/// @return Absolute size of the image
			//--------------------------------------------------------
			Core::Vector2 GetSizeFromImage() const;
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
			//--------------------------------------------------------
			/// Layout Content
			///
			/// Called when the view is able to retrieve an absolute
			/// value.
			//--------------------------------------------------------
			void LayoutContent();
			
        private:
            //-----------------------------------------------------------
            /// Triggered when a button event happens that should
            /// cause the button to highlight
            ///
            /// @author S Downie
            ///
            /// @param Button that triggered event
            /// @param The pointer
            //-----------------------------------------------------------
            void OnButtonSelect(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer);
            //-----------------------------------------------------------
            /// Triggered when a button event happens that should
            /// cause the button to fire
            ///
            /// @author S Downie
            ///
            /// @param Button that triggered event
            /// @param The pointer
            //-----------------------------------------------------------
            void OnButtonActivated(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer);
            //-----------------------------------------------------------
            /// Triggered when a button event happens that should
            /// cause the button to return to normal
            ///
            /// @author S Downie
            ///
            /// @param Button that triggered event
            /// @param The pointer
            //-----------------------------------------------------------
            void OnButtonDeselect(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer);
            //-----------------------------------------------------------
            /// Triggered when a button event happens that should
            /// cause the button to return to normal
            ///
            /// @author S Downie
            ///
            /// @param Button that triggered event
            /// @param The pointer
            //-----------------------------------------------------------
            void OnButtonDeselectThreshold(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer);
			//-----------------------------------------------------------
			/// Draw
			///
			/// Overloaded draw method
			///
			/// @param Canvas renderer
			//-----------------------------------------------------------
			void Draw(Rendering::CanvasRenderer* inpCanvasRenderer);
            
        private:
            
            Core::UnifiedVector2 mvFillMaintainTarget;
            Core::UnifiedVector2 mvFitMaintainTarget;
            
            bool mbFillMaintain;
            bool mbFitMaintain;
			
			//---Properties
			DECLARE_PROPERTY_A(Rendering::TextureCSPtr, NormalTexture, SetNormalImage, GetNormalImage);
			DECLARE_PROPERTY_A(Rendering::TextureCSPtr, HighlightTexture, SetHighlightImage, GetHighlightImage);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, NormalTextureAtlas, SetNormalTextureAtlas, GetNormalTextureAtlas);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, HighlightTextureAtlas, SetHighlightTextureAtlas, GetHighlightTextureAtlas);

			DECLARE_PROPERTY_A(Core::Colour, HighlightColour, SetHighlightColour, GetHighlightColour);

			DECLARE_PROPERTY_A(std::string, NormalTextureAtlasID, SetNormalTextureAtlasID, GetNormalTextureAtlasID);
			DECLARE_PROPERTY_A(std::string, HighlightTextureAtlasID, SetHighlightTextureAtlasID, GetHighlightTextureAtlasID);

			DECLARE_PROPERTY_A(bool, SizeFromImage, EnableSizeFromImage, IsSizeFromImageEnabled);
            DECLARE_PROPERTY_A(bool, WidthFromImage, EnableSizeFromImage, IsSizeFromImageEnabled);
            DECLARE_PROPERTY_A(bool, HeightFromImage, EnableSizeFromImage, IsSizeFromImageEnabled);
			DECLARE_PROPERTY_A(bool, HeightMaintain, EnableHeightMaintainingAspect, IsHeightMaintainingAspectEnabled);
			DECLARE_PROPERTY_A(bool, WidthMaintain, EnableWidthMaintainingAspect, IsWidthMaintainingAspectEnabled);

            ImageViewSPtr mpBackgroundImage;
            
            Core::EventConnectionUPtr m_pressedInsideConnection;
            Core::EventConnectionUPtr m_releasedInsideConnection;
            Core::EventConnectionUPtr m_movedWithinConnection;
            Core::EventConnectionUPtr m_movedOutsideConnection;
			
            Core::Rectangle msDefaultUVs;
            Core::Rectangle msHighlightUVs;
            
            Core::Vector2 mvSelectedPos;
            
			bool mbSelected;
		};
    }
}

#endif