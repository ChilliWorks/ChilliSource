//
//  GenericFactory.h
//  Chilli Source
//  Created by Ian Copland on 08/11/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _ICENGINE_RENDERING_PARTICLES_GENERICFACTORY_H_
#define _ICENGINE_RENDERING_PARTICLES_GENERICFACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <json/json.h>

#include <functional>
#include <unordered_map>

namespace ChilliSource
{
	namespace Core
	{
		//--------------------------------------------------------------------------
		/// A generic factory for creating concrete instances of an abstract class
		/// using only a string name and json parameters. Instances can be created
		/// either on the main thread or asynchronously.
		///
		/// The object being constructed must provide two constructors: 
		/// 
		/// TConcreteType(const Json::Value&);
		/// TConcreteType(const Json::Value&, const std::function<void(TAbstractType*)>);
		///
		/// @author Ian Copland
		//--------------------------------------------------------------------------
		template <typename TAbstractType> class GenericFactory : public Core::AppSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(GenericFactory);
			//-----------------------------------------------------------------
			/// Asynchronous load completion delegate. This will be called once
			/// the object has been fully loaded.
			/// 
			/// @author Ian Copland
			///
			/// @param The loaded affector def
			//-----------------------------------------------------------------
			using AsyncLoadDelegate = std::function<void(std::unique_ptr<TAbstractType> in_abstract)>;
			//-----------------------------------------------------------------
			/// Registers a new particle affector definition type with the 
			/// factory. Future calls to CreateAffectorDef() with the given name 
			/// will instantiate a particle affector definition of this type. 
			/// The given name must be unique, this will try to assert if it is
			/// not.
			///
			/// This is not thread-safe and should only be called on the main
			/// thread.
			///
			/// @author Ian Copland
			///
			/// @param The name of the affector def.
			//-----------------------------------------------------------------
			template <typename TConcreteType> void Register(const std::string& in_concreteTypeName);
			//-----------------------------------------------------------------
			/// Creates an instance of the concrete type registered under the 
			/// given name with the given json description. 
			///
			/// This is in not thread-safe and should only be called on the
			/// main thread. CreateAsync() should be used for background loading.
			///
			/// @author Ian Copland
			///
			/// @param The name of the concrete type previously registered with
			/// the factory.
			/// @param A json object describing the parameters of the concrete
			/// type.
			///
			/// @return The new instance.
			//-----------------------------------------------------------------
			std::unique_ptr<TAbstractType> CreateInstance(const std::string& in_name, const Json::Value& in_jsonParams) const;
			//-----------------------------------------------------------------
			/// Creates an instance of the concrete type registered under the 
			/// given name with the given json description. The object will
			/// be asynchronously created and returned via the async load 
			/// delegate.
			///
			/// @author Ian Copland
			///
			/// @param The name of the concrete type previously registered with
			/// the factory.
			/// @param A json object describing the parameters of the concrete
			/// type.
			/// @param The async load delegate.
			//-----------------------------------------------------------------
			void CreateInstanceAsync(const std::string& in_name, const Json::Value& in_jsonParams, const AsyncLoadDelegate& in_asyncLoadDelegate) const;
		private:
			//-----------------------------------------------------------------
			/// A method for sub-classes to register their default concrete
			/// classes. This is called during OnInit().
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			virtual void RegisterDefaults() = 0;
		protected:
			//-----------------------------------------------------------------
			/// A delegate which is used to instantiate the registered concrete
			/// class.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the parameters to create the
			/// concrete class with.
			//-----------------------------------------------------------------
			using CreatorDelegate = std::function<std::unique_ptr<TAbstractType>(const Json::Value& in_jsonParams)>;
			//-----------------------------------------------------------------
			/// A delegate which is used to instantiate the registered concrete
			/// class asynchronously.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the parameters to create the
			/// concrete class with.
			/// @param The async load delegate.
			//-----------------------------------------------------------------
			using CreatorAsyncDelegate = std::function<void(const Json::Value& in_jsonParams, const AsyncLoadDelegate& in_asyncLoadDelegate)>;
			//-----------------------------------------------------------------
			/// Creates a new instance of the concrete class. This is the method
			/// refered to by the creator delegates.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the parameters for the concrete
			/// class.
			//-----------------------------------------------------------------
			template <typename TConcreteType> std::unique_ptr<TAbstractType> CreateInstance(const Json::Value& in_jsonParams) const;
			//-----------------------------------------------------------------
			/// Creates a new instance of the concrete class asynchronously.
			/// The async load delegate will be called once created.  This is 
			/// the method refered to by the async creator delegates.
			///
			/// @author Ian Copland
			///
			/// @param A json object describing the parameters for the concrete
			/// class.
			/// @param The async load delegate.
			//-----------------------------------------------------------------
			template <typename TConcreteType> void CreateInstanceAsync(const Json::Value& in_jsonParams, const AsyncLoadDelegate& in_asyncLoadDelegate) const;
			//-----------------------------------------------------------------
			/// Initialised the factory calling the RegisterDefaults() method
			/// for subclasses to register their default concrete classes.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			void OnInit() override;
			//-----------------------------------------------------------------
			/// Deregisters all concrete classes.
			///
			/// @author Ian Copland
			//-----------------------------------------------------------------
			void OnDestroy() override;

			std::unordered_map<std::string, CreatorDelegate> m_creatorDelegateMap;
			std::unordered_map<std::string, CreatorAsyncDelegate> m_creatorAsyncDelegateMap;
		};
		CS_DEFINE_NAMEDTYPETEMPLATED(GenericFactory, TAbstractType);
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		template <typename TAbstractType> template <typename TConcreteType> void GenericFactory<TAbstractType>::Register(const std::string& in_typeName)
		{
			m_creatorDelegateMap.insert(std::make_pair(in_typeName, Core::MakeDelegate(this, &GenericFactory<TAbstractType>::CreateInstance<TConcreteType>)));
			m_creatorAsyncDelegateMap.insert(std::make_pair(in_typeName, Core::MakeDelegate(this, &GenericFactory<TAbstractType>::CreateInstanceAsync<TConcreteType>)));
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		template <typename TAbstractType> std::unique_ptr<TAbstractType> GenericFactory<TAbstractType>::CreateInstance(const std::string& in_name, const Json::Value& in_jsonParams) const
		{
			auto delegateIt = m_creatorDelegateMap.find(in_name);
			CS_ASSERT(delegateIt != m_creatorDelegateMap.end(), "Could not create object with name: " + in_name);

			return delegateIt->second(in_jsonParams);
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		template <typename TAbstractType> void GenericFactory<TAbstractType>::CreateInstanceAsync(const std::string& in_name, const Json::Value& in_jsonParams, const AsyncLoadDelegate& in_asyncLoadDelegate) const
		{
			auto delegateIt = m_creatorAsyncDelegateMap.find(in_name);
			CS_ASSERT(delegateIt != m_creatorAsyncDelegateMap.end(), "Could not create particle affector def with name: " + in_name);

			delegateIt->second(in_jsonParams, in_asyncLoadDelegate);
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		template <typename TAbstractType> template <typename TConcreteType> std::unique_ptr<TAbstractType> GenericFactory<TAbstractType>::CreateInstance(const Json::Value& in_jsonParams) const
		{
			return std::unique_ptr<TAbstractType>(new TConcreteType(in_jsonParams));
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		template <typename TAbstractType> template <typename TConcreteType> void GenericFactory<TAbstractType>::CreateInstanceAsync(const Json::Value& in_jsonParams, const AsyncLoadDelegate& in_asyncLoadDelegate) const
		{
			new TConcreteType(in_jsonParams, [=](TAbstractType* in_loaded)
			{
				in_asyncLoadDelegate(std::unique_ptr<TAbstractType>(in_loaded));
			});
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		template <typename TAbstractType> void GenericFactory<TAbstractType>::OnInit()
		{
			RegisterDefaults();
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		template <typename TAbstractType> void GenericFactory<TAbstractType>::OnDestroy()
		{
			m_creatorDelegateMap.clear();
		}
	}
}

#endif