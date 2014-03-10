//
//  QueryableInterface.h
//  ChilliSource
//
//  Created by Stuart McGaw on 27/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_QUERYABLEINTERFACE_H_
#define _CHILLISOURCE_CORE_QUERYABLEINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/String/StringUtils.h>

#define CS_DECLARE_NAMEDTYPE(x) \
static const ::ChilliSource::Core::InterfaceIDType InterfaceID; \
static const ::std::string TypeName; \
virtual ::ChilliSource::Core::InterfaceIDType GetInterfaceID() const; \
virtual const ::std::string& GetInterfaceTypeName() const;

#define CS_DEFINE_NAMEDTYPE(x) \
const ::ChilliSource::Core::InterfaceIDType x::InterfaceID = ::ChilliSource::Core::QueryableInterface::InterfaceIDHash(#x); \
const ::std::string x::TypeName = ::ChilliSource::Core::StringUtils::StandardiseClassName(#x); \
::ChilliSource::Core::InterfaceIDType x::GetInterfaceID() const \
{ \
return InterfaceID; \
} \
const ::std::string& x::GetInterfaceTypeName() const \
{ \
return TypeName; \
}

namespace ChilliSource
{
	namespace Core
	{
		typedef u32 InterfaceIDType;

		//-------------------------------------------------------
        /// Classes that inherit from queryable interface
        /// can have their type obtained at runtime.
        ///
        /// @author S McGaw
        //-------------------------------------------------------
		class QueryableInterface
		{
		public:
            /// Allow hashing of TypeName strings at runtime without direct reference 
            /// to specific hashing function
            static InterfaceIDType InterfaceIDHash(const std::string& instrName)
            {
                return Core::HashCRC32::GenerateHashCode(instrName);
            }
            //-------------------------------------------------------
            /// Destructor
            ///
            /// @author S McGaw
            //-------------------------------------------------------
			virtual ~QueryableInterface(){}
            //-------------------------------------------------------
            /// Query whether the object implements an interface
            /// that has the given ID
            ///
            /// @author S McGaw
            ///
            /// @param Interface ID
            ///
            /// @return Whether the object implements an interface
            /// that has the given ID
            //-------------------------------------------------------
			virtual bool IsA(InterfaceIDType in_interfaceID) const = 0;
            //----------------------------------------------------
            /// @author A Glass
            ///
            /// @return Interface ID of the class
            //----------------------------------------------------
            virtual InterfaceIDType GetInterfaceID() const = 0;
            //----------------------------------------------------
            /// @author A Glass
            ///
            /// @return String representation of interface ID of the class
            //----------------------------------------------------
            virtual const std::string& GetInterfaceTypeName() const = 0;
		};
	}
}

#endif
