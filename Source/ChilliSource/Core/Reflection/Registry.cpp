//
//  Registry.hpp
//  MoFlowSkeleton
//
//  Created by Robert Henning on 30/11/2011.
//  Copyright (c) 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Reflection/Registry.h>
#include <ChilliSource/Core/Reflection/MetaClass.h>

#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

namespace ChilliSource
{
	namespace Reflect
	{
		DYNAMIC_ARRAY<CMetaClass*> CRegistry::MetaClasses;

		//------------------------------------------------------------
		/// Add Class
		///
		/// @param Meta-class
		//------------------------------------------------------------
		void CRegistry::AddClass(CMetaClass* inpClass)
		{
			MetaClasses.push_back(inpClass);
		}
		//------------------------------------------------------------
		/// Remove Class
		///
		/// @param Meta-class
		//------------------------------------------------------------
		void CRegistry::RemoveClass(CMetaClass* inpClass)
		{
			DYNAMIC_ARRAY<CMetaClass*>::iterator it = std::find(MetaClasses.begin(), MetaClasses.end(), inpClass);
			if(it != MetaClasses.end())
			{
				MetaClasses.erase(it);
			}
		}
		//------------------------------------------------------------
		/// Get Class
		///
		/// Get the meta-class with the given name ID
		///
		/// @param Class name
		/// @return Meta class
		//------------------------------------------------------------
		CMetaClass* CRegistry::GetClass(const std::string& instrName)
		{
			u32 udwNameHash = CHashCRC32::GenerateHashCode(instrName);
			for(DYNAMIC_ARRAY<CMetaClass*>::iterator it = MetaClasses.begin(); it != MetaClasses.end(); ++it)
			{
				if((*it)->GetNameHash() == udwNameHash)
				{
					return (*it);
				}
			}

			return NULL;
		}
		//------------------------------------------------------------
		/// Get Classes
		///
		/// Get all the meta-classes
		///
		/// @return Array of meta classes
		//------------------------------------------------------------
		const DYNAMIC_ARRAY<CMetaClass*>& CRegistry::GetClasses()
		{
			return MetaClasses;
		}
		//-------------------------------------------------------------
		/// Destructor
		//-------------------------------------------------------------
		CRegistry::~CRegistry()
		{
			for(DYNAMIC_ARRAY<CMetaClass*>::iterator it = MetaClasses.begin(); it != MetaClasses.end(); ++it)
			{
				delete (*it);
			}
		}
	}
}

