//
//  Pointer.h
//  Chilli Source
//  Created by Scott Downie on 08/07/2014.
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

#ifndef _CHILLISOURCE_INPUT_POINTER_POINTER_H_
#define _CHILLISOURCE_INPUT_POINTER_POINTER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>

#include <set>

namespace ChilliSource
{
    namespace Input
    {
        
        //----------------------------------------------------
        /// A container for information on a single pointer.
        /// A pointer contains positional information as well
        /// as an index and unique Id. The Id is unique to that
        /// specific pointer and can be used to query for it.
        /// The index describes the active index of the pointer
        /// at the point it was created, so if there were already
        /// two active pointers existing at creation, it will be
        /// have an index of 2.
        ///
        /// @author Ian Copland
        //----------------------------------------------------
        class Pointer final
        {
        public:

            using Id = u64;
            //----------------------------------------------------
            /// An enum describing the different types of pointer
            /// press.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            enum class InputType
            {
                k_none,
                k_touch,
                k_leftMouseButton,
                k_middleMouseButton,
                k_rightMouseButton
            };
            //----------------------------------------------------
            /// Returns the default press type for the current platform.
            /// This should be used where possible rather than
            /// direct use of the press type to make cross platform
            /// development easier.
            ///
            /// @author Ian Copland
            ///
            /// @return the default press type.
            //----------------------------------------------------
            static InputType GetDefaultInputType();
            //----------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Unique Id
            /// @param Index in list
            /// @param Position
            //----------------------------------------------------
            Pointer(Id in_uniqueId, u32 in_index, const Core::Vector2& in_initialPosition);
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Current pointer location
            //----------------------------------------------------
            Core::Vector2 GetPosition() const;
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Previous pointer location
            //----------------------------------------------------
            Core::Vector2 GetPreviousPosition() const;
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Input type
            ///
            /// @return Whether the input type is down
            //----------------------------------------------------
            bool IsInputDown(InputType in_type) const;
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @param Input type
            ///
            /// @return Whether the input type is up
            //----------------------------------------------------
            bool IsInputUp(InputType in_type) const;
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Id
            //----------------------------------------------------
            Id GetId() const;
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Index of pointer in pointer list
            //----------------------------------------------------
            u32 GetIndex() const;
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Container of inout types that are currently
            /// down
            //----------------------------------------------------
            const std::set<InputType>& GetActiveInputs() const;
            
        private:
            
            friend class PointerSystem;
            
            Core::Vector2 m_position;
            Core::Vector2 m_previousPosition;
            Id m_uniqueId;
            u32 m_index;
            std::set<InputType> m_activeInput;
        };
    }
}

#endif
