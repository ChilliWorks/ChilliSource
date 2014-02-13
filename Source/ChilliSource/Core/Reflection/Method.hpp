//
//  Method.hpp
//  MoFlowSkeleton
//
//  Created by Robert Henning on 30/11/2011.
//  Copyright (c) 2011 Tag Games. All rights reserved.
//

#ifndef MO_FLOW_CORE_REFLECTION_METHOD_HPP_
#define MO_FLOW_CORE_REFLECTION_METHOD_HPP_

#include <moFlo/Core/Reflection/ForwardDeclarations.h>


namespace moFlo
{
    namespace Reflect
    {
        //===============================================================================
        /// Method
        ///
        /// An exposed member function that can be invoked by name
        //===============================================================================
        template <typename T, typename RType> class CMethod
        {
        public:
            
//            CMethod(const std::string& instrName, RType (T::* inpMethodPtr)(), CMetaClass<T>* inpClass)
//            : mstrName(instrName), mpMethodPtr(inpMethodPtr), mpClass(inpClass){}
//            
//            RType Invoke() //Static
//            {
//                return (mpMethodPtr)();
//            }
//            
//            //            {
//            //                (pInstance->*mpMethodPtr)()
//            //            }
//            
//        public:
//            
//            typedef RType ReturnType;
//            
//        private:
//            
//            RType (T::* mpMethodPtr)();
//            std::string mstrName;
//            CMetaClass<T>* mpClass;
        };
    }
}

#endif
