//
//	HighlightButton.h
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_DEFAULTS_HIGHLIGHTBUTTON_H_
#define _MOFLO_GUI_DEFAULTS_HIGHLIGHTBUTTON_H_

#include <moFlo/GUI/ForwardDeclarations.h>
#include <moFlo/GUI/Button.h>
#include <moFlo/Core/Math/Shapes.h>

#include <moFlo/Audio/ForwardDeclarations.h>

namespace moFlo
{
    namespace GUI
    {
        class CHighlightButton : public IButton
        {
        public:

			DECLARE_META_CLASS(CHighlightButton)

            CHighlightButton();
            CHighlightButton(const Core::ParamDictionary& insParams);
            virtual ~CHighlightButton();
            //-----------------------------------------------------------
            /// Get Background Image View
            ///
            /// @return Image view
            //-----------------------------------------------------------
            const ImageViewPtr& GetBackgroundImageView() const;
            //-----------------------------------------------------------
            /// Set Normal Image
            ///
            /// Image for unselected button
            ///
            /// @param Texture shared pointer
            //-----------------------------------------------------------
            void SetNormalImage(const Rendering::TexturePtr& inpTexture);
			//-----------------------------------------------------------
			/// Get Normal Image
			///
			/// @return Texture for non-selected button
			//-----------------------------------------------------------
			const Rendering::TexturePtr& GetNormalImage() const;
            //-----------------------------------------------------------
            /// Set Highlight Image
            ///
            /// Image for selected button
            ///
            /// @param Texture shared pointer
            //-----------------------------------------------------------
            void SetHighlightImage(const Rendering::TexturePtr& inpTexture);
			//-----------------------------------------------------------
			/// Get Highlight Image
			///
			/// @return Texture for selected button
			//-----------------------------------------------------------
			const Rendering::TexturePtr& GetHighlightImage() const;
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
            void SetNormalSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet);
			//-----------------------------------------------------------
			/// Set Highlight Sprite Sheet
			///
			/// @param Sprite sheet with with highlight image
			//-----------------------------------------------------------
			void SetHighlightSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet);
            //-----------------------------------------------------------
            /// Get Normal Sprite Sheet
            ///
            /// @return Sprite sheet with default image
            //-----------------------------------------------------------
			const Rendering::SpriteSheetPtr& GetNormalSpriteSheet() const;
			//-----------------------------------------------------------
			/// Get Highlight Sprite Sheet
			///
			/// @return Sprite sheet with highlight image
			//-----------------------------------------------------------
			const Rendering::SpriteSheetPtr& GetHighlightSpriteSheet() const;
            //-----------------------------------------------------------
            /// Set Normal Sprite Sheet Index
            ///
            /// @param Index of default state on sprite sheet
            //-----------------------------------------------------------
            void SetNormalSpriteSheetIndex(u32 inudwIndex);
            //-----------------------------------------------------------
            /// Set Highlight Sprite Sheet Index
            ///
            /// @param Index of highlight state on sprite sheet
            //-----------------------------------------------------------
            void SetHighlightSpriteSheetIndex(u32 inudwIndex);
            //-----------------------------------------------------------
            /// Get Normal Sprite Sheet Index
            ///
            /// @return Index of default state on sprite sheet
            //-----------------------------------------------------------
            u32 GetNormalSpriteSheetIndex() const;
			//-----------------------------------------------------------
			/// Get Highlight Sprite Sheet Index
			///
			/// @return Index of highlight state on sprite sheet
			//-----------------------------------------------------------
			u32 GetHighlightSpriteSheetIndex() const;
			//-----------------------------------------------------------
			/// Set Normal Sprite Sheet Index
			///
			/// @param Index of default state on sprite sheet
			//-----------------------------------------------------------
			void SetNormalSpriteSheetIndexID(const std::string& instrID);
			//-----------------------------------------------------------
			/// Set Highlight Sprite Sheet Index ID
			///
			/// @param Index ID of highlight state on sprite sheet
			//-----------------------------------------------------------
			void SetHighlightSpriteSheetIndexID(const std::string& instrID);
			//-----------------------------------------------------------
			/// Get Normal Sprite Sheet Index ID
			///
			/// @return Index ID of default state on sprite sheet
			//-----------------------------------------------------------
			const std::string& GetNormalSpriteSheetIndexID() const;
			//-----------------------------------------------------------
			/// Get Highlight Sprite Sheet Index ID
			///
			/// @return Index ID of highlight state on sprite sheet
			//-----------------------------------------------------------
			const std::string& GetHighlightSpriteSheetIndexID() const;
            //-----------------------------------------------------------
            /// Set Default Select Audio Effect
            ///
            /// @param Audio effect played when the button is selected
            //-----------------------------------------------------------
            static void SetDefaultSelectAudioEffect(Core::STORAGE_LOCATION ineLocation, const std::string instrAudioEffect);
			//-----------------------------------------------------------
            /// Set Default De-Select Audio Effect
            ///
            /// @param Audio effect played when the button is selected
            //-----------------------------------------------------------
            static void SetDefaultDeselectAudioEffect(Core::STORAGE_LOCATION ineLocation, const std::string instrAudioEffect);
            //-----------------------------------------------------------
            /// Set Select Audio Effect
            ///
            /// @param Audio effect played when the button is selected
            //-----------------------------------------------------------
            void SetSelectAudioEffect(const Audio::AudioComponentPtr& inpEffect);
			//-----------------------------------------------------------
            /// Set De-Select Audio Effect
            ///
            /// @param Audio effect played when the button is selected
            //-----------------------------------------------------------
            void SetDeselectAudioEffect(const Audio::AudioComponentPtr& inpEffect);
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
			Core::CVector2 GetSizeFromImage() const;
            //--------------------------------------------------------
			/// Set Highlight Colour
			///
			/// @param Tint colour to apply when the button is
			/// selected
			//--------------------------------------------------------
			void SetHighlightColour(const Core::CColour & inValue);
			//--------------------------------------------------------
			/// Get Highlight Colour
			///
			/// @return Tint colour to apply when the button is
			/// selected
			//--------------------------------------------------------
			const Core::CColour & GetHighlightColour() const;
			//--------------------------------------------------------
			/// Layout Content
			///
			/// Called when the view is able to retrieve an absolute
			/// value.
			//--------------------------------------------------------
			void LayoutContent();
			
        private:
            //-----------------------------------------------------------
            /// On Button Select
            ///
            /// Triggered when a button event happens that should
            /// cause the button to highlight
            ///
            /// @param Button that triggered event
            /// @param Touch info
            //-----------------------------------------------------------
            void OnButtonSelect(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo);
            //-----------------------------------------------------------
            /// On Button Activated
            ///
            /// Triggered when a button event happens that should
            /// cause the button to fire
            ///
            /// @param Button that triggered event
            /// @param Touch info
            //-----------------------------------------------------------
            void OnButtonActivated(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo);
            //-----------------------------------------------------------
            /// On Button De-select
            ///
            /// Triggered when a button event happens that should
            /// cause the button to return to normal
            ///
            /// @param Button that triggered event
            /// @param Touch info
            //-----------------------------------------------------------
            void OnButtonDeselect(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo);
            //-----------------------------------------------------------
            /// On Button De-select Threshold
            ///
            /// Triggered when a button event happens that should
            /// cause the button to return to normal
            ///
            /// @param Button that triggered event
            /// @param Touch info
            //-----------------------------------------------------------
            void OnButtonDeselectThreshold(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo);
			//-----------------------------------------------------------
			/// Draw
			///
			/// Overloaded draw method
			///
			/// @param Canvas renderer
			//-----------------------------------------------------------
			void Draw(Rendering::CCanvasRenderer* inpCanvasRenderer);
            
        private:
            
            UnifiedVector2 mvFillMaintainTarget;
            UnifiedVector2 mvFitMaintainTarget;
            
            bool mbFillMaintain;
            bool mbFitMaintain;
			
			//---Properties
			DECLARE_PROPERTY_A(Rendering::TexturePtr, NormalTexture, SetNormalImage, GetNormalImage);
			DECLARE_PROPERTY_A(Rendering::TexturePtr, HighlightTexture, SetHighlightImage, GetHighlightImage);
			DECLARE_PROPERTY_A(Rendering::SpriteSheetPtr, NormalSpriteSheet, SetNormalSpriteSheet, GetNormalSpriteSheet);
			DECLARE_PROPERTY_A(Rendering::SpriteSheetPtr, HighlightSpriteSheet, SetHighlightSpriteSheet, GetHighlightSpriteSheet);

			DECLARE_PROPERTY_A(Core::CColour, HighlightColour, SetHighlightColour, GetHighlightColour);

			DECLARE_PROPERTY_A(u32, NormalSpriteSheetIndex, SetNormalSpriteSheetIndex, GetNormalSpriteSheetIndex);
			DECLARE_PROPERTY_A(u32, HighlightSpriteSheetIndex, SetHighlightSpriteSheetIndex, GetHighlightSpriteSheetIndex);

			DECLARE_PROPERTY_A(std::string, NormalSpriteSheetIndexID, SetNormalSpriteSheetIndexID, GetNormalSpriteSheetIndexID);
			DECLARE_PROPERTY_A(std::string, HighlightSpriteSheetIndexID, SetHighlightSpriteSheetIndexID, GetHighlightSpriteSheetIndexID);

			DECLARE_PROPERTY_A(bool, SizeFromImage, EnableSizeFromImage, IsSizeFromImageEnabled);
            DECLARE_PROPERTY_A(bool, WidthFromImage, EnableSizeFromImage, IsSizeFromImageEnabled);
            DECLARE_PROPERTY_A(bool, HeightFromImage, EnableSizeFromImage, IsSizeFromImageEnabled);
			DECLARE_PROPERTY_A(bool, HeightMaintain, EnableHeightMaintainingAspect, IsHeightMaintainingAspectEnabled);
			DECLARE_PROPERTY_A(bool, WidthMaintain, EnableWidthMaintainingAspect, IsWidthMaintainingAspectEnabled);

            ImageViewPtr mpBackgroundImage;
 
            Audio::AudioComponentPtr mpSelectAudioEffect;
			Audio::AudioComponentPtr mpDeselectAudioEffect;
			
            Core::Rectangle msDefaultUVs;
            Core::Rectangle msHighlightUVs;
            
			bool mbSelected;
            
            static Audio::AudioComponentPtr mspDefaultSelectSound;
            static Audio::AudioComponentPtr mspDefaultDeSelectSound;
		};
        
        typedef SHARED_PTR<CHighlightButton> HighlightButtonPtr;
		typedef WEAK_PTR<CHighlightButton> HighlightButtonWeakPtr;
    }
}

#endif