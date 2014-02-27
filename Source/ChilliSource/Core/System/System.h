/** 
 * File: ForwardDeclarations.h
 * Date: 27 Sept 2010
 * Description: Declares types and typedefs used by moFlo's Physics2D interface
 */

/** 
 *
 * Author Stuart McGaw
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_CORE_SYSTEM_H
#define _MOFLO_CORE_SYSTEM_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

namespace ChilliSource
{
	namespace Core
	{
		class System : public QueryableInterface
		{
		public:
			virtual ~System(){}
            
            //----------------------------------------------------
            /// Get Interface ID
            ///
            /// @return Interface ID of the class
            //----------------------------------------------------
            virtual InterfaceIDType GetInterfaceID() const = 0;
            //----------------------------------------------------
            /// Get Interface Type Name
            ///
            /// @return String representation of interface ID of the class
            //----------------------------------------------------
            virtual const std::string& GetInterfaceTypeName() const = 0;
		};
		
		typedef std::shared_ptr<System> SystemSPtr;
	}
}

#endif