//
//  HighlightButton.cpp
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Button/StretchableHighlightButton.h>
#include <ChilliSource/GUI/Image/ImageView.h>

#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
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
        
        DEFINE_PROPERTY(NormalSpriteSheet);
        DEFINE_PROPERTY(HighlightSpriteSheet);
        DEFINE_PROPERTY(BaseNormalSpriteSheetIndexID);
        DEFINE_PROPERTY(BaseHighlightSpriteSheetIndexID);
        DEFINE_PROPERTY(HighlightColour);
        DEFINE_PROPERTY(HeightMaintain);
        DEFINE_PROPERTY(WidthMaintain);
        
		//-----------------------------------------------------------
        /// Constructor
        //-----------------------------------------------------------
        StretchableHighlightButton::StretchableHighlightButton()
        :   HighlightColour(0.7f, 0.7f, 0.7f, 1.0f), mbSelected(false), HeightMaintain(false), WidthMaintain(false), mCurrentColour(Core::Colour::WHITE)
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
            
            //---Sprite sheet
            Core::StorageLocation eNormalSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("NormalSpriteSheetLocation", strValue))
            {
                eNormalSpriteSheetLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("NormalSpriteSheet", strValue))
            {
                SetNormalSpriteSheet(LOAD_RESOURCE(Rendering::SpriteSheet, eNormalSpriteSheetLocation, strValue));
            }
            //---Sprite sheet
            Core::StorageLocation eHighlightSpriteSheetLocation = Core::StorageLocation::k_package;
            if(insParams.TryGetValue("HighlightSpriteSheetLocation", strValue))
            {
                eHighlightSpriteSheetLocation = ChilliSource::Core::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("HighlightSpriteSheet", strValue))
            {
                SetHighlightSpriteSheet(LOAD_RESOURCE(Rendering::SpriteSheet, eHighlightSpriteSheetLocation, strValue));
            }
			//---Default index ID
			if(insParams.TryGetValue("BaseNormalSpriteSheetIndexID", strValue))
			{
				SetBaseNormalSpriteSheetIndexID(strValue);
			}
			//---Highlight index ID
			if(insParams.TryGetValue("BaseHighlightSpriteSheetIndexID", strValue))
			{
				SetBaseHighlightSpriteSheetIndexID(strValue);
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
        /// Set Normal Sprite Sheet
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetNormalSpriteSheet(const Rendering::SpriteSheetSPtr& inpSpriteSheet)
        {
            NormalSpriteSheet = inpSpriteSheet;
            mCurrentSpriteSheet = NormalSpriteSheet;
            
            if(!HighlightSpriteSheet)
            {
                SetHighlightSpriteSheet(inpSpriteSheet);
            }
        }
        //-----------------------------------------------------------
        /// Set Highlight Sprite Sheet
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetHighlightSpriteSheet(const Rendering::SpriteSheetSPtr& inpSpriteSheet)
        {
            HighlightSpriteSheet = inpSpriteSheet;
            
            if(!NormalSpriteSheet)
            {
                SetNormalSpriteSheet(inpSpriteSheet);
            }
        }
        //-----------------------------------------------------------
        /// Get Normal Sprite Sheet
        //-----------------------------------------------------------
        const Rendering::SpriteSheetSPtr& StretchableHighlightButton::GetNormalSpriteSheet() const
        {
            return NormalSpriteSheet; 
        }
        //-----------------------------------------------------------
        /// Get Highlight Sprite Sheet
        //-----------------------------------------------------------
        const Rendering::SpriteSheetSPtr& StretchableHighlightButton::GetHighlightSpriteSheet() const
        {
            return HighlightSpriteSheet; 
        }
        //-----------------------------------------------------------
        /// Set Base Normal Sprite Sheet Index ID
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetBaseNormalSpriteSheetIndexID(const std::string& instrID)
        {
            if(NormalSpriteSheet)
			{
				BaseNormalSpriteSheetIndexID = instrID;
                
				msNormalIndices.udwTopLeft = NormalSpriteSheet->GetFrameIndexByID(instrID + "TOP_LEFT");
				msNormalIndices.udwTopRight = NormalSpriteSheet->GetFrameIndexByID(instrID + "TOP_RIGHT");
				msNormalIndices.udwBottomLeft = NormalSpriteSheet->GetFrameIndexByID(instrID + "BOTTOM_LEFT");
				msNormalIndices.udwBottomRight = NormalSpriteSheet->GetFrameIndexByID(instrID + "BOTTOM_RIGHT");
				msNormalIndices.udwTopCentre = NormalSpriteSheet->GetFrameIndexByID(instrID + "TOP_CENTRE");
				msNormalIndices.udwBottomCentre = NormalSpriteSheet->GetFrameIndexByID(instrID + "BOTTOM_CENTRE");
				msNormalIndices.udwLeftCentre = NormalSpriteSheet->GetFrameIndexByID(instrID + "MIDDLE_LEFT");
				msNormalIndices.udwRightCentre = NormalSpriteSheet->GetFrameIndexByID(instrID + "MIDDLE_RIGHT");
				msNormalIndices.udwMiddleCentre = NormalSpriteSheet->GetFrameIndexByID(instrID + "MIDDLE_CENTRE");
			}
            
            msCurrentIndices = msNormalIndices;
        }
        //-----------------------------------------------------------
        /// Set Base Highlight Sprite Sheet Index ID
        //-----------------------------------------------------------
        void StretchableHighlightButton::SetBaseHighlightSpriteSheetIndexID(const std::string& instrID)
        {
            if(HighlightSpriteSheet)
			{
				BaseHighlightSpriteSheetIndexID = instrID;
                
				msHighlightIndices.udwTopLeft = HighlightSpriteSheet->GetFrameIndexByID(instrID + "TOP_LEFT");
				msHighlightIndices.udwTopRight = HighlightSpriteSheet->GetFrameIndexByID(instrID + "TOP_RIGHT");
				msHighlightIndices.udwBottomLeft = HighlightSpriteSheet->GetFrameIndexByID(instrID + "BOTTOM_LEFT");
				msHighlightIndices.udwBottomRight = HighlightSpriteSheet->GetFrameIndexByID(instrID + "BOTTOM_RIGHT");
				msHighlightIndices.udwTopCentre = HighlightSpriteSheet->GetFrameIndexByID(instrID + "TOP_CENTRE");
				msHighlightIndices.udwBottomCentre = HighlightSpriteSheet->GetFrameIndexByID(instrID + "BOTTOM_CENTRE");
				msHighlightIndices.udwLeftCentre = HighlightSpriteSheet->GetFrameIndexByID(instrID + "MIDDLE_LEFT");
				msHighlightIndices.udwRightCentre = HighlightSpriteSheet->GetFrameIndexByID(instrID + "MIDDLE_RIGHT");
				msHighlightIndices.udwMiddleCentre = HighlightSpriteSheet->GetFrameIndexByID(instrID + "MIDDLE_CENTRE");
			}
        }
        //-----------------------------------------------------------
        /// Get Base Normal Sprite Sheet Index ID
        //-----------------------------------------------------------
        const std::string& StretchableHighlightButton::GetBaseNormalSpriteSheetIndexID() const
        {
            return BaseNormalSpriteSheetIndexID;
        }
        //-----------------------------------------------------------
        /// Get Base Highlight Sprite Sheet Index ID
        //-----------------------------------------------------------
        const std::string& StretchableHighlightButton::GetBaseHighlightSpriteSheetIndexID() const
        {
            return BaseHighlightSpriteSheetIndexID;
        }
        //---------------------------------------------------------
        /// Set Normal Sprite Sheet Indices
        //---------------------------------------------------------
        void StretchableHighlightButton::SetNormalSpriteSheetIndices(const SpriteSheetIndex& insIndices)
        {
            msNormalIndices = insIndices;
            msCurrentIndices = msNormalIndices;
        }
        //---------------------------------------------------------
        /// Set Normal Sprite Sheet Indices
        //---------------------------------------------------------
        void StretchableHighlightButton::SetNormalSpriteSheetIndices(const u32* inpIndices)
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
        void StretchableHighlightButton::SetHighlightSpriteSheetIndices(const SpriteSheetIndex& insIndices)
        {
            msHighlightIndices = insIndices;
        }
        //---------------------------------------------------------
        /// Set Highlight Sprite Sheet Indices
        //---------------------------------------------------------
        void StretchableHighlightButton::SetHighlightSpriteSheetIndices(const u32* inpIndices)
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
			
            if(Visible && mCurrentSpriteSheet)
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
                Core::Vector2 vTLPatchSize = mCurrentSpriteSheet->GetSizeForFrame(msCurrentIndices.udwTopLeft);
                Core::Vector2 vTRPatchSize = mCurrentSpriteSheet->GetSizeForFrame(msCurrentIndices.udwTopRight);
                Core::Vector2 vBLPatchSize = mCurrentSpriteSheet->GetSizeForFrame(msCurrentIndices.udwBottomLeft);
                Core::Vector2 vBRPatchSize = mCurrentSpriteSheet->GetSizeForFrame(msCurrentIndices.udwBottomRight);
                Core::Vector2 vMLPatchSize = mCurrentSpriteSheet->GetSizeForFrame(msCurrentIndices.udwLeftCentre);
                Core::Vector2 vMRPatchSize = mCurrentSpriteSheet->GetSizeForFrame(msCurrentIndices.udwRightCentre);
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
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwTopLeft), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topLeft);
                
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_topRight));
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vTRPatchSize,  
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwTopRight), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_topRight);
                
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft));
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vBLPatchSize, 
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwBottomLeft), 
                                   AbsColour, 
                                   Rendering::AlignmentAnchor::k_bottomLeft);
                
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor::k_bottomRight));
                Core::Matrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vBRPatchSize,  
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwBottomRight), 
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
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwTopCentre), 
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
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwBottomCentre), 
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
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwLeftCentre), 
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
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwRightCentre), 
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
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwMiddleCentre), 
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
        /// On Button Select
        //-----------------------------------------------------------
        void StretchableHighlightButton::OnButtonSelect(GUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
            if(!mbSelected)
			{
                mvSelectedPos = insTouchInfo.vLocation;
                
				mbSelected = true;
				
				if(mpSelectAudioEffect && !mpSelectAudioEffect->IsPlaying())
				{
					mpSelectAudioEffect->Play();
				}
				
				if (NormalSpriteSheet)
				{
					bool bUniqueHighlight = BaseHighlightSpriteSheetIndexID != BaseNormalSpriteSheetIndexID;
					
					if (bUniqueHighlight && HighlightSpriteSheet)
					{
						msCurrentIndices = msHighlightIndices;
                        mCurrentSpriteSheet = HighlightSpriteSheet;
					} 
					
                    mCurrentColour = HighlightColour;
				}
			}
        }
        //-----------------------------------------------------------
        /// On Button Activated
        //-----------------------------------------------------------
        void StretchableHighlightButton::OnButtonActivated(GUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
            if(mbSelected)
			{
				OnButtonDeselect(inpButton, insTouchInfo);
				mOnButtonActivatedEvent.NotifyConnections(this);
			}
        }
        //-----------------------------------------------------------
        /// On Button De-select
        //-----------------------------------------------------------
        void StretchableHighlightButton::OnButtonDeselect(GUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
            if(mbSelected)
			{
				if(mpDeselectAudioEffect && !mpDeselectAudioEffect->IsPlaying())
				{
					mpDeselectAudioEffect->Play();
				}
				
				if (NormalSpriteSheet)
				{
					mCurrentColour = Core::Colour::WHITE;
					mCurrentSpriteSheet = NormalSpriteSheet;
					msCurrentIndices = msNormalIndices;
				}
				
				mbSelected = false;
			}
        }
        //-----------------------------------------------------------
        /// On Button De-select Threshold
        //-----------------------------------------------------------
        void StretchableHighlightButton::OnButtonDeselectThreshold(GUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
            const f32 kfThreshold = (f32)(Core::Screen::GetOrientedWidth()) * 0.02f;
            f32 fDistX = std::abs(insTouchInfo.vLocation.x - mvSelectedPos.x);
            f32 fDisty = std::abs(insTouchInfo.vLocation.y - mvSelectedPos.y);
			if(fDistX >= kfThreshold || fDisty >= kfThreshold)
			{
                OnButtonDeselect(inpButton, insTouchInfo);
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
