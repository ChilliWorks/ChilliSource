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
	namespace Rendering
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
        
        typedef SHARED_PTR<CGUIView> GUIViewPtr;
		typedef WEAK_PTR<CGUIView> GUIViewWeakPtr;
        
        typedef SHARED_PTR<CGridView> GridViewPtr;
		typedef WEAK_PTR<CGridView> GridViewWeakPtr;
        
        typedef SHARED_PTR<CHorizontalList> HorizontalListPtr;
		typedef WEAK_PTR<CHorizontalList> HorizontalListWeakPtr;
        
        typedef SHARED_PTR<CVerticalList> VerticalListPtr;
		typedef WEAK_PTR<CVerticalList> VerticalListWeakPtr;
        
        typedef SHARED_PTR<CScrollView> ScrollViewPtr;
		typedef WEAK_PTR<CScrollView> ScrollViewWeakPtr;
        
        typedef SHARED_PTR<CImageView> ImageViewPtr;
		typedef WEAK_PTR<CImageView> ImageViewWeakPtr;

		typedef SHARED_PTR<CEditableLabel> EditableLabelPtr;
		typedef WEAK_PTR<CEditableLabel> EditableLabelWeakPtr;
		
		typedef SHARED_PTR<IButton> ButtonPtr;
		typedef WEAK_PTR<IButton> ButtonWeakPtr;
		
        typedef SHARED_PTR<CStretchableImage> StretchableImagePtr;
		typedef WEAK_PTR<CStretchableImage> StretchableImageWeakPtr;
        
        typedef SHARED_PTR<CLabel> LabelPtr;
		typedef WEAK_PTR<CLabel> LabelWeakPtr;
        
        typedef SHARED_PTR<CFormattedLabel> FormattedLabelPtr;
		typedef WEAK_PTR<CFormattedLabel> FormattedLabelWeakPtr;
        
        typedef SHARED_PTR<CProgressBar> ProgressBarPtr;
        typedef WEAK_PTR<CProgressBar> ProgressBarWeakPtr;
		
		typedef SHARED_PTR<CDebugStatsView> DebugStatsViewPtr;
		typedef WEAK_PTR<CDebugStatsView> DebugStatsViewWeakPtr;
        
        //---Default widgets
        class CHighlightButton;
        typedef SHARED_PTR<CHighlightButton> HighlightButtonPtr;
        typedef WEAK_PTR<CHighlightButton> HighlightButtonWeakPtr;
        
        class CHorizontalClippingProgressBar;
        typedef SHARED_PTR<CHorizontalClippingProgressBar> HorizontalClippingProgressBarPtr;
		typedef WEAK_PTR<CHorizontalClippingProgressBar> HorizontalClippingProgressBarWeakPtr;
        
        class CHorizontalEndcappedProgressBar;
        typedef SHARED_PTR<CHorizontalEndcappedProgressBar> HorizontalEndcappedProgressBarPtr;
		typedef WEAK_PTR<CHorizontalEndcappedProgressBar> HorizontalEndcappedProgressBarWeakPtr;

        class CVerticalEndcappedProgressBar;
        typedef SHARED_PTR<CVerticalEndcappedProgressBar> VerticalEndcappedProgressBarPtr;
		typedef WEAK_PTR<CVerticalEndcappedProgressBar> VerticalEndcappedProgressBarWeakPtr;
        
        class CHorizontalProgressBar;
        typedef SHARED_PTR<CHorizontalProgressBar> HorizontalProgressBarPtr;
		typedef WEAK_PTR<CHorizontalProgressBar> HorizontalProgressBarWeakPtr;
        
        class CHorizontalSliderBar;
        typedef SHARED_PTR<CHorizontalSliderBar> HorizontalSliderBarPtr;
		typedef WEAK_PTR<CHorizontalSliderBar> HorizontalSliderBarWeakPtr;
        
        class CHorizontalStretchableImage;
        typedef SHARED_PTR<CHorizontalStretchableImage> HorizontalStretchableImagePtr;
        typedef WEAK_PTR<CHorizontalStretchableImage> HorizontalStretchableImageWeakPtr;
        
        class CStretchableHighlightButton;
        typedef SHARED_PTR<CStretchableHighlightButton> StretchableHighlightButtonPtr;
		typedef WEAK_PTR<CStretchableHighlightButton> StretchableHighlightButtonWeakPtr;
        
        class CToggleButton;
        typedef SHARED_PTR<CToggleButton> ToggleButtonPtr;
        typedef WEAK_PTR<CToggleButton> ToggleButtonWeakPtr;
        
        class CVerticalSliderBar;
        typedef SHARED_PTR<CVerticalSliderBar> VerticalSliderBarPtr;
        typedef WEAK_PTR<CVerticalSliderBar> VerticalSliderBarWeakPtr;
        
        class CVerticalStretchableImage;
        typedef SHARED_PTR<CVerticalStretchableImage> VerticalStretchableImagePtr;
        typedef WEAK_PTR<CVerticalStretchableImage> VerticalStretchableImageWeakPtr;
    }
}

#endif
