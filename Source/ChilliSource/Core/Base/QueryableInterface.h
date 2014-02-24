/** 
Get * File: QueryableInterface.h
 * Date: 27 Sept 2010
 * Description: Defines an interface for querying interfaces an object provides as well as
 * macros allowing generating interface ID codes for an interface
 */

/** 
 *
 * Author Stuart McGaw
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_CORE_QUERYABLE_INTERFACE_H_
#define _MOFLO_CORE_QUERYABLE_INTERFACE_H_

#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
	namespace Core
	{
		typedef u32 InterfaceIDType;

		//Queryable
		class IQueryableInterface
		{
		public:
            /// Allow hashing of TypeName strings at runtime without direct reference 
            /// to specific hashing function
            static InterfaceIDType InterfaceIDHash(const std::string& instrName)
            {
                return Core::CHashCRC32::GenerateHashCode(instrName);
            }
            
			IQueryableInterface(){}
			virtual ~IQueryableInterface(){}
			virtual bool IsA(InterfaceIDType inInterfaceID) const = 0;
			
			/// As GetInterface is currently implemented using dynamic_cast it is relatively expensive
			/// Prefer caching the result of this operation rather than doing the operation per-frame
			template <typename T> T* GetInterface(){
				return dynamic_cast<T*> (this);
			}
		};
		
#define DECLARE_NAMED_INTERFACE(x) \
static const ::ChilliSource::Core::InterfaceIDType InterfaceID; \
static const ::std::string TypeName; \
virtual ::ChilliSource::Core::InterfaceIDType GetInterfaceID() const; \
virtual const ::std::string& GetInterfaceTypeName() const;
        
#define DEFINE_NAMED_INTERFACE(x) \
const ::ChilliSource::Core::InterfaceIDType x::InterfaceID = ::ChilliSource::Core::IQueryableInterface::InterfaceIDHash(#x); \
const ::std::string x::TypeName = ::ChilliSource::Core::CStringUtils::StandardiseClassName(#x); \
::ChilliSource::Core::InterfaceIDType x::GetInterfaceID() const \
{ \
    return InterfaceID; \
} \
const ::std::string& x::GetInterfaceTypeName() const \
{ \
    return TypeName; \
}
		
	}
}

#endif
