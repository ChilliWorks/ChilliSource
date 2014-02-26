//
//  ForwardDeclarations.h
//  moFloTest
//
//  Created by Scott Downie on 21/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_GUI_FORWARDDECLARATIONS_H_
#define _MOFLO_GUI_FORWARDDECLARATIONS_H_

namespace ChilliSource
{
	namespace GUI
	{
        //---Container views
        class CWindow;
        class CGUIView;
        class CScrollView;
        class CGridView;
        class CHorizontalList;
        class CVerticalList;
        
        //---Elements
        class CLabel;
        class CImageView;
        class CEditableLabel;
        class CFormattedLabel;
        class IButton;
        class CProgressBar;
        class CStretchableImage;
        
        //---Resources
		class CDebugStatsView;
        
        typedef std::shared_ptr<CGUIView> GUIViewPtr;
		typedef std::weak_ptr<CGUIView> GUIViewWeakPtr;
        
        typedef std::shared_ptr<CGridView> GridViewPtr;
		typedef std::weak_ptr<CGridView> GridViewWeakPtr;
        
        typedef std::shared_ptr<CHorizontalList> HorizontalListPtr;
		typedef std::weak_ptr<CHorizontalList> HorizontalListWeakPtr;
        
        typedef std::shared_ptr<CVerticalList> VerticalListPtr;
		typedef std::weak_ptr<CVerticalList> VerticalListWeakPtr;
        
        typedef std::shared_ptr<CScrollView> ScrollViewPtr;
		typedef std::weak_ptr<CScrollView> ScrollViewWeakPtr;
        
        typedef std::shared_ptr<CImageView> ImageViewPtr;
		typedef std::weak_ptr<CImageView> ImageViewWeakPtr;

		typedef std::shared_ptr<CEditableLabel> EditableLabelPtr;
		typedef std::weak_ptr<CEditableLabel> EditableLabelWeakPtr;
		
		typedef std::shared_ptr<IButton> ButtonPtr;
		typedef std::weak_ptr<IButton> ButtonWeakPtr;
		
        typedef std::shared_ptr<CStretchableImage> StretchableImagePtr;
		typedef std::weak_ptr<CStretchableImage> StretchableImageWeakPtr;
        
        typedef std::shared_ptr<CLabel> LabelPtr;
		typedef std::weak_ptr<CLabel> LabelWeakPtr;
        
        typedef std::shared_ptr<CFormattedLabel> FormattedLabelPtr;
		typedef std::weak_ptr<CFormattedLabel> FormattedLabelWeakPtr;
        
        typedef std::shared_ptr<CProgressBar> ProgressBarPtr;
        typedef std::weak_ptr<CProgressBar> ProgressBarWeakPtr;
		
		typedef std::shared_ptr<CDebugStatsView> DebugStatsViewPtr;
		typedef std::weak_ptr<CDebugStatsView> DebugStatsViewWeakPtr;
        
        //---Default widgets
        class CHighlightButton;
        typedef std::shared_ptr<CHighlightButton> HighlightButtonPtr;
        typedef std::weak_ptr<CHighlightButton> HighlightButtonWeakPtr;
        
        class CHorizontalClippingProgressBar;
        typedef std::shared_ptr<CHorizontalClippingProgressBar> HorizontalClippingProgressBarPtr;
		typedef std::weak_ptr<CHorizontalClippingProgressBar> HorizontalClippingProgressBarWeakPtr;
        
        class CHorizontalEndcappedProgressBar;
        typedef std::shared_ptr<CHorizontalEndcappedProgressBar> HorizontalEndcappedProgressBarPtr;
		typedef std::weak_ptr<CHorizontalEndcappedProgressBar> HorizontalEndcappedProgressBarWeakPtr;

        class CVerticalEndcappedProgressBar;
        typedef std::shared_ptr<CVerticalEndcappedProgressBar> VerticalEndcappedProgressBarPtr;
		typedef std::weak_ptr<CVerticalEndcappedProgressBar> VerticalEndcappedProgressBarWeakPtr;
        
        class CHorizontalProgressBar;
        typedef std::shared_ptr<CHorizontalProgressBar> HorizontalProgressBarPtr;
		typedef std::weak_ptr<CHorizontalProgressBar> HorizontalProgressBarWeakPtr;
        
        class CHorizontalSliderBar;
        typedef std::shared_ptr<CHorizontalSliderBar> HorizontalSliderBarPtr;
		typedef std::weak_ptr<CHorizontalSliderBar> HorizontalSliderBarWeakPtr;
        
        class CHorizontalStretchableImage;
        typedef std::shared_ptr<CHorizontalStretchableImage> HorizontalStretchableImagePtr;
        typedef std::weak_ptr<CHorizontalStretchableImage> HorizontalStretchableImageWeakPtr;
        
        class CStretchableHighlightButton;
        typedef std::shared_ptr<CStretchableHighlightButton> StretchableHighlightButtonPtr;
		typedef std::weak_ptr<CStretchableHighlightButton> StretchableHighlightButtonWeakPtr;
        
        class CToggleButton;
        typedef std::shared_ptr<CToggleButton> ToggleButtonPtr;
        typedef std::weak_ptr<CToggleButton> ToggleButtonWeakPtr;
        
        class CVerticalSliderBar;
        typedef std::shared_ptr<CVerticalSliderBar> VerticalSliderBarPtr;
        typedef std::weak_ptr<CVerticalSliderBar> VerticalSliderBarWeakPtr;
        
        class CVerticalStretchableImage;
        typedef std::shared_ptr<CVerticalStretchableImage> VerticalStretchableImagePtr;
        typedef std::weak_ptr<CVerticalStretchableImage> VerticalStretchableImageWeakPtr;
    }
}

#endif
