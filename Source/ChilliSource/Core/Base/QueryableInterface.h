//
//  QueryableInterface.h
//  Chilli Source
//  Created by Stuart McGaw on 27/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_QUERYABLEINTERFACE_H_
#define _CHILLISOURCE_CORE_QUERYABLEINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/String/StringUtils.h>

#define CS_DECLARE_NAMEDTYPE(x) \
    static const ::CSCore::InterfaceIDType InterfaceID; \
    static const ::std::string TypeName; \
    virtual ::CSCore::InterfaceIDType GetInterfaceID() const; \
    virtual const ::std::string& GetInterfaceTypeName() const;

#define CS_DEFINE_NAMEDTYPE(x) \
    const ::CSCore::InterfaceIDType x::InterfaceID = ::CSCore::QueryableInterface::InterfaceIDHash(#x); \
    const ::std::string x::TypeName = #x; \
    ::CSCore::InterfaceIDType x::GetInterfaceID() const \
    { \
        return InterfaceID; \
    } \
    const ::std::string& x::GetInterfaceTypeName() const \
    { \
        return TypeName; \
    }

#define CS_DEFINE_NAMEDTYPETEMPLATED(in_templatedClass, in_templateParams) \
	template <typename in_templateParams> const ::CSCore::InterfaceIDType in_templatedClass<in_templateParams>::InterfaceID = ::CSCore::QueryableInterface::InterfaceIDHash(#in_templatedClass#in_templateParams); \
	template <typename in_templateParams> const ::std::string in_templatedClass<in_templateParams>::TypeName = #in_templatedClass#in_templateParams; \
	template <typename in_templateParams> ::CSCore::InterfaceIDType in_templatedClass<in_templateParams>::GetInterfaceID() const \
    { \
    return InterfaceID; \
    } \
	template <typename in_templateParams> const ::std::string& in_templatedClass<in_templateParams>::GetInterfaceTypeName() const \
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
            //-------------------------------------------------------
            /// Allow hashing of TypeName strings at runtime without
            /// direct reference to specific hashing function.
            ///
            /// @author S McGaw
            ///
            /// @param The interface name.
            ///
            /// @param The interface Id.
            //-------------------------------------------------------
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
            /// Queries whether or not this object implements the
            /// interface given as a template parameter. The interface
            /// must be a named type, i.e it must make use of the
            /// CS_DECLARE_NAMEDTYPE() and CS_DEFINE_NAMEDTYPE()
            /// macros.
            ///
            /// @author Ian Copland
            ///
            /// @return Whether the object implements the given
            /// interface.
            //-------------------------------------------------------
            template <typename TNamedType> bool IsA() const;
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
			virtual bool IsA(InterfaceIDType in_interfaceId) const = 0;
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
        //-------------------------------------------------------
        //-------------------------------------------------------
        template <typename TNamedType> bool QueryableInterface::IsA() const
        {
            return IsA(TNamedType::InterfaceID);
        }
	}
}

#endif
