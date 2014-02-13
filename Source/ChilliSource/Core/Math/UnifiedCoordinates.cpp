//
//  UnifiedCoordinates.cpp
//  moFloTest
//
//  Created by Scott Downie on 22/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Math/UnifiedCoordinates.h>

namespace moFlo
{
    const UnifiedVector2 UnifiedVector2::ZERO(Core::CVector2::ZERO, Core::CVector2::ZERO);
    
	//------------------------------------------------------
	/// Get Absolute
	///
	/// @return Absolute vector
	//------------------------------------------------------
	const Core::CVector2& UnifiedVector2::GetAbsolute() const
	{
		return vAbsolute;
	}
	//------------------------------------------------------
	/// Get Relative
	///
	/// @return Relative vector
	//------------------------------------------------------
	const Core::CVector2& UnifiedVector2::GetRelative() const
	{
		return vRelative;
	}
}
