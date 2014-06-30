//
//  GridView.h
//  Chilli Source
//  Created by Scott Downie on 14/12/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//


#ifndef _CHILLISOURCE_GUI_GRIDVIEW_H_
#define _CHILLISOURCE_GUI_GRIDVIEW_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Base/GUIView.h>

namespace ChilliSource
{
	namespace GUI
	{
		class GridView : public GUIView
		{
		public:

			DECLARE_META_CLASS(GridView)

			GridView();
			GridView(const Core::ParamDictionary& insParams);
			//-------------------------------------------------------
			/// Set Number of Rows
			///
			/// The amount of content that can be added vertically
			/// If this is zero then the number of rows will expand
			/// to incorporate any new content
			///
			/// @param Num rows
			//-------------------------------------------------------
			void SetNumberOfRows(u32 inudwNumRows);
			//-------------------------------------------------------
			/// Set Number of Columns
			///
			/// The amount of content that can be added horizontally
			/// If this is zero then the number of columns will expand
			/// to incorporate any new content
			///
			/// @param Num columns
			//-------------------------------------------------------
			void SetNumberOfColumns(u32 inudwNumColumns);
			//-------------------------------------------------------
			/// Get Number of Rows
			///
			/// The amount of content that can be added vertically
			/// If this is zero then the number of rows will expand
			/// to incorporate any new content
			///
			/// @return Num rows
			//-------------------------------------------------------
			u32 GetNumberOfRows() const;
			//-------------------------------------------------------
			/// Get Number of Columns
			///
			/// The amount of content that can be added horizontally
			/// If this is zero then the number of columns will expand
			/// to incorporate any new content
			///
			/// @return Num columns
			//-------------------------------------------------------
			u32 GetNumberOfColumns() const;
			//-------------------------------------------------------
			/// Enable Row Fill
			///
			/// Set whether the rows are filled first instead of
			/// the columns
			/// 
			/// @param Boolean
			//-------------------------------------------------------
			void EnableRowFill(bool inbEnable);
			//-------------------------------------------------------
			/// Is Row Fill Enabled
			///
			/// Get whether the rows are filled first instead of
			/// the columns
			/// 
			/// @return Boolean
			//-------------------------------------------------------
			bool IsRowFillEnabled() const;
			//-------------------------------------------------------
			/// Set Absolute Column Spacing
			///
			/// The absolute spacing between horizontal content
			///
			/// @param Dips scalar
			//-------------------------------------------------------
			void SetAbsoluteColumnSpacing(f32 infSpacing);
			//-------------------------------------------------------
			/// Set Absolute Row Spacing
			///
			/// The absolute spacing between vertical content
			///
			/// @param Dips scalar
			//-------------------------------------------------------
			void SetAbsoluteRowSpacing(f32 infSpacing);
			//-------------------------------------------------------
			/// Get Absolute Column Spacing
			///
			/// The absolute spacing between horizontal content
			///
			/// @return Dips scalar
			//-------------------------------------------------------
			f32 GetAbsoluteColumnSpacing() const;
			//-------------------------------------------------------
			/// Get Absolute Row Spacing
			///
			/// The absolute spacing between vertical content
			///
			/// @return Dips scalar
			//-------------------------------------------------------
			f32 GetAbsoluteRowSpacing() const;
            //-------------------------------------------------------
			/// Set Relative Column Spacing
			///
			/// The relative spacing between horizontal content
			///
			/// @param Dips scalar
			//-------------------------------------------------------
			void SetRelativeColumnSpacing(f32 infSpacing);
			//-------------------------------------------------------
			/// Set Relative Row Spacing
			///
			/// The relative spacing between vertical content
			///
			/// @param Dips scalar
			//-------------------------------------------------------
			void SetRelativeRowSpacing(f32 infSpacing);
			//-------------------------------------------------------
			/// Get Relative Column Spacing
			///
			/// The relative spacing between horizontal content
			///
			/// @return Dips scalar
			//-------------------------------------------------------
			f32 GetRelativeColumnSpacing() const;
			//-------------------------------------------------------
			/// Get Relative Row Spacing
			///
			/// The relative spacing between vertical content
			///
			/// @return Dips scalar
			//-------------------------------------------------------
			f32 GetRelativeRowSpacing() const;
			//-------------------------------------------------------
			/// Draw
			///
			/// Draw all our subviews in neat rows and columns. Each
			/// cell is based on the size of the largest content 
			///
			/// @param Canvas renderer pointer
			//-------------------------------------------------------
			void Draw(Rendering::CanvasRenderer* inpCanvas);
			//-------------------------------------------------------
			/// Sets Debug Drawing
			///
			/// Enables/Disables debug drawing
			///
			/// @param New value for this flag. DEBUG_DRAWING must be
			/// set to TRUE
			//-------------------------------------------------------
			void EnableDebugDrawing(bool inbValue);

			//-------------------------------------------------------
			/// Layout Content
			///
			/// Loop through the subviews and position them correctly
			/// in the grid based on the rows and columns
			//-------------------------------------------------------
			virtual void LayoutContent();
            //-----------------------------------------------------
            /// Add Subview
            ///
            /// Add a view to the hierarchy
            ///
            /// @param GUIView shared pointer
            //-----------------------------------------------------
            void AddSubview(const GUIViewSPtr& inpSubview);
            //-----------------------------------------------------
            /// Remove Subview (Internal)
            ///
            /// Remove a view from our hierarchy
            ///
            /// @param GUIView pointer
            //-----------------------------------------------------
            void RemoveSubview(GUIView* inpSubview);
            //-----------------------------------------------------
            /// Remove Subview (Internal)
            ///
            /// Remove a view from our hierarchy
            ///
            /// @param GUIView pointer
            //-----------------------------------------------------			
            void RemoveAllSubviews();

			static bool mbDrawDebug;

		private:
			//-------------------------------------------------------
			/// Layout Content Column Major
			///
			/// Loop through the subviews and position them correctly
			/// in the grid based on the rows and columns
			/// going from left to right
			///
			/// @param Cell size
			//-------------------------------------------------------
			void LayoutContentColumnMajor(const Core::Vector2& invCellSize, u32 udwNumCols, u32 udwNumRows);
			//-------------------------------------------------------
			/// Layout Content Row Major
			///
			/// Loop through the subviews and position them correctly
			/// in the grid based on the rows and columns
			/// going from top to bottom
			///
			/// @param Cell size
			//-------------------------------------------------------
			void LayoutContentRowMajor(const Core::Vector2& invCellSize, u32 udwNumCols, u32 udwNumRows);

		protected:
            GUIView::Subviews mapGridSubviews;

			DECLARE_PROPERTY_A(f32, AbsoluteRowSpacing, SetAbsoluteRowSpacing, GetAbsoluteRowSpacing);
			DECLARE_PROPERTY_A(f32, AbsoluteColumnSpacing, SetAbsoluteColumnSpacing, GetAbsoluteColumnSpacing);
            DECLARE_PROPERTY_A(f32, RelativeRowSpacing, SetRelativeRowSpacing, GetRelativeRowSpacing);
			DECLARE_PROPERTY_A(f32, RelativeColumnSpacing, SetRelativeColumnSpacing, GetRelativeColumnSpacing);

			DECLARE_PROPERTY_A(u32, MaxNumColumns, SetNumberOfColumns, GetNumberOfColumns);
			DECLARE_PROPERTY_A(u32, MaxNumRows, SetNumberOfRows, GetNumberOfRows);

			DECLARE_PROPERTY_A(bool, RowFill, EnableRowFill, IsRowFillEnabled);
		};
	}
}

#endif