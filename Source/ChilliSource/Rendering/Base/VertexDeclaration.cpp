/*
*  VertexDeclaration.cpp
*  moFlo
*
*  Created by Scott Downie on 01/10/2010.
*  Copyright 2010 Tag Games. All rights reserved.
*
*/

#include <ChilliSource/Rendering/Base/VertexDeclaration.h>

namespace moFlo
{
	namespace Rendering
	{
		
		CVertexDeclaration::CVertexDeclaration()
		{
			mTotalSize = 0;
			mTotalNumValues = 0;
		}
		
		//-------------------------------------------
		/// Create Declaration
		///
		/// Creates the strides and offsets based
		/// on the vertex layout.
		///
		/// @param Num of semantics
		/// @param Array of semantic names
		//-------------------------------------------
	
		CVertexDeclaration::CVertexDeclaration(u16 nElements, const VertexElement* inpElements)
		{
			mTotalSize = 0;
			mTotalNumValues = 0;

			for(u16 i=0; i<nElements; ++i)
			{
				mTotalSize += inpElements[i].Size();
				mTotalNumValues += inpElements[i].NumDataTypesPerType();
				mElements.push_back(inpElements[i]);
			}
		}
		//-------------------------------------------------------
		/// Get Num Elements
		///
		/// @return the number of elemets in the vertex layout
		//-------------------------------------------------------
		const u16 CVertexDeclaration::GetNumElements()
		{
			return mElements.size();
		}
		//-------------------------------------------------------
		/// Get Element At Index
		///
		/// @return the element 
		//-------------------------------------------------------
		const VertexElement& CVertexDeclaration::GetElementAtIndex(const u16 inIndex)
		{
			return mElements[inIndex];
		}
        //-------------------------------------------------------
		/// Get Elements
		///
		/// @return Element array 
		//-------------------------------------------------------
        const DYNAMIC_ARRAY<VertexElement>& CVertexDeclaration::GetElements() const
        {
            return mElements;
        }
		//-------------------------------------------------------
		/// Get Total Size
		///
		/// @return the size of the vertex layout
		//-------------------------------------------------------
		const u16 CVertexDeclaration::GetTotalSize()
		{
			return mTotalSize;
		}
		//-------------------------------------------------------
		/// Get Total Number of Values
		///
		/// @return the number of individual values in the layout
		//-------------------------------------------------------
		const u32 CVertexDeclaration::GetTotalNumValues()
		{
			return mTotalNumValues;
		}
		//-------------------------------------------------------
		/// Get Element Offset
		///
		/// @return The offset of the element in the layout
		//-------------------------------------------------------
		const u16 CVertexDeclaration::GetElementOffset(const VertexElement &inElement)
		{
			u16 Offset = 0;

			for(DYNAMIC_ARRAY<VertexElement>::iterator it = mElements.begin(); it != mElements.end(); ++it)
			{
				if((*it).eSemantic == inElement.eSemantic)
				{
					break;
				}
				Offset += GetSizeOfElement(*it);
			}

			return Offset;
		}
		//-------------------------------------------------------
		/// Get Size Of Element
		///
		/// @return the size of the element type 
		//-------------------------------------------------------
		const u16 CVertexDeclaration::GetSizeOfElement(VertexElement &inElement)
		{
			return inElement.Size();
		}
		//-------------------------------------------------------
		/// Is Equals operator
		//-------------------------------------------------------
		bool CVertexDeclaration::operator== (const CVertexDeclaration& inOther)
		{
			if (mTotalSize != inOther.mTotalSize)
				return false;
				
			if (mElements.size() != inOther.mElements.size())
				return false;
			
			for (u32 i = 0; i < mElements.size(); i++)
			{
				if (!(mElements[i] == inOther.mElements[i]))
					return false;
			}
			
			return true;
		}
	}
}