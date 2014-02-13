//
//  Property.hpp
//  MoFlowSkeleton
//
//  Created by Scott Downie on 29/11/2011.
//  Copyright (c) 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Reflection/Property.h>

#include <ChilliSource/Core/HashCRC32.h>

namespace moFlo 
{
	namespace Reflect
	{
		//--------------------------------------------------------
		/// Get Name Hash
		///
		/// @return The hashed name identifier of this property
		//--------------------------------------------------------
		u32 CProperty::GetNameHash() const
		{
			return mudwNameHash;
		}
		//--------------------------------------------------------
		/// Get Name
		///
		/// @return The name identifier of this property
		//--------------------------------------------------------
		const std::string& CProperty::GetName() const
		{
			return mstrName;
		}
		//--------------------------------------------------------
		/// Get Type Hash
		///
		/// @return The hashed type identifier of this property
		//--------------------------------------------------------
		u32 CProperty::GetTypeHash() const
		{
			return mudwTypeHash;
		}
		//--------------------------------------------------------
		/// Get Type
		///
		/// @return The name identifier of this property type
		//--------------------------------------------------------
		const std::string& CProperty::GetType() const
		{
			return mstrType;
		}
		//--------------------------------------------------------
		/// Get Size
		///
		/// @return Sizeof the property type
		//--------------------------------------------------------
		u32 CProperty::GetSize() const
		{
			return mudwSize;
		}
		//--------------------------------------------------------
		/// Get Class
		///
		/// @return The meta class for this instance
		//--------------------------------------------------------
		CMetaClass* CProperty::GetClass() const
		{
			return mpClass;
		}
	}
}

