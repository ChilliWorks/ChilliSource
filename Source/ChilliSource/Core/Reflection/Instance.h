//
//  Instance.hpp
//  MoFlowSkeleton
//
//  Created by Scott Downie on 30/11/2011.
//  Copyright (c) 2011 Tag Games. All rights reserved.
//

#ifndef MO_FLOW_CORE_REFLECTION_INSTANCE_HPP_
#define MO_FLOW_CORE_REFLECTION_INSTANCE_HPP_

#include <ChilliSource/Core/Reflection/ForwardDeclarations.h>

namespace moFlo
{
    namespace Reflect
    {
        //===============================================================================
        /// Instance
        ///
        /// An instance of a class object. This holds meta information relating to the
        /// class.  
        //===============================================================================
        class CInstance
        {
        public:
            ~CInstance();
            //--------------------------------------------------------
            /// Create 
            ///
            /// Create a new instance of the class represented by
            /// the given meta class
            ///
            /// @param Instance name
			/// @param Meta class
            /// @return Instance of class
            //--------------------------------------------------------
            static CInstance* Create(const std::string& instrName, CMetaClass* inpMetaClass);
			//--------------------------------------------------------
			/// Create 
			///
			/// Create a new meta instance of the class represented by
			/// the given meta class with the given instance
			///
			/// @param Instance pointer
			/// @param Instance name
			/// @param Meta class
			/// @return Instance of class
			//--------------------------------------------------------
			static CInstance* Create(void* inpInstance, const std::string& instrName, CMetaClass* inpMetaClass);
			//--------------------------------------------------------
			/// Destroy 
			///
			/// Delete this instance of the class represented by
			/// the given meta class
			//--------------------------------------------------------
			void Destroy();
            //--------------------------------------------------------
            /// Create 
            ///
            /// @return Instance of class
            //--------------------------------------------------------
            template <typename T> static void* Create(CInstance* inpInstance)
            {
				T* pInstance = new T();
				pInstance->SetMetaInstance(inpInstance);
                return pInstance;
            }
            //--------------------------------------------------------
            /// Destroy 
            ///
            /// Destruct the instance
            /// @param Instance
            //--------------------------------------------------------
            template <typename T> static void Destroy(void* inpInstance)
            {
                delete static_cast<T*>(inpInstance);
            } 
            //--------------------------------------------------------
            /// Get Name Hash
            ///
            /// @return The name identifier of this instance
            //--------------------------------------------------------
            u32 GetNameHash() const;
            //--------------------------------------------------------
            /// Get Name
            ///
            /// @return The name identifier of this instance
            //--------------------------------------------------------
            const std::string& GetName() const;
			//--------------------------------------------------------
			/// Set Name
			///
			/// @param The name identifier for this instance
			//--------------------------------------------------------
			void SetName(const std::string& instrName);
            //--------------------------------------------------------
            /// Get Class
            ///
            /// @return The meta class for this instance
            //--------------------------------------------------------
            CMetaClass* GetClass() const;
			//--------------------------------------------------------
			/// Set Instance
			///
			/// @param The concrete instance wrapped by the meta-type
			//--------------------------------------------------------
			void SetInstance(void* inpInstance);
            //--------------------------------------------------------
            /// Get Instance
            ///
            /// @return The concrete instance wrapped by the meta-type
            //--------------------------------------------------------
            void* GetInstance() const;
            
        private:
            
            //--------------------------------------------------------
            /// Constructor
            ///
            /// @param Instance name
            /// @param Meta-class of which this is an instance
            //--------------------------------------------------------
            CInstance(const std::string& instrName, CMetaClass* inpClass);
            
        private:
            
            std::string mstrName;
            
            CMetaClass* mpClass;
            
            void* mpInstance;
            
            u32 mudwNameHash;
        };
    }
}

#endif
