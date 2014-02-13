/** 
 * File: Curves.cpp
 * Date: 17/11/2010
 * Description:
 */

/** 
 *
 * Author: Stuart McGaw 
 * Version: 1.0 - MoFlow
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <moFlo/Core/Math/Curves.h>

namespace moFlo
{
	namespace Core
	{
		template<typename T> const u16 CCatmullRomSpline<T>::wLengthSamplesPerSegment = 259;
		template<typename T> const float CCatmullRomSpline<T>::mfSmallTDelta = 0.005f;
	}
}

