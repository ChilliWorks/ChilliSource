//
//  HighlightButton.cpp
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <moFlo/GUI/Defaults/StretchableHighlightButton.h>
#include <moFlo/GUI/ImageView.h>

#include <moFlo/Rendering/SpriteSheet.h>
#include <moFlo/Rendering/SpriteSheetManager.h>
#include <moFlo/Rendering/TextureManager.h>
#include <moFlo/Rendering/Texture.h>
#include <moFlo/Rendering/CanvasRenderer.h>

#include <moFlo/Core/ResourceManagerDispenser.h>
#include <moFlo/Core/ComponentFactoryDispenser.h>
#include <moFlo/Core/Screen.h>

#include <moFlo/Audio/AudioComponent.h>
#include <moFlo/Audio/AudioComponentFactory.h>

namespace moFlo
{
    namespace GUI
    {
        DEFINE_META_CLASS(CStretchableHighlightButton);
        
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
        CStretchableHighlightButton::CStretchableHighlightButton()
        :   HighlightColour(0.7f, 0.7f, 0.7f, 1.0f), mbSelected(false), HeightMaintain(false), WidthMaintain(false), mCurrentColour(Core::CColour::WHITE)
        {
            memset(&msNormalIndices, 0, sizeof(u32) * 9);
            memset(&msHighlightIndices, 0, sizeof(u32) * 9);
            memset(&msCurrentIndices, 0, sizeof(u32) * 9);
            
            mInputEvents.GetPressedInsideEvent() += GUIEventDelegate(this, &CStretchableHighlightButton::OnButtonSelect);
            mInputEvents.GetReleasedInsideEvent() += GUIEventDelegate(this, &CStretchableHighlightButton::OnButtonActivated);
            mInputEvents.GetMovedWithinEvent() += GUIEventDelegate(this, &CStretchableHighlightButton::OnButtonDeselectThreshold);
        }
        //-----------------------------------------------------------
        /// Constructor
        //-----------------------------------------------------------
        CStretchableHighlightButton::CStretchableHighlightButton(const Core::ParamDictionary& insParams)
        : IButton(insParams), HighlightColour(0.7f, 0.7f, 0.7f, 1.0f), mbSelected(false), HeightMaintain(false), WidthMaintain(false)
        {
            std::string strValue;
            
            memset(&msNormalIndices, 0, sizeof(u32) * 9);
            memset(&msHighlightIndices, 0, sizeof(u32) * 9);
            memset(&msCurrentIndices, 0, sizeof(u32) * 9);
            
            //---Sprite sheet
            Core::STORAGE_LOCATION eNormalSpriteSheetLocation = Core::SL_PACKAGE;
            if(insParams.TryGetValue("NormalSpriteSheetLocation", strValue))
            {
                eNormalSpriteSheetLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("NormalSpriteSheet", strValue))
            {
                SetNormalSpriteSheet(LOAD_RESOURCE(Rendering::CSpriteSheet, eNormalSpriteSheetLocation, strValue));
            }
            //---Sprite sheet
            Core::STORAGE_LOCATION eHighlightSpriteSheetLocation = Core::SL_PACKAGE;
            if(insParams.TryGetValue("HighlightSpriteSheetLocation", strValue))
            {
                eHighlightSpriteSheetLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("HighlightSpriteSheet", strValue))
            {
                SetHighlightSpriteSheet(LOAD_RESOURCE(Rendering::CSpriteSheet, eHighlightSpriteSheetLocation, strValue));
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
                SetHighlightColour(Core::CStringConverter::ParseColourValue(strValue));
            }
            //---Audio effect
            Core::STORAGE_LOCATION eSelectAudioLocation = Core::SL_PACKAGE;
            Core::STORAGE_LOCATION eDeselectAudioLocation = Core::SL_PACKAGE;
            if(insParams.TryGetValue("SelectAudioEffectLocation", strValue))
            {
                eSelectAudioLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
			if(insParams.TryGetValue("DeselectAudioEffectLocation", strValue))
            {
                eDeselectAudioLocation = moFlo::Core::CStringConverter::ParseStorageLocation(strValue);
            }
            if(insParams.TryGetValue("SelectAudioEffect", strValue))
            {
                Audio::IAudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::IAudioComponentFactory);
                SetSelectAudioEffect(pAudioFactory->CreateAudioComponent(eSelectAudioLocation, strValue, false, false));
            }
			if(insParams.TryGetValue("DeselectAudioEffect", strValue))
            {
                Audio::IAudioComponentFactory* pAudioFactory = GET_COMPONENT_FACTORY(Audio::IAudioComponentFactory);
                SetDeselectAudioEffect(pAudioFactory->CreateAudioComponent(eDeselectAudioLocation, strValue, false, false));
            }
            //---Normal Sprite sheet indices
            if(insParams.TryGetValue("NormalTopLeftIndex", strValue))
            {
                msNormalIndices.udwTopLeft = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("NormalTopRightIndex", strValue))
            {
                msNormalIndices.udwTopRight = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("NormalBottomLeftIndex", strValue))
            {
                msNormalIndices.udwBottomLeft = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("NormalBottomRightIndex", strValue))
            {
                msNormalIndices.udwBottomRight = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("NormalTopIndex", strValue))
            {
                msNormalIndices.udwTopCentre = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("NormalBottomIndex", strValue))
            {
                msNormalIndices.udwBottomCentre = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("NormalLeftIndex", strValue))
            {
                msNormalIndices.udwLeftCentre = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("NormalRightIndex", strValue))
            {
                msNormalIndices.udwRightCentre = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("NormalCentreIndex", strValue))
            {
                msNormalIndices.udwMiddleCentre = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            //---Highlight Sprite sheet indices
            if(insParams.TryGetValue("HighlightTopLeftIndex", strValue))
            {
                msHighlightIndices.udwTopLeft = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("HighlightTopRightIndex", strValue))
            {
                msHighlightIndices.udwTopRight = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("HighlightBottomLeftIndex", strValue))
            {
                msHighlightIndices.udwBottomLeft = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("HighlightBottomRightIndex", strValue))
            {
                msHighlightIndices.udwBottomRight = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("HighlightTopIndex", strValue))
            {
                msHighlightIndices.udwTopCentre = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("HighlightBottomIndex", strValue))
            {
                msHighlightIndices.udwBottomCentre = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("HighlightLeftIndex", strValue))
            {
                msHighlightIndices.udwLeftCentre = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("HighlightRightIndex", strValue))
            {
                msHighlightIndices.udwRightCentre = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
            if(insParams.TryGetValue("HighlightCentreIndex", strValue))
            {
                msHighlightIndices.udwMiddleCentre = Core::CStringConverter::ParseUnsignedInt(strValue);
            }
			//---Width maintain
			if(insParams.TryGetValue("WidthMaintain", strValue))
            {
				EnableWidthMaintainingAspect(Core::CStringConverter::ParseBool(strValue));
			}
			//---Height maintain
			if(insParams.TryGetValue("HeightMaintain", strValue))
            {
				EnableHeightMaintainingAspect(Core::CStringConverter::ParseBool(strValue));
			}
            
            mInputEvents.GetPressedInsideEvent() += GUIEventDelegate(this, &CStretchableHighlightButton::OnButtonSelect);
            mInputEvents.GetReleasedInsideEvent() += GUIEventDelegate(this, &CStretchableHighlightButton::OnButtonActivated);
            mInputEvents.GetMovedWithinEvent() += GUIEventDelegate(this, &CStretchableHighlightButton::OnButtonDeselectThreshold);
        }
        //-----------------------------------------------------------
        /// Set Normal Sprite Sheet
        //-----------------------------------------------------------
        void CStretchableHighlightButton::SetNormalSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet)
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
        void CStretchableHighlightButton::SetHighlightSpriteSheet(const Rendering::SpriteSheetPtr& inpSpriteSheet)
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
        const Rendering::SpriteSheetPtr& CStretchableHighlightButton::GetNormalSpriteSheet() const
        {
            return NormalSpriteSheet; 
        }
        //-----------------------------------------------------------
        /// Get Highlight Sprite Sheet
        //-----------------------------------------------------------
        const Rendering::SpriteSheetPtr& CStretchableHighlightButton::GetHighlightSpriteSheet() const
        {
            return HighlightSpriteSheet; 
        }
        //-----------------------------------------------------------
        /// Set Base Normal Sprite Sheet Index ID
        //-----------------------------------------------------------
        void CStretchableHighlightButton::SetBaseNormalSpriteSheetIndexID(const std::string& instrID)
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
        void CStretchableHighlightButton::SetBaseHighlightSpriteSheetIndexID(const std::string& instrID)
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
        const std::string& CStretchableHighlightButton::GetBaseNormalSpriteSheetIndexID() const
        {
            return BaseNormalSpriteSheetIndexID;
        }
        //-----------------------------------------------------------
        /// Get Base Highlight Sprite Sheet Index ID
        //-----------------------------------------------------------
        const std::string& CStretchableHighlightButton::GetBaseHighlightSpriteSheetIndexID() const
        {
            return BaseHighlightSpriteSheetIndexID;
        }
        //---------------------------------------------------------
        /// Set Normal Sprite Sheet Indices
        //---------------------------------------------------------
        void CStretchableHighlightButton::SetNormalSpriteSheetIndices(const SpriteSheetIndex& insIndices)
        {
            msNormalIndices = insIndices;
            msCurrentIndices = msNormalIndices;
        }
        //---------------------------------------------------------
        /// Set Normal Sprite Sheet Indices
        //---------------------------------------------------------
        void CStretchableHighlightButton::SetNormalSpriteSheetIndices(const u32* inpIndices)
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
        void CStretchableHighlightButton::SetHighlightSpriteSheetIndices(const SpriteSheetIndex& insIndices)
        {
            msHighlightIndices = insIndices;
        }
        //---------------------------------------------------------
        /// Set Highlight Sprite Sheet Indices
        //---------------------------------------------------------
        void CStretchableHighlightButton::SetHighlightSpriteSheetIndices(const u32* inpIndices)
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
        void CStretchableHighlightButton::SetSelectAudioEffect(const Audio::AudioComponentPtr& inpEffect)
        {
            mpSelectAudioEffect = inpEffect;
        }
        //-----------------------------------------------------------
        /// Set De-Select Audio Effect
        //-----------------------------------------------------------
        void CStretchableHighlightButton::SetDeselectAudioEffect(const Audio::AudioComponentPtr& inpEffect)
        {
            mpDeselectAudioEffect = inpEffect;
        }
        //--------------------------------------------------------
        /// Set Highlight Colour
        //--------------------------------------------------------
        void CStretchableHighlightButton::SetHighlightColour(const Core::CColour & inValue)
        {
            HighlightColour = inValue;
        }
        //--------------------------------------------------------
        /// Get Highlight Colour
        //--------------------------------------------------------
        const Core::CColour & CStretchableHighlightButton::GetHighlightColour() const
        {
            return HighlightColour;
        }
        //-----------------------------------------------------------
        /// Draw
        //-----------------------------------------------------------
        void CStretchableHighlightButton::Draw(Rendering::CCanvasRenderer* inpCanvas)
        {
            //Check if this is on screen
			Core::CVector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Core::ALIGN_TOP_RIGHT);
			Core::CVector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Core::ALIGN_BOTTOM_LEFT);
			
			if(vTopRight.y < 0 || vBottomLeft.y > Core::CScreen::GetOrientedHeight() || vTopRight.x < 0 || vBottomLeft.x > Core::CScreen::GetOrientedWidth())
			{
				//Offscreen
				return;
			}
			
            if(Visible && mCurrentSpriteSheet)
            {			
                Core::CVector2 vPanelSize = GetAbsoluteSize();
                Core::CVector2 vPanelPos = GetAbsoluteScreenSpacePosition();
                Core::CVector2 vTopLeft = GetAbsoluteAnchorPoint(Core::ALIGN_TOP_LEFT);
                Core::CVector2 vPatchPos;
                
                Core::CColour AbsColour = GetAbsoluteColour() * mCurrentColour;
                
                //We need to use a matrix so that we can rotate all the patches with respect
                //to the view
                Core::CMatrix3x3 matTransform;
                Core::CMatrix3x3 matPatchTransform;
                Core::CMatrix3x3 matViewTransform;
                
                matViewTransform.SetTransform(vPanelPos, Core::CVector2(1, 1), GetAbsoluteRotation());
                
                //Get the patch sizes
                Core::CVector2 vTLPatchSize = mCurrentSpriteSheet->GetSizeForFrame(msCurrentIndices.udwTopLeft);
                Core::CVector2 vTRPatchSize = mCurrentSpriteSheet->GetSizeForFrame(msCurrentIndices.udwTopRight);
                Core::CVector2 vBLPatchSize = mCurrentSpriteSheet->GetSizeForFrame(msCurrentIndices.udwBottomLeft);
                Core::CVector2 vBRPatchSize = mCurrentSpriteSheet->GetSizeForFrame(msCurrentIndices.udwBottomRight);
                Core::CVector2 vMLPatchSize = mCurrentSpriteSheet->GetSizeForFrame(msCurrentIndices.udwLeftCentre);
                Core::CVector2 vMRPatchSize = mCurrentSpriteSheet->GetSizeForFrame(msCurrentIndices.udwRightCentre);
                Core::CVector2 vTCPatchSize;
                Core::CVector2 vBCPatchSize;
                Core::CVector2 vMCPatchSize;
                
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
                Core::CMatrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform,
                                   vTLPatchSize, 
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwTopLeft), 
                                   AbsColour, 
                                   Core::ALIGN_TOP_LEFT);
                
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Core::ALIGN_TOP_RIGHT));
                Core::CMatrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vTRPatchSize,  
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwTopRight), 
                                   AbsColour, 
                                   Core::ALIGN_TOP_RIGHT);
                
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Core::ALIGN_BOTTOM_LEFT));
                Core::CMatrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vBLPatchSize, 
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwBottomLeft), 
                                   AbsColour, 
                                   Core::ALIGN_BOTTOM_LEFT);
                
                matPatchTransform.Translate(GetAbsoluteAnchorPoint(Core::ALIGN_BOTTOM_RIGHT));
                Core::CMatrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vBRPatchSize,  
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwBottomRight), 
                                   AbsColour, 
                                   Core::ALIGN_BOTTOM_RIGHT);
                
                //Draw the top and bottom 
                vTCPatchSize.x = vPanelSize.x - (vTLPatchSize.x + vTRPatchSize.x);
                vTCPatchSize.y = vTLPatchSize.y;
                vPatchPos.x = vTopLeft.x + vTLPatchSize.x;
                vPatchPos.y = GetAbsoluteAnchorPoint(Core::ALIGN_TOP_CENTRE).y;
                
                matPatchTransform.Translate(vPatchPos);
                Core::CMatrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vTCPatchSize, 
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwTopCentre), 
                                   AbsColour, 
                                   Core::ALIGN_TOP_LEFT);
                
                
                vBCPatchSize.x = vPanelSize.x - (vBLPatchSize.x + vBRPatchSize.x);
                vBCPatchSize.y = vBLPatchSize.y;
                vPatchPos.x = vTopLeft.x + vBLPatchSize.x;
                vPatchPos.y = GetAbsoluteAnchorPoint(Core::ALIGN_BOTTOM_CENTRE).y;
                
                matPatchTransform.Translate(vPatchPos);
                Core::CMatrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vBCPatchSize, 
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwBottomCentre), 
                                   AbsColour, 
                                   Core::ALIGN_BOTTOM_LEFT);
                
                //Draw the left and right
                vMLPatchSize.y = vPanelSize.y - (vTLPatchSize.y + vBLPatchSize.y);
                vBCPatchSize.x = vTLPatchSize.x;
                vPatchPos.x = GetAbsoluteAnchorPoint(Core::ALIGN_MIDDLE_LEFT).x;
                vPatchPos.y = vTopLeft.y - vTLPatchSize.y;
                
                matPatchTransform.Translate(vPatchPos);
                Core::CMatrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vMLPatchSize, 
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwLeftCentre), 
                                   AbsColour, 
                                   Core::ALIGN_TOP_LEFT);
                
                vMRPatchSize.y = vPanelSize.y - (vTRPatchSize.y + vBRPatchSize.y);
                vMRPatchSize.x = vTRPatchSize.x;
                vPatchPos.x = GetAbsoluteAnchorPoint(Core::ALIGN_MIDDLE_RIGHT).x;
                vPatchPos.y = vTopLeft.y - vTRPatchSize.y;
                
                matPatchTransform.Translate(vPatchPos);
                Core::CMatrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform, 
                                   vMRPatchSize, 
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwRightCentre), 
                                   AbsColour, 
                                   Core::ALIGN_TOP_RIGHT);
                
                //Draw the centre
                vMCPatchSize.x = vPanelSize.x - (vMLPatchSize.x + vMRPatchSize.x);
                vMCPatchSize.y = vPanelSize.y - (vTCPatchSize.y + vBCPatchSize.y);
                vPatchPos.x = vTopLeft.x + vTLPatchSize.x;
                vPatchPos.y = vTopLeft.y - vTLPatchSize.y;
                
                matPatchTransform.Translate(vPatchPos);
                Core::CMatrix3x3::Multiply(&matPatchTransform, &matViewTransform, &matTransform);
                inpCanvas->DrawBox(matTransform,
                                   vMCPatchSize, 
								   mCurrentSpriteSheet->GetTexture(),
                                   mCurrentSpriteSheet->GetUVsForFrame(msCurrentIndices.udwMiddleCentre), 
                                   AbsColour, 
                                   Core::ALIGN_TOP_LEFT);
                
                //Render subviews
                CGUIView::Draw(inpCanvas);
            }
        }
        //--------------------------------------------------------
        /// Set Width Maintaining Aspect
        //--------------------------------------------------------
        void CStretchableHighlightButton::SetWidthMaintainingAspect(f32 infRelWidth, f32 infAbsWidth)
        {
            Core::CVector2 vCurrentSize = GetAbsoluteSize();
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
        void CStretchableHighlightButton::SetHeightMaintainingAspect(f32 infRelHeight, f32 infAbsHeight)
        {
            Core::CVector2 vCurrentSize = GetAbsoluteSize();
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
        void CStretchableHighlightButton::EnableWidthMaintainingAspect(bool inbEnabled)
        {
            WidthMaintain = inbEnabled;  
        }
        //--------------------------------------------------------
        /// Is Width Maintaining Aspect Enabled
        //--------------------------------------------------------
        bool CStretchableHighlightButton::IsWidthMaintainingAspectEnabled() const
        {
            return WidthMaintain;
             
        }
        //--------------------------------------------------------
        /// Enable Height Maintaining Aspect
        //--------------------------------------------------------
        void CStretchableHighlightButton::EnableHeightMaintainingAspect(bool inbEnabled)
        {
            HeightMaintain = inbEnabled;
        }
        //--------------------------------------------------------
        /// Is Height Maintaining Aspect Enabled
        //--------------------------------------------------------
        bool CStretchableHighlightButton::IsHeightMaintainingAspectEnabled() const
        {
            return HeightMaintain;
        }
        //-----------------------------------------------------------
        /// On Button Select
        //-----------------------------------------------------------
        void CStretchableHighlightButton::OnButtonSelect(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
            if(!mbSelected)
			{
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
        void CStretchableHighlightButton::OnButtonActivated(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
            if(mbSelected)
			{
				OnButtonDeselect(inpButton, insTouchInfo);
				mOnButtonActivatedEvent.Invoke(this);
			}
        }
        //-----------------------------------------------------------
        /// On Button De-select
        //-----------------------------------------------------------
        void CStretchableHighlightButton::OnButtonDeselect(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
            if(mbSelected)
			{
				if(mpDeselectAudioEffect && !mpDeselectAudioEffect->IsPlaying())
				{
					mpDeselectAudioEffect->Play();
				}
				
				if (NormalSpriteSheet)
				{
					mCurrentColour = Core::CColour::WHITE;
					mCurrentSpriteSheet = NormalSpriteSheet;
					msCurrentIndices = msNormalIndices;
				}
				
				mbSelected = false;
			}
        }
        //-----------------------------------------------------------
        /// On Button De-select Threshold
        //-----------------------------------------------------------
        void CStretchableHighlightButton::OnButtonDeselectThreshold(CGUIView* inpButton, const Input::TouchInfo & insTouchInfo)
        {
            const f32 kfThreshold = 5.0f;
			if(std::abs(insTouchInfo.vLocation.x - insTouchInfo.vPreviousLocation.x) > kfThreshold || std::abs(insTouchInfo.vLocation.y - insTouchInfo.vPreviousLocation.y) > kfThreshold)
			{
                OnButtonDeselect(inpButton, insTouchInfo);
			}
        }
        //-----------------------------------------------------------
        /// Destructor
        //-----------------------------------------------------------
        CStretchableHighlightButton::~CStretchableHighlightButton()
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
