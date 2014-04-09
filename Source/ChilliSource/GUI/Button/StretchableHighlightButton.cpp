//
//  HighlightButton.cpp
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Button/StretchableHighlightButton.h>
#include <ChilliSource/GUI/Image/ImageView.h>

#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/Entity/ComponentFactoryDispenser.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <ChilliSource/Audio/3D/AudioComponent.h>
#include <ChilliSource/Audio/3D/AudioComponentFactory.h>

#include <cmath>

namespace ChilliSource
{
    namespace GUI
    {
        DEFINE_META_CLASS(StretchableHighlightButton);
        
        DEFINE_PROPERTY(NormalTextureAtlas);
        DEFINE_PROPERTY(HighlightTextureAtlas);
        DEFINE_PROPERTY(BaseNormalTextureAtlasID);
        DEFINE_PROPERTY(BaseHighlightTextureAtlasID);
        DEFINE_PROPERTY(HighlightColour);
        DEFINE_PROPERTY(HeightMaintain);
        DEFINE_PROPERTY(WidthMaintain);
        
		//-----------------------------------------------------------
        /// Constructor
        //-----------------------------------------------------------
        StretchableHighlightButton::StretchableHighlightButton()
        :   HighlightColour(0.7f, 0.7f, 0.7f, 1.0f), mbSelected(false), HeightMaintain(false), WidthMaintain(false), mCurrentColour(Core::Colour::k_white)
        {
            memset(&msNormalIndices, 0, sizeof(u32) * 9);
            memset(&msHighlightIndices, 0, sizeof(u32) * 9);
            memset(&msCurrentIndices, 0, sizeof(u32) * 9);
            
            m_pressedInsideConnection = mInputEvents.GetPressedInsideEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonSelect));
            m_releasedInsideConnection = mInputEvents.GetReleasedInsideEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonActivated));
            m_movedWithinConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonDeselectThreshold));
            m_movedOutsideConnection = mInputEvents.GetMovedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonDeselect));
        }
        //-----------------------------------------------------------
        /// Constructor
        //-----------------------------------------------------------
        StretchableHighlightButton::StretchableHighlightButton(const Core::ParamDictionary& insParams)
        : Button(insParams), HighlightColour(0.7f, 0.7f, 0.7f, 1.0f), mbSelected(false), HeightMaintain(false), WidthMaintain(false)
        {
            std::string strValue;
            
            memset(&msNormalIndices, 0, sizeof(u32) * 9);
            memset(&msHighlightIndices, 0, sizeof(u32) * 9);
            memset(&msCurrentIndices, 0, sizeof(u32) * 9);
            
            //---Texture
            Core::StorageLocation eNormalTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("NormalTextureLocation", strValue))
            {
                eNormalTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("NormalTexture", strValue))
            {
                SetNormalTexture(LOAD_RESOURCE(Rendering::Texture, eNormalTextureLocation, strValue));
            }
            //---Texture
            Core::StorageLocation eHighlightTextureLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("HighlightTextureLocation", strValue))
            {
                eHighlightTextureLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("HighlightTexture", strValue))
            {
                SetHighlightTexture(LOAD_RESOURCE(Rendering::Texture, eHighlightTextureLocation, strValue));
            }
            
            //---Sprite sheet
            Core::StorageLocation eNormalTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("NormalTextureAtlasLocation", strValue))
            {
                eNormalTextureAtlasLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("NormalTextureAtlas", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetNormalTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eNormalTextureAtlasLocation, strValue));
            }
            //---Sprite sheet
            Core::StorageLocation eHighlightTextureAtlasLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("HighlightTextureAtlasLocation", strValue))
            {
                eHighlightTextureAtlasLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("HighlightTextureAtlas", strValue))
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                SetNormalTextureAtlas(resourcePool->LoadResource<Rendering::TextureAtlas>(eHighlightTextureAtlasLocation, strValue));
            }
			//---Default index ID
			if(insParams.TryGetValue("BaseNormalTextureAtlasID", strValue))
			{
				SetBaseNormalTextureAtlasID(strValue);
			}
			//---Highlight index ID
			if(insParams.TryGetValue("BaseHighlightTextureAtlasID", strValue))
			{
				SetBaseHighlightTextureAtlasID(strValue);
			}
            //---Highlight Colour
            if(insParams.TryGetValue("HighlightColour", strValue))
            {
                SetHighlightColour(Core::ParseColour(strValue));
            }
            //---Audio effect
            Core::StorageLocation eSelectAudioLocation = Core::StorageLocation::k_package;
            Core::StorageLocation eDeselectAudioLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("SelectAudioEffectLocation", strValue))
            {
                eSelectAudioLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("DeselectAudioEffectLocation", strValue))
            {
                eDeselectAudioLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("SelectAudioEffect", strValue))
            {
                Audio::AudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::AudioComponentFactory);
                SetSelectAudioEffect(pAudioFactory->CreateAudioComponent(eSelectAudioLocation, strValue, false, false));
            }
			if(insParams.TryGetValue("DeselectAudioEffect", strValue))
            {
                Audio::AudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::AudioComponentFactory);
                SetDeselectAudioEffect(pAudioFactory->CreateAudioComponent(eDeselectAudioLocation, strValue, false, false));
            }
            //---Normal Sprite sheet indices
            if(insParams.TryGetValue("NormalTopLeftIndex", strValue))
            {
                msNormalIndices.udwTopLeft = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("NormalTopRightIndex", strValue))
            {
                msNormalIndices.udwTopRight = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("NormalBottomLeftIndex", strValue))
            {
                msNormalIndices.udwBottomLeft = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("NormalBottomRightIndex", strValue))
            {
                msNormalIndices.udwBottomRight = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("NormalTopIndex", strValue))
            {
                msNormalIndices.udwTopCentre = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("NormalBottomIndex", strValue))
            {
                msNormalIndices.udwBottomCentre = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("NormalLeftIndex", strValue))
            {
                msNormalIndices.udwLeftCentre = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("NormalRightIndex", strValue))
            {
                msNormalIndices.udwRightCentre = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("NormalCentreIndex", strValue))
            {
                msNormalIndices.udwMiddleCentre = Core::ParseU32(strValue);
            }
            //---Highlight Sprite sheet indices
            if(insParams.TryGetValue("HighlightTopLeftIndex", strValue))
            {
                msHighlightIndices.udwTopLeft = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("HighlightTopRightIndex", strValue))
            {
                msHighlightIndices.udwTopRight = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("HighlightBottomLeftIndex", strValue))
            {
                msHighlightIndices.udwBottomLeft = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("HighlightBottomRightIndex", strValue))
            {
                msHighlightIndices.udwBottomRight = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("HighlightTopIndex", strValue))
            {
                msHighlightIndices.udwTopCentre = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("HighlightBottomIndex", strValue))
            {
                msHighlightIndices.udwBottomCentre = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("HighlightLeftIndex", strValue))
            {
                msHighlightIndices.udwLeftCentre = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("HighlightRightIndex", strValue))
            {
                msHighlightIndices.udwRightCentre = Core::ParseU32(strValue);
            }
            if(insParams.TryGetValue("HighlightCentreIndex", strValue))
            {
                msHighlightIndices.udwMiddleCentre = Core::ParseU32(strValue);
            }
			//---Width maintain
			if(insParams.TryGetValue("WidthMaintain", strValue))
            {
				EnableWidthMaintainingAspect(Core::ParseBool(strValue));
			}
			//---Height maintain
			if(insParams.TryGetValue("HeightMaintain", strValue))
            {
				EnableHeightMaintainingAspect(Core::ParseBool(strValue));
			}
            
            m_pressedInsideConnection = mInputEvents.GetPressedInsideEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonSelect));
            m_releasedInsideConnection = mInputEvents.GetReleasedInsideEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonActivated));
            m_movedWithinConnection = mInputEvents.GetMovedWithinEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonDeselectThreshold));
            m_movedOutsideConnection = mInputEvents.GetMovedOutsideEvent().OpenConnection(Core::MakeDelegate(this, &StretchableHighlightButton::OnButtonDeselect));
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetNormalTexture(const Rendering::TextureSPtr& inpTexture)
        {
            NormalTexture = inpTexture;
            mCurrentTexture = NormalTexture;
            
            if(!HighlightTexture)
            {
                SetHighlightTexture(inpTexture);
            }
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetHighlightTexture(const Rendering::TextureSPtr& inpTexture)
        {
            HighlightTexture = inpTexture;
            
            if(!NormalTexture)
            {
                SetNormalTexture(inpTexture);
            }
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        const Rendering::TextureSPtr& StretchableHighlightButton::GetNormalTexture() const
        {
            return NormalTexture;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        const Rendering::TextureSPtr& StretchableHighlightButton::GetHighlightTexture() const
        {
            return HighlightTexture;
        }
        //-----------------------------------------------------------
        /// Set Normal Sprite Sheet
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetNormalTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
        {
            NormalTextureAtlas = inpTextureAtlas;
            mCurrentTextureAtlas = NormalTextureAtlas;
            
            if(!HighlightTextureAtlas)
            {
                SetHighlightTextureAtlas(inpTextureAtlas);
            }
        }
        //-----------------------------------------------------------
        /// Set Highlight Sprite Sheet
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetHighlightTextureAtlas(const Rendering::TextureAtlasCSPtr& inpTextureAtlas)
        {
            HighlightTextureAtlas = inpTextureAtlas;
            
            if(!NormalTextureAtlas)
            {
                SetNormalTextureAtlas(inpTextureAtlas);
            }
        }
        //-----------------------------------------------------------
        /// Get Normal Sprite Sheet
        //-----------------------------------------------------------
        const Rendering::TextureAtlasCSPtr& StretchableHighlightButton::GetNormalTextureAtlas() const
        {
            return NormalTextureAtlas; 
        }
        //-----------------------------------------------------------
        /// Get Highlight Sprite Sheet
        //-----------------------------------------------------------
        const Rendering::TextureAtlasCSPtr& StretchableHighlightButton::GetHighlightTextureAtlas() const
        {
            return HighlightTextureAtlas; 
        }
        //-----------------------------------------------------------
        /// Set Base Normal Sprite Sheet Index ID
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetBaseNormalTextureAtlasID(const std::string& instrID)
        {
            if(NormalTextureAtlas)
			{
				BaseNormalTextureAtlasID = instrID;
                
				msNormalIndices.udwTopLeft = NormalTextureAtlas->GetFrameIndexById(instrID + "TOP_LEFT");
				msNormalIndices.udwTopRight = NormalTextureAtlas->GetFrameIndexById(instrID + "TOP_RIGHT");
				msNormalIndices.udwBottomLeft = NormalTextureAtlas->GetFrameIndexById(instrID + "BOTTOM_LEFT");
				msNormalIndices.udwBottomRight = NormalTextureAtlas->GetFrameIndexById(instrID + "BOTTOM_RIGHT");
				msNormalIndices.udwTopCentre = NormalTextureAtlas->GetFrameIndexById(instrID + "TOP_CENTRE");
				msNormalIndices.udwBottomCentre = NormalTextureAtlas->GetFrameIndexById(instrID + "BOTTOM_CENTRE");
				msNormalIndices.udwLeftCentre = NormalTextureAtlas->GetFrameIndexById(instrID + "MIDDLE_LEFT");
				msNormalIndices.udwRightCentre = NormalTextureAtlas->GetFrameIndexById(instrID + "MIDDLE_RIGHT");
				msNormalIndices.udwMiddleCentre = NormalTextureAtlas->GetFrameIndexById(instrID + "MIDDLE_CENTRE");
			}
            
            msCurrentIndices = msNormalIndices;
        }
        //-----------------------------------------------------------
        /// Set Base Highlight Sprite Sheet Index ID
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetBaseHighlightTextureAtlasID(const std::string& instrID)
        {
            if(HighlightTextureAtlas)
			{
				BaseHighlightTextureAtlasID = instrID;
                
				msHighlightIndices.udwTopLeft = HighlightTextureAtlas->GetFrameIndexById(instrID + "TOP_LEFT");
				msHighlightIndices.udwTopRight = HighlightTextureAtlas->GetFrameIndexById(instrID + "TOP_RIGHT");
				msHighlightIndices.udwBottomLeft = HighlightTextureAtlas->GetFrameIndexById(instrID + "BOTTOM_LEFT");
				msHighlightIndices.udwBottomRight = HighlightTextureAtlas->GetFrameIndexById(instrID + "BOTTOM_RIGHT");
				msHighlightIndices.udwTopCentre = HighlightTextureAtlas->GetFrameIndexById(instrID + "TOP_CENTRE");
				msHighlightIndices.udwBottomCentre = HighlightTextureAtlas->GetFrameIndexById(instrID + "BOTTOM_CENTRE");
				msHighlightIndices.udwLeftCentre = HighlightTextureAtlas->GetFrameIndexById(instrID + "MIDDLE_LEFT");
				msHighlightIndices.udwRightCentre = HighlightTextureAtlas->GetFrameIndexById(instrID + "MIDDLE_RIGHT");
				msHighlightIndices.udwMiddleCentre = HighlightTextureAtlas->GetFrameIndexById(instrID + "MIDDLE_CENTRE");
			}
        }
        //-----------------------------------------------------------
        /// Get Base Normal Sprite Sheet Index ID
        //-----------------------------------------------------------
        const std::string& StretchableHighlightButton::GetBaseNormalTextureAtlasID() const
        {
            return BaseNormalTextureAtlasID;
        }
        //-----------------------------------------------------------
        /// Get Base Highlight Sprite Sheet Index ID
        //-----------------------------------------------------------
        const std::string& StretchableHighlightButton::GetBaseHighlightTextureAtlasID() const
        {
            return BaseHighlightTextureAtlasID;
        }
        //---------------------------------------------------------
        /// Set Normal Sprite Sheet Indices
        //---------------------------------------------------------
        void StretchableHighlightButton::SetNormalTextureAtlasIndices(const TextureAtlasIndex& insIndices)
        {
            msNormalIndices = insIndices;
            msCurrentIndices = msNormalIndices;
        }
        //---------------------------------------------------------
        /// Set Normal Sprite Sheet Indices
        //---------------------------------------------------------
        void StretchableHighlightButton::SetNormalTextureAtlasIndices(const u32* inpIndices)
        {
            msNormalIndices.udwTopLeft = inpIndices[0];
            msNormalIndices.udwTopRight = inpIndices[1];
            msNormalIndices.udwBottomLeft = inpIndices[2];
            msNormalIndices.udwBottomRight = inpIndices[3];
            msNormalIndices.udwTopCentre = inpIndices[4];
            msNormalIndices.udwBottomCentre = inpIndices[5];
            msNormalIndices.udwLeftCentre = inpIndices[6];
            msNormalIndices.udwRightCentre = inpIndices[7];
            msNormalIndices.udwMiddleCentre = inpIndices[8];
            msCurrentIndices = msNormalIndices;
        }
        //---------------------------------------------------------
        /// Set Highlight Sprite Sheet Indices
        //---------------------------------------------------------
        void StretchableHighlightButton::SetHighlightTextureAtlasIndices(const TextureAtlasIndex& insIndices)
        {
            msHighlightIndices = insIndices;
        }
        //---------------------------------------------------------
        /// Set Highlight Sprite Sheet Indices
        //---------------------------------------------------------
        void StretchableHighlightButton::SetHighlightTextureAtlasIndices(const u32* inpIndices)
        {
            msHighlightIndices.udwTopLeft = inpIndices[0];
            msHighlightIndices.udwTopRight = inpIndices[1];
            msHighlightIndices.udwBottomLeft = inpIndices[2];
            msHighlightIndices.udwBottomRight = inpIndices[3];
            msHighlightIndices.udwTopCentre = inpIndices[4];
            msHighlightIndices.udwBottomCentre = inpIndices[5];
            msHighlightIndices.udwLeftCentre = inpIndices[6];
            msHighlightIndices.udwRightCentre = inpIndices[7];
            msHighlightIndices.udwMiddleCentre = inpIndices[8];
        }
        //-----------------------------------------------------------
        /// Set Select Audio Effect
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetSelectAudioEffect(const Audio::AudioComponentSPtr& inpEffect)
        {
            mpSelectAudioEffect = inpEffect;
        }
        //-----------------------------------------------------------
        /// Set De-Select Audio Effect
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetDeselectAudioEffect(const Audio::AudioComponentSPtr& inpEffect)
        {
            mpDeselectAudioEffect = inpEffect;
        }
        //--------------------------------------------------------
        /// Set Highlight Colour
        //--------------------------------------------------------
        void StretchableHighlightButton::SetHighlightColour(const Core::Colour & inValue)
        {
            HighlightColour = inValue;
        }
        //--------------------------------------------------------
        /// Get Highlight Colour
        //--------------------------------------------------------
        const Core::Colour & StretchableHighlightButton::GetHighlightColour() const
        {
            return HighlightColour;
        }
        //-----------------------------------------------------------
        /// Draw
        //-----------------------------------------------------------
        void StretchableHighlightButton::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            //Check if this is on screen
			Core::Vector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topRight);
			Core::Vector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
			
			if(vTopRight.y < 0 || vBottomLeft.y > Core::Screen::GetOrientedHeight() || vTopRight.x < 0 || vBottomLeft.x > Core::Screen::GetOrientedWidth())
			{
				//Offscreen
				return;
			}
			
            if(Visible && mCurrentTextureAtlas && mCurrentTexture)
            {			
                Core::Vector2 vPanelSize = GetAbsoluteSize();
                Core::Vector2 vPanelPos = GetAbsoluteScreenSpacePosition();
                Core::Vector2 vTopLeft = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topLeft);
                Core::Vector2 vPatchPos;
                
                Core::Colour AbsColour = GetAbsoluteColour() * mCurrentColour;
                
                //We need to use a matrix so that we can rotate all the patches with respect
                //to the view
                Core::Matrix3x3 matTransform;
                Core::Matrix3x3 matPatchTransform;
                Core::Matrix3x3 matViewTransform;
                
                matViewTransform.SetTransform(vPanelPos, Core::Vector2(1, 1), GetAbsoluteRotation());
                
                //Get the patch sizes
                Core::Vector2 vTLPatchSize = mCurrentTextureAtlas->GetFrameSize(msCurrentIndices.udwTopLeft);
                Core::Vector2 vTRPatchSize = mCurrentTextureAtlas->GetFrameSize(msCurrentIndices.udwTopRight);
                Core::Vector2 vBLPatchSize = mCurrentTextureAtlas->GetFrameSize(msCurrentIndices.udwBottomLeft);
                Core::Vector2 vBRPatchSize = mCurrentTextureAtlas->GetFrameSize(msCurrentIndices.udwBottomRight);
                Core::Vector2 vMLPatchSize = mCurrentTextureAtlas->GetFrameSize(msCurrentIndices.udwLeftCentre);
                Core::Vector2 vMRPatchSize = mCurrentTextureAtlas->GetFrameSize(msCurrentIndices.udwRightCentre);
                Core::Vector2 vTCPatchSize;
                Core::Vector2 vBCPatchSize;
                Core::Vector2 vMCPatchSize;
                
                //Check to see if they are going to fit in the bounds of the view
                f32 fTotal = vTLPatchSize.y + vBLPatchSize.y;
                if(fTotal > vPanelSize.y)
                {
                    //The corners are too tall let's
                    //squish them
                    f32 fScale = vPanelSize.y/fTotal;
                    vTLPatchSize.y *= fScale;
                    vBLPatchSize.y *= fScale;
                }
                fTotal = vTRPatchSize.y + vBRPatchSize.y ;
                if(fTotal > vPanelSize.y)
                {
                    //The corners are too tall let's
                    //squish them
                    f32 fScale = vPanelSize.y/fTotal;
                    vTRPatchSize.y *= fScale;
                    vBRPatchSize.y *= fScale;
                }
                fTotal = vTLPatchSize.x + vTRPatchSize.x;
                if(fTotal > vPanelSize.x)
                {
                    //The corners are too tall let's
                    //squish them
                    f32 fScale = vPanelSize.x/fTotal;
                    vTLPatchSize.x *= fScale;
                    vTRPatchSize.x *= fScale;
                }
                fTotal = vTLPatchSize.x + vBRPatchSize.x;
                if(fTotal > vPanelSize.x)
                {
                    //The corners are too tall let's
                    //squish them
                    f32 fScale = vPanelSize.x/fTotal;
                    vBLPatchSize.x *= fScale;
                    vBRPatchSize.x *= fScale;
                }
                
                //Render ourself
                //Draw the corners first
                matPatchTransform.Translate(vTopLeft);
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform,
                                   vTLPatchSize, 
								   mCurrentTexture,
                                   mCurrentTextureAtlas->GetFrameUVs(msCurrentIndices.udwTopLeft), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
                
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topRight));
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vTRPatchSize,  
								   mCurrentTexture,
                                   mCurrentTextureAtlas->GetFrameUVs(msCurrentIndices.udwTopRight), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topRight);
                
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft));
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vBLPatchSize, 
								   mCurrentTexture,
                                   mCurrentTextureAtlas->GetFrameUVs(msCurrentIndices.udwBottomLeft), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_bottomLeft);
                
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomRight));
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vBRPatchSize,  
								   mCurrentTexture,
                                   mCurrentTextureAtlas->GetFrameUVs(msCurrentIndices.udwBottomRight), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_bottomRight);
                
                //Draw the top and bottom 
                vTCPatchSize.x = vPanelSize.x - (vTLPatchSize.x + vTRPatchSize.x);
                vTCPatchSize.y = vTLPatchSize.y;
                vPatchPos.x = vTopLeft.x + vTLPatchSize.x;
                vPatchPos.y = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topCentre).y;
                
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vTCPatchSize, 
								   mCurrentTexture,
                                   mCurrentTextureAtlas->GetFrameUVs(msCurrentIndices.udwTopCentre), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
                
                
                vBCPatchSize.x = vPanelSize.x - (vBLPatchSize.x + vBRPatchSize.x);
                vBCPatchSize.y = vBLPatchSize.y;
                vPatchPos.x = vTopLeft.x + vBLPatchSize.x;
                vPatchPos.y = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomCentre).y;
                
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vBCPatchSize, 
								   mCurrentTexture,
                                   mCurrentTextureAtlas->GetFrameUVs(msCurrentIndices.udwBottomCentre), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_bottomLeft);
                
                //Draw the left and right
                vMLPatchSize.y = vPanelSize.y - (vTLPatchSize.y + vBLPatchSize.y);
                vBCPatchSize.x = vTLPatchSize.x;
                vPatchPos.x = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_middleLeft).x;
                vPatchPos.y = vTopLeft.y - vTLPatchSize.y;
                
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vMLPatchSize, 
								   mCurrentTexture,
                                   mCurrentTextureAtlas->GetFrameUVs(msCurrentIndices.udwLeftCentre), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
                
                vMRPatchSize.y = vPanelSize.y - (vTRPatchSize.y + vBRPatchSize.y);
                vMRPatchSize.x = vTRPatchSize.x;
                vPatchPos.x = GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_middleRight).x;
                vPatchPos.y = vTopLeft.y - vTRPatchSize.y;
                
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vMRPatchSize, 
								   mCurrentTexture,
                                   mCurrentTextureAtlas->GetFrameUVs(msCurrentIndices.udwRightCentre), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topRight);
                
                //Draw the centre
                vMCPatchSize.x = vPanelSize.x - (vMLPatchSize.x + vMRPatchSize.x);
                vMCPatchSize.y = vPanelSize.y - (vTCPatchSize.y + vBCPatchSize.y);
                vPatchPos.x = vTopLeft.x + vTLPatchSize.x;
                vPatchPos.y = vTopLeft.y - vTLPatchSize.y;
                
                matPatchTransform.Translate(vPatchPos);
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform,
                                   vMCPatchSize, 
								   mCurrentTexture,
                                   mCurrentTextureAtlas->GetFrameUVs(msCurrentIndices.udwMiddleCentre), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
                
                //Render subviews
                GUIView::Draw(inpCanvas);
            }
        }
        //--------------------------------------------------------
        /// Set Width Maintaining Aspect
        //--------------------------------------------------------
        void StretchableHighlightButton::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
        {
            Core::Vector2 vCurrentSize = GetAbsoluteSize();
			f32 fAspectRatio = vCurrentSize.y / vCurrentSize.x;
			SetSize(infRelWidth, 0.0f, infAbsWidth, 0.0f);
			
			f32 fScaleY = GetAbsoluteScale().y;
			if(fScaleY == 0.0f)
				return;
			
			vCurrentSize = GetAbsoluteSize();
            f32 fAbsHeight = (fAspectRatio * vCurrentSize.x) / fScaleY;
			SetSize(infRelWidth, 0.0f, infAbsWidth, fAbsHeight);
        }
        //--------------------------------------------------------
        /// Set Height Maintaining Aspect
        //--------------------------------------------------------
        void StretchableHighlightButton::SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight)
        {
            Core::Vector2 vCurrentSize = GetAbsoluteSize();
			f32 fAspectRatio = vCurrentSize.x / vCurrentSize.y;
			SetSize(0.0f, infRelHeight, 0.0f, infAbsHeight);
			
			f32 fScaleX = GetAbsoluteScale().x;
			if(fScaleX == 0.0f)
				return;
			
			vCurrentSize = GetAbsoluteSize();
            f32 fAbsWidth = (fAspectRatio * vCurrentSize.y) / fScaleX;
			SetSize(0.0f, infRelHeight, fAbsWidth, infAbsHeight);
        }
        //--------------------------------------------------------
        /// Enable Width Maintaining Aspect
        //--------------------------------------------------------
        void StretchableHighlightButton::EnableWidthMaintainingAspect(bool inbEnabled)
        {
            WidthMaintain = inbEnabled;  
        }
        //--------------------------------------------------------
        /// Is Width Maintaining Aspect Enabled
        //--------------------------------------------------------
        bool StretchableHighlightButton::IsWidthMaintainingAspectEnabled() const
        {
            return WidthMaintain;
             
        }
        //--------------------------------------------------------
        /// Enable Height Maintaining Aspect
        //--------------------------------------------------------
        void StretchableHighlightButton::EnableHeightMaintainingAspect(bool inbEnabled)
        {
            HeightMaintain = inbEnabled;
        }
        //--------------------------------------------------------
        /// Is Height Maintaining Aspect Enabled
        //--------------------------------------------------------
        bool StretchableHighlightButton::IsHeightMaintainingAspectEnabled() const
        {
            return HeightMaintain;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void StretchableHighlightButton::OnButtonSelect(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer)
        {
            if(!mbSelected)
			{
                mvSelectedPos = in_pointer.m_location;
                
				mbSelected = true;
				
				if(mpSelectAudioEffect && !mpSelectAudioEffect->IsPlaying())
				{
					mpSelectAudioEffect->Play();
				}
				
				if (NormalTextureAtlas)
				{
					bool bUniqueHighlight = BaseHighlightTextureAtlasID != BaseNormalTextureAtlasID;
					
					if (bUniqueHighlight && HighlightTextureAtlas)
					{
						msCurrentIndices = msHighlightIndices;
                        mCurrentTextureAtlas = HighlightTextureAtlas;
					} 
					
                    mCurrentColour = HighlightColour;
				}
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void StretchableHighlightButton::OnButtonActivated(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer)
        {
            if(mbSelected)
			{
				OnButtonDeselect(in_button, in_pointer);
				mOnButtonActivatedEvent.NotifyConnections(this);
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void StretchableHighlightButton::OnButtonDeselect(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer)
        {
            if(mbSelected)
			{
				if(mpDeselectAudioEffect && !mpDeselectAudioEffect->IsPlaying())
				{
					mpDeselectAudioEffect->Play();
				}
				
				if (NormalTextureAtlas)
				{
					mCurrentColour = Core::Colour::k_white;
					mCurrentTextureAtlas = NormalTextureAtlas;
					msCurrentIndices = msNormalIndices;
				}
				
				mbSelected = false;
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void StretchableHighlightButton::OnButtonDeselectThreshold(GUIView* in_button, const Input::PointerSystem::Pointer& in_pointer)
        {
            const f32 kfThreshold = (f32)(Core::Screen::GetOrientedWidth()) * 0.02f;
            f32 fDistX = std::abs(in_pointer.m_location.x - mvSelectedPos.x);
            f32 fDisty = std::abs(in_pointer.m_location.y - mvSelectedPos.y);
			if(fDistX >= kfThreshold || fDisty >= kfThreshold)
			{
                OnButtonDeselect(in_button, in_pointer);
			}
        }
        //-----------------------------------------------------------
        /// Destructor
        //-----------------------------------------------------------
        StretchableHighlightButton::~StretchableHighlightButton()
        {
            if(mpSelectAudioEffect)
            {
                mpSelectAudioEffect->Stop();
            }
			if(mpDeselectAudioEffect)
            {
                mpDeselectAudioEffect->Stop();
            }
        }
    }
}
