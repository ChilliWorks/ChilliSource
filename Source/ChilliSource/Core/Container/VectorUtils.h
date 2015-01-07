//
//  VectorUtils.h
//  Chilli Source
//  Created by Scott Downie on 01/12/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_CONTAINER_VECTORUTILS_H_
#define _CHILLISOURCE_CORE_CONTAINER_VECTORUTILS_H_

#include <ChilliSource/ChilliSource.h>

#include <algorithm>
#include <vector>

namespace ChilliSource
{
	namespace Core
	{
		//------------------------------------------------------------------------------
		/// A collection of convenience methods for working with vector from the
		/// standard library.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		namespace VectorUtils
		{
			//------------------------------------------------------------------------------
			/// Returns whether or not the given vector contains the given value.
			///
			/// @author Ian Copland
			///
			/// @param The vector.
			/// @param The value to check.
			///
			/// @return Whether or not the given value is contained in the given vector.
			//------------------------------------------------------------------------------
			template <typename TValueType> bool Contains(const std::vector<TValueType>& in_vector, TValueType in_value);
			//------------------------------------------------------------------------------
			/// Removes the first instance of the given value from the vector. If the value 
			/// doesn't exist in the vector the app is considered to be in an irrecovereable 
			/// state and will terminate. This should not be called while iterating over a 
			/// vector as it will invalidate any iterators.
			///
			/// @author Ian Copland
			///
			/// @param The vector.
			/// @param The value to check.
			//------------------------------------------------------------------------------
			template <typename TValueType> void Remove(std::vector<TValueType>& in_vector, TValueType in_value);

			//------------------------------------------------------------------------------
			//------------------------------------------------------------------------------
			template <typename TValueType> bool Contains(const std::vector<TValueType>& in_vector, TValueType in_value)
			{
				return (std::find(in_vector.begin(), in_vector.end(), in_value) != in_vector.end());
			}
			//------------------------------------------------------------------------------
			//------------------------------------------------------------------------------
			template <typename TValueType> void Remove(std::vector<TValueType>& in_vector, TValueType in_value)
			{
				auto it = std::find(in_vector.begin(), in_vector.end(), in_value);
				CS_ASSERT(it != in_vector.end(), "Cannot remove the given value from the vector becuase it doesn't exist.");

				in_vector.erase(it);
			}
		}
	}
}

#endif
