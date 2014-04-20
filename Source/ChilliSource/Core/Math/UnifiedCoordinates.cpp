//
//  UnifiedCoordinates.cpp
//  moFloTest
//
//  Created by Scott Downie on 22/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Math/UnifiedCoordinates.h>

namespace ChilliSource
{
    namespace Core
    {
        const UnifiedVector2Old UnifiedVector2Old::ZERO(Core::Vector2Old::ZERO, Core::Vector2Old::ZERO);
        
        //------------------------------------------------------
        /// Get Absolute
        ///
        /// @return Absolute vector
        //------------------------------------------------------
        const Core::Vector2Old& UnifiedVector2Old::GetAbsolute() const
        {
            return vAbsolute;
        }
        //------------------------------------------------------
        /// Get Relative
        ///
        /// @return Relative vector
        //------------------------------------------------------
        const Core::Vector2Old& UnifiedVector2Old::GetRelative() const
        {
            return vRelative;
        }
    }
}
