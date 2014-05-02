//
//  GridView.cpp
//  moFlo
//
//  Created by Scott Downie on 27/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Container/GridView.h>

#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Base/Screen.h>

#if CS_ENABLE_DEBUGDRAWING
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#endif

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(GridView)

		DEFINE_PROPERTY(AbsoluteRowSpacing);
		DEFINE_PROPERTY(AbsoluteColumnSpacing);
        DEFINE_PROPERTY(RelativeRowSpacing);
		DEFINE_PROPERTY(RelativeColumnSpacing);
		DEFINE_PROPERTY(MaxNumColumns);
		DEFINE_PROPERTY(MaxNumRows);
		DEFINE_PROPERTY(RowFill);

        bool GridView::mbDrawDebug = false;
        
        //------------------------------------------------------
        /// Constructor
        ///
        /// Default
        //------------------------------------------------------
        GridView::GridView()
            : AbsoluteRowSpacing(0.0f), AbsoluteColumnSpacing(0.0f), RelativeRowSpacing(0.0f), RelativeColumnSpacing(0.0f), RowFill(false),
            MaxNumColumns(0), MaxNumRows(0)
        {
        }
        //------------------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //------------------------------------------------------
        GridView::GridView(const Core::ParamDictionary& insParams)
            : GUIView(insParams), AbsoluteRowSpacing(0.0f), AbsoluteColumnSpacing(0.0f), RelativeRowSpacing(0.0f), RelativeColumnSpacing(0.0f),
            RowFill(false), MaxNumColumns(0), MaxNumRows(0)
        {
            std::string strValue;
            
            //---Num columns
            if(insParams.TryGetValue("MaxNumColumns", strValue))
            {
                MaxNumColumns = Core::ParseU32(strValue);
            }
            //---Num rows
            if(insParams.TryGetValue("MaxNumRows", strValue))
            {
                MaxNumRows = Core::ParseU32(strValue);
            }
            //---Fill direction
            if(insParams.TryGetValue("RowFill", strValue))
            {
                RowFill = Core::ParseBool(strValue);
            }
            //---Row spacing
            if(insParams.TryGetValue("AbsoluteRowSpacing", strValue))
            {
                SetAbsoluteRowSpacing(Core::ParseF32(strValue));
            }
            //---Column spacing
            if(insParams.TryGetValue("AbsoluteColumnSpacing", strValue))
            {
                SetAbsoluteColumnSpacing(Core::ParseF32(strValue));
            }
            //---Row spacing
            if(insParams.TryGetValue("RelativeRowSpacing", strValue))
            {
                SetRelativeRowSpacing(Core::ParseF32(strValue));
            }
            //---Column spacing
            if(insParams.TryGetValue("RelativeColumnSpacing", strValue))
            {
                SetRelativeColumnSpacing(Core::ParseF32(strValue));
            }
        }
        //-------------------------------------------------------
        /// Set Number of Rows
        ///
        /// The amount of content that can be added vertically
        /// If this is zero then the number of rows will expand
        /// to incorporate any new content
        ///
        /// If the number of rows and the number of columns are
        /// zero then the view will exapnd uniformly in a square
        ///
        /// @param Num rows
        //-------------------------------------------------------
        void GridView::SetNumberOfRows(u32 inudwNumRows)
        {
            MaxNumRows = inudwNumRows; 
        }
        //-------------------------------------------------------
        /// Set Number of Columns
        ///
        /// The amount of content that can be added horizontally
        /// If this is zero then the number of columns will expand
        /// to incorporate any new content
        ///
        /// If the number of rows and the number of columns are
        /// zero then the view will exapnd uniformly in a square
        ///
        /// @param Num columns
        //-------------------------------------------------------
        void GridView::SetNumberOfColumns(u32 inudwNumColumns)
        {
            MaxNumColumns = inudwNumColumns;
        }
		//-------------------------------------------------------
		/// Get Number of Rows
		///
		/// The amount of content that can be added vertically
		/// If this is zero then the number of rows will expand
		/// to incorporate any new content
		///
		/// @return Num rows
		//-------------------------------------------------------
		u32 GridView::GetNumberOfRows() const
		{
			return MaxNumRows;
		}
		//-------------------------------------------------------
		/// Get Number of Columns
		///
		/// The amount of content that can be added horizontally
		/// If this is zero then the number of columns will expand
		/// to incorporate any new content
		///
		/// @return Num columns
		//-------------------------------------------------------
		u32 GridView::GetNumberOfColumns() const
		{
			return MaxNumColumns;
		}
        //-------------------------------------------------------
        /// Set Absolute Column Spacing
        ///
        /// The spacing between horizontal content
        ///
        /// @param Absolute scalar
        //-------------------------------------------------------
        void GridView::SetAbsoluteColumnSpacing(f32 infSpacing)
        {
            AbsoluteColumnSpacing = infSpacing;
        }
        //-------------------------------------------------------
        /// Set Absolute Row Spacing
        ///
        /// The spacing between vertical content
        ///
        /// @param Absolute scalar
        //-------------------------------------------------------
        void GridView::SetAbsoluteRowSpacing(f32 infSpacing)
        {
            AbsoluteRowSpacing = infSpacing;
        }
		//-------------------------------------------------------
		/// Get Absolute Column Spacing
		///
		/// The spacing between horizontal content
		///
		/// @return Dips scalar
		//-------------------------------------------------------
		f32 GridView::GetAbsoluteColumnSpacing() const
		{
			return AbsoluteColumnSpacing;
		}
		//-------------------------------------------------------
		/// Get Absolute Row Spacing
		///
		/// The spacing between vertical content
		///
		/// @return Dips scalar
		//-------------------------------------------------------
		f32 GridView::GetAbsoluteRowSpacing() const
		{
			return AbsoluteRowSpacing;
		}
        //-------------------------------------------------------
        /// Set Relative Column Spacing
        ///
        /// The relative spacing between horizontal content
        ///
        /// @param Dips scalar
        //-------------------------------------------------------
        void GridView::SetRelativeColumnSpacing(f32 infSpacing)
        {
            RelativeColumnSpacing = infSpacing;
        }
        //-------------------------------------------------------
        /// Set Relative Row Spacing
        ///
        /// The relative spacing between vertical content
        ///
        /// @param Dips scalar
        //-------------------------------------------------------
        void GridView::SetRelativeRowSpacing(f32 infSpacing)
        {
            RelativeRowSpacing = infSpacing;
        }
        //-------------------------------------------------------
        /// Get Relative Column Spacing
        ///
        /// The relative spacing between horizontal content
        ///
        /// @return Dips scalar
        //-------------------------------------------------------
        f32 GridView::GetRelativeColumnSpacing() const
        {
            return RelativeColumnSpacing;
        }
        //-------------------------------------------------------
        /// Get Relative Row Spacing
        ///
        /// The relative spacing between vertical content
        ///
        /// @return Dips scalar
        //-------------------------------------------------------
        f32 GridView::GetRelativeRowSpacing() const
        {
            return RelativeRowSpacing;
        }
		//-------------------------------------------------------
		/// Enable Row Fill
		///
		/// Set whether the rows are filled first instead of
		/// the columns
		/// 
		/// @param Boolean
		//-------------------------------------------------------
		void GridView::EnableRowFill(bool inbEnable)
		{
			RowFill = inbEnable;
		}
		//-------------------------------------------------------
		/// Is Row Fill Enabled
		///
		/// Get whether the rows are filled first instead of
		/// the columns
		/// 
		/// @return Boolean
		//-------------------------------------------------------
		bool GridView::IsRowFillEnabled() const
		{
			return RowFill;
		}
        //-------------------------------------------------------
        /// Layout Content Column Major
        ///
        /// Loop through the subviews and position them correctly
        /// in the grid based on the rows and columns
        /// going from left to right
        ///
        /// @param Cell size
        //-------------------------------------------------------
        void GridView::LayoutContentColumnMajor(const Core::Vector2& invCellSize, u32 udwNumCols, u32 udwNumRows)
        {
            const f32 fColumnSpacing = (GetAbsoluteSize().x * RelativeColumnSpacing) + AbsoluteColumnSpacing;
            const f32 fRowSpacing = (GetAbsoluteSize().y * RelativeRowSpacing) + AbsoluteRowSpacing;
            
            //Layout based on the cell size
            Core::Vector2 vCellPosition;
            
            u32 udwRow = 0;
            u32 udwCol = 0;
            
            for(Subviews::iterator pView = mapGridSubviews.begin(); pView != mapGridSubviews.end(); ++pView)
            {
                (*pView)->EnableAlignmentToParent(true);
                (*pView)->SetAlignmentToParent(Rendering::AlignmentAnchor::k_topLeft);
                (*pView)->SetLocalAlignment(Rendering::AlignmentAnchor::k_topLeft);
                (*pView)->SetOffsetFromParentAlignment(Core::UnifiedVector2(Core::Vector2(0.0f, 0.0f), vCellPosition));
                vCellPosition.x += (invCellSize.x + fColumnSpacing);
                
                //If we have a fixed column and row width we must wrap the content
                if(++udwCol >= udwNumCols)
                {
                    //Wrap to next row
                    vCellPosition.y -= (invCellSize.y + fRowSpacing);
                    udwCol = 0;
                    vCellPosition.x = 0.0f;
                    
                    if(++udwRow >= udwNumRows)
                    {
                        //Exceeded limit
                        break;
                    }
                }
            }
        }
        //-------------------------------------------------------
        /// Layout Content Row Major
        ///
        /// Loop through the subviews and position them correctly
        /// in the grid based on the rows and columns
        /// going from top to bottom
        ///
        /// @param Cell size
        //-------------------------------------------------------
        void GridView::LayoutContentRowMajor(const Core::Vector2& invCellSize, u32 udwNumCols, u32 udwNumRows)
        {
            const f32 fColumnSpacing = (GetAbsoluteSize().x * RelativeColumnSpacing) + AbsoluteColumnSpacing;
            const f32 fRowSpacing = (GetAbsoluteSize().y * RelativeRowSpacing) + AbsoluteRowSpacing;
            
            //Layout based on the cell size
            Core::Vector2 vCellPosition;
            
            u32 udwRow = 0;
            u32 udwCol = 0;
            
            for(Subviews::iterator pView = mapGridSubviews.begin(); pView != mapGridSubviews.end(); ++pView)
            {
                (*pView)->EnableAlignmentToParent(true);
                (*pView)->SetAlignmentToParent(Rendering::AlignmentAnchor::k_topLeft);
				(*pView)->SetLocalAlignment(Rendering::AlignmentAnchor::k_topLeft);
                (*pView)->SetOffsetFromParentAlignment(Core::UnifiedVector2(Core::Vector2(0.0f, 0.0f), vCellPosition));
                vCellPosition.y -= (invCellSize.y + fRowSpacing);
                
                //If we have a fixed column and row width we must wrap the content
                if(++udwRow >= udwNumRows)
                {
                    //Wrap to next column
                    vCellPosition.x += (invCellSize.x + fColumnSpacing);
                    udwRow = 0;
                    vCellPosition.y = 0.0f;
                    
                    if(++udwCol >= udwNumCols)
                    {
                        //Exceeded limit
                        break;
                    }
                }
            }
        }
        //-------------------------------------------------------
        /// Layout Content
        ///
        /// Loop through the subviews and position them correctly
        /// in the grid based on the rows and columns
        //-------------------------------------------------------
        void GridView::LayoutContent()
        {
            //All the subviews within this grid view will helpfully
            //be re-arranged to fit the grid
            //If num columns is zero we expand horizontally
            //If num rows is zero we expand vertically
            if(MaxNumRows == 0 && MaxNumColumns == 0)
            {
                //Anything else would result in undefined behaviour
                MaxNumColumns = 1;
            }
               
            //Find the largest object. This will be our 'cell' size
            Core::Vector2 vCellSize;
            for(Subviews::iterator pView = mapGridSubviews.begin(); pView != mapGridSubviews.end(); ++pView)
            {
                Core::Vector2 vViewSize = (*pView)->GetAbsoluteSize();
                if(vViewSize.x > vCellSize.x)
                {
                    vCellSize.x = vViewSize.x;
                }
                if(vViewSize.y > vCellSize.y)
                {
                    vCellSize.y = vViewSize.y;
                }
            }
            
            u32 udwNumContentToDraw = mapGridSubviews.size();
            u32 udwNumCols = 0;
            u32 udwNumRows = 0;
            
            MaxNumRows ? udwNumRows = Core::MathUtils::Min(MaxNumRows, udwNumContentToDraw) : udwNumRows = (u32)std::ceil((f32)udwNumContentToDraw/(f32)MaxNumColumns);
            MaxNumColumns ? udwNumCols = Core::MathUtils::Min(MaxNumColumns, udwNumContentToDraw) : udwNumCols = (u32)std::ceil((f32)udwNumContentToDraw/(f32)MaxNumRows);
            
            if(!RowFill)
            {
                LayoutContentColumnMajor(vCellSize, udwNumCols, udwNumRows);
            }
            else
            {
                LayoutContentRowMajor(vCellSize, udwNumCols, udwNumRows);
            }
            
            const f32 fColumnSpacing = (GetAbsoluteSize().x * RelativeColumnSpacing) + AbsoluteColumnSpacing;
            const f32 fRowSpacing = (GetAbsoluteSize().y * RelativeRowSpacing) + AbsoluteRowSpacing;
            
            //Calculate the size of the grid view based on the content
            SetSize(0.0f, 0.0f,
                    (vCellSize.x * udwNumCols) + (fColumnSpacing * (udwNumCols ? (udwNumCols-1) : 0)),
                    (vCellSize.y * udwNumRows) + (fRowSpacing * (udwNumRows ? (udwNumRows-1) : 0)));
        }
        
        //-----------------------------------------------------
        /// Add Subview
        ///
        /// Add a view to the hierarchy
        ///
        /// @param GUIView shared pointer
        //-----------------------------------------------------
        void GridView::AddSubview(const GUIViewSPtr& inpSubview)
        {
            GUIView::AddSubview(inpSubview);
            mapGridSubviews.push_back(inpSubview);
        }
        //-----------------------------------------------------
        /// Remove Subview (Internal)
        ///
        /// Remove a view from our hierarchy
        ///
        /// @param GUIView pointer
        //-----------------------------------------------------
        void GridView::RemoveSubview(GUIView* inpSubview)
        {
            GUIView::RemoveSubview(inpSubview);
            for(GUIView::Subviews::iterator it = mapGridSubviews.begin(); it != mapGridSubviews.end(); ++it)
            {
                if((*it).get() == inpSubview)
                {
                    mapGridSubviews.erase(it);
                    return;
                }
            }

        }
		//-----------------------------------------------------
		/// Remove Subview (Internal)
		///
		/// Remove a view from our hierarchy
		///
		/// @param GUIView pointer
		//-----------------------------------------------------			
		void GridView::RemoveAllSubviews()
		{
			GUIView::RemoveAllSubviews();
            mapGridSubviews.clear();
		}
        
        //-------------------------------------------------------
        /// Draw
        ///
        /// Draw all our subviews in neat rows and columns. Each
        /// cell is based on the size of the largest content 
        ///
        /// @param Canvas renderer pointer
        //-------------------------------------------------------
        void GridView::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            //This is a container view and has no renderables
            //We will need to render our subviews though
            if(Visible)
            {
				//Check if this is on screen
				Core::Vector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topRight);
				Core::Vector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
				
				if(vTopRight.y < 0 || vBottomLeft.y > GetScreen()->GetResolution().y || vTopRight.x < 0 || vBottomLeft.x > GetScreen()->GetResolution().x)
				{
					//Offscreen
					return;
				}
				
                //Arrange everything!
                LayoutContent();
				
#if CS_ENABLE_DEBUGDRAWING
				if(GridView::mbDrawDebug)
				{
					Rendering::TextureManager* pMgr = (Rendering::TextureManager*)(Core::ResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(Rendering::Texture::InterfaceID));
					inpCanvas->DrawBox(GetTransform(), GetAbsoluteSize(), pMgr->GetDefaultTexture(), Core::Rectangle(Core::Vector2::k_zero, Core::Vector2::k_zero), Core::Colour(1.0f,0.0f,0.0f,0.5f));
				}
#endif
				
                GUIView::Draw(inpCanvas);
            }
        }
		//-------------------------------------------------------
		/// Sets Debug Drawing
		///
		/// Enables/Disables debug drawing
		///
		/// @param New value for this flag. DEBUG_DRAWING must be
		/// set to TRUE
		//-------------------------------------------------------
		void GridView::EnableDebugDrawing(bool inbValue)
		{
#if CS_ENABLE_DEBUGDRAWING
			GridView::mbDrawDebug = inbValue;
#else
			GridView::mbDrawDebug = false;
#endif
		}
    }
}
