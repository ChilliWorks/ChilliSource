//
//	ToggleButton.h
//  moFlo
//
//  Created by Scott Downie on 14/12/2012.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_DEFAULTS_TOGGLEBUTTON_H_
#define _MOFLO_GUI_DEFAULTS_TOGGLEBUTTON_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Button/Button.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>

namespace ChilliSource
{
    namespace GUI
    {
        class ToggleButton : public Button
        {
        public:

			DECLARE_META_CLASS(ToggleButton)

            ToggleButton();
            ToggleButton(const Core::ParamDictionary& insParams);
            virtual ~ToggleButton();
            //-----------------------------------------------------------
            /// Get Background Image View
            ///
            /// @return Image view
            //-----------------------------------------------------------
            const ImageViewSPtr& GetBackgroundImageView() const;
            //-----------------------------------------------------------
            /// Set On Image
            ///
            /// Image for unselected button
            ///
            /// @param Texture shared pointer
            //-----------------------------------------------------------
            void SetOnImage(const Rendering::TextureSPtr& inpTexture);
			//-----------------------------------------------------------
			/// Get On Image
			///
			/// @return Texture for non-selected button
			//-----------------------------------------------------------
			const Rendering::TextureSPtr& GetOnImage() const;
            //-----------------------------------------------------------
            /// Set Off Image
            ///
            /// Image for selected button
            ///
            /// @param Texture shared pointer
            //-----------------------------------------------------------
            void SetOffImage(const Rendering::TextureSPtr& inpTexture);
			//-----------------------------------------------------------
			/// Get Off Image
			///
			/// @return Texture for selected button
			//-----------------------------------------------------------
			const Rendering::TextureSPtr& GetOffImage() const;
            //-----------------------------------------------------------
            /// Set On UVs
            ///
            /// Sets the UV coordinates for the default texture
            ///
            /// @param Rectangle for the UVs
            //-----------------------------------------------------------
            void SetOnUVs(Core::Rectangle insUVs);
            //-----------------------------------------------------------
            /// Set Off UVs
            ///
            /// Sets the UV coordinates for the Off texture
            ///
            /// @param Rectangle for the UVs
            //-----------------------------------------------------------
            void SetOffUVs(Core::Rectangle insUVs);
            //-----------------------------------------------------------
            /// Set On Sprite Sheet
            ///
            /// @param Sprite sheet with with On image
            //-----------------------------------------------------------
            void SetOnTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
			//-----------------------------------------------------------
			/// Set Off Sprite Sheet
			///
			/// @param Sprite sheet with with Off image
			//-----------------------------------------------------------
			void SetOffTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas);
            //-----------------------------------------------------------
            /// Get On Sprite Sheet
            ///
            /// @return Sprite sheet with default image
            //-----------------------------------------------------------
			const Rendering::TextureAtlasCSPtr& GetOnTextureAtlas() const;
			//-----------------------------------------------------------
			/// Get Off Sprite Sheet
			///
			/// @return Sprite sheet with Off image
			//-----------------------------------------------------------
			const Rendering::TextureAtlasCSPtr& GetOffTextureAtlas() const;
			//-----------------------------------------------------------
			/// Set On Sprite Sheet Index
			///
			/// @param Index of default state on sprite sheet
			//-----------------------------------------------------------
			void SetOnTextureAtlasID(const std::string& instrID);
			//-----------------------------------------------------------
			/// Set Off Sprite Sheet Index ID
			///
			/// @param Index ID of Off state on sprite sheet
			//-----------------------------------------------------------
			void SetOffTextureAtlasID(const std::string& instrID);
			//-----------------------------------------------------------
			/// Get On Sprite Sheet Index ID
			///
			/// @return Index ID of default state on sprite sheet
			//-----------------------------------------------------------
			const std::string& GetOnTextureAtlasID() const;
			//-----------------------------------------------------------
			/// Get Off Sprite Sheet Index ID
			///
			/// @return Index ID of Off state on sprite sheet
			//-----------------------------------------------------------
			const std::string& GetOffTextureAtlasID() const;
            //-----------------------------------------------------------
            /// Set Default Select Audio Effect
            ///
            /// @param Audio effect played when the button is selected
            //-----------------------------------------------------------
            static void SetDefaultSelectAudioEffect(Core::StorageLocation ineLocation, const std::string instrAudioEffect);
			//-----------------------------------------------------------
            /// Set Default De-Select Audio Effect
            ///
            /// @param Audio effect played when the button is selected
            //-----------------------------------------------------------
            static void SetDefaultDeselectAudioEffect(Core::StorageLocation ineLocation, const std::string instrAudioEffect);
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
			/// Get Size From Image
			///
			/// @return Absolute size of the image
			//--------------------------------------------------------
			Core::Vector2 GetSizeFromImage() const;
			//--------------------------------------------------------
			/// Layout Content
			///
			/// Called when the view is able to retrieve an absolute
			/// value.
			//--------------------------------------------------------
			void LayoutContent();
            //--------------------------------------------------------
            /// Toggle On
            ///
            /// Force the look of the button to be toggled on
            /// this will not generate a button activated event
            ///-------------------------------------------------------
            virtual void ToggleOn();
            //--------------------------------------------------------
            /// Toggle Off
            ///
            /// Force the look of the button to be toggled on
            /// this will not generate a button activated event
            ///-------------------------------------------------------
            virtual void ToggleOff();
            //--------------------------------------------------------
            /// Set Toggle
            ///
            /// @param Whether to toggle the button on or off
            ///-------------------------------------------------------
            void SetToggle(bool inbToggle);
            //--------------------------------------------------------
            /// Is Toggled
            ///
            /// @return Whether the button is toggled on or not
            ///-------------------------------------------------------
            bool IsToggled() const;
			
        protected:
            //-----------------------------------------------------------
            /// Triggered when a button event happens that should
            /// cause the button to Off
            ///
            /// @author S Downie
            ///
            /// @param Button that triggered event
            /// @param The pointer.
            //-----------------------------------------------------------
            virtual void OnButtonSelect(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer);
            //-----------------------------------------------------------
            /// Triggered when a button event happens that should
            /// cause the button to fire
            ///
            /// @author S Downie
            ///
            /// @param Button that triggered event
            /// @param The pointer.
            //-----------------------------------------------------------
            virtual void OnButtonActivated(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer);
            //-----------------------------------------------------------
            /// Triggered when a button event happens that should
            /// cause the button to return to On
            ///
            /// @author S Downie
            ///
            /// @param Button that triggered event
            /// @param The pointer.
            //-----------------------------------------------------------
            virtual void OnButtonDeselect(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer);
            //-----------------------------------------------------------
            /// Triggered when a button event happens that should
            /// cause the button to return to On
            ///
            /// @author S Downie
            ///
            /// @param Button that triggered event
            /// @param The pointer.
            //-----------------------------------------------------------
            virtual void OnButtonDeselectThreshold(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer);
			//-----------------------------------------------------------
			/// Draw
			///
			/// Overloaded draw method
			///
			/// @param Canvas renderer
			//-----------------------------------------------------------
			void Draw(Rendering::CanvasRenderer* inpCanvasRenderer);
            
        protected:
			//---Properties
			DECLARE_PROPERTY_A(Rendering::TextureSPtr, OnTexture, SetOnImage, GetOnImage);
			DECLARE_PROPERTY_A(Rendering::TextureSPtr, OffTexture, SetOffImage, GetOffImage);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, OnTextureAtlas, SetOnTextureAtlas, GetOnTextureAtlas);
			DECLARE_PROPERTY_A(Rendering::TextureAtlasCSPtr, OffTextureAtlas, SetOffTextureAtlas, GetOffTextureAtlas);

			DECLARE_PROPERTY_A(std::string, OnTextureAtlasID, SetOnTextureAtlasID, GetOnTextureAtlasID);
			DECLARE_PROPERTY_A(std::string, OffTextureAtlasID, SetOffTextureAtlasID, GetOffTextureAtlasID);

			DECLARE_PROPERTY_A(bool, SizeFromImage, EnableSizeFromImage, IsSizeFromImageEnabled);
			DECLARE_PROPERTY_A(bool, HeightMaintain, EnableHeightMaintainingAspect, IsHeightMaintainingAspectEnabled);
			DECLARE_PROPERTY_A(bool, WidthMaintain, EnableWidthMaintainingAspect, IsWidthMaintainingAspectEnabled);

            ImageViewSPtr mpBackgroundImage;
 
            Audio::AudioComponentSPtr mpSelectAudioEffect;
			Audio::AudioComponentSPtr mpDeselectAudioEffect;
            
            Core::ConnectionUPtr m_pressedInsideConnection;
            Core::ConnectionUPtr m_releasedInsideConnection;
            Core::ConnectionUPtr m_movedWithinConnection;
            Core::ConnectionUPtr m_movedOutsideConnection;
			
            Core::Rectangle msOnUVs;
            Core::Rectangle msOffUVs;
            
            Core::Vector2 mvSelectedPos;
            
			bool mbSelected;
            bool mbToggledOn;
            
            static Audio::AudioComponentSPtr mspDefaultSelectSound;
            static Audio::AudioComponentSPtr mspDefaultDeSelectSound;
		};
    }
}

#endif