//
//  AmazonIAPJavaInterface.cpp
//  Chilli Source
//
//  Created by Ian Copland on 10/12/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#ifdef CS_ANDROIDEXTENSION_AMAZON

#include <ChilliSource/Backend/Platform/Android/Extensions/Amazon/AmazonIAPJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/JSON/json.h>

#include <jni.h>

extern "C"
{
	//--------------------------------------------------------------------------------------
	/// Called by Java when product descriptions request completes
	///
	/// @author I Copland
	///
	/// @param JNI Environment
	/// @param Pointer to the calling object
	/// @param Array of product IDs
	/// @param Array of product Names
	/// @param Array of product Descriptions
	/// @param Array of product Prices
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_amazon_AmazonIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* in_env, jobject in_this, jobjectArray in_productIds, jobjectArray in_names, jobjectArray in_descs, jobjectArray in_prices);
	//--------------------------------------------------------------------------------------
	/// Called by Java when a transaction status changes
	///
	/// @author I Copland
	///
	/// @param JNI Environment
	/// @param Pointer to the calling object
	/// @param Result
	/// @param Product ID
	/// @param Transaction ID
	/// @param Receipt
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_amazon_AmazonIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* in_env, jobject in_this, jint inudwResult, jstring in_productId, jstring in_transactionId, jstring in_receipt);
	//--------------------------------------------------------------------------------------
	/// Called by Java when a transaction complete request completes
	///
	/// @author I Copland
	///
	/// @param JNI Environment
	/// @param Pointer to the calling function
	//--------------------------------------------------------------------------------------
	void Java_com_chillisource_amazon_AmazonIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* in_env, jobject in_this, jstring in_productId, jstring in_transactionId);
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_amazon_AmazonIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* in_env, jobject in_this, jobjectArray in_productIds, jobjectArray in_names, jobjectArray in_descs, jobjectArray in_prices)
{
	ChilliSource::Android::AmazonIAPJavaInterfaceSPtr javaInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::AmazonIAPJavaInterface>();
	if (javaInterface != nullptr)
	{
		u32 numProducts = in_env->GetArrayLength(in_productIds);
		std::vector<CSNetworking::IAPSystem::ProductDesc> products;
		products.reserve(numProducts);

		for(u32 i=0; i<numProducts; ++i)
		{
			CSNetworking::IAPSystem::ProductDesc desc;
			jstring id = (jstring)in_env->GetObjectArrayElement(in_productIds, i);
			desc.m_id = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(id);

			jstring name = (jstring)in_env->GetObjectArrayElement(in_names, i);
			desc.m_name = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(name);

			jstring description = (jstring)in_env->GetObjectArrayElement(in_descs, i);
			desc.m_description = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(description);

			jstring price = (jstring)in_env->GetObjectArrayElement(in_prices, i);
			desc.m_formattedPrice = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(price);

			products.push_back(desc);
		}

		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&ChilliSource::Android::AmazonIAPJavaInterface::OnProductDescriptionsRequestComplete, javaInterface.get(), products));
	}
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_amazon_AmazonIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* in_env, jobject in_this, jint in_result, jstring in_productId, jstring in_transactionId, jstring in_receipt)
{
	ChilliSource::Android::AmazonIAPJavaInterfaceSPtr javaInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::AmazonIAPJavaInterface>();
	if (javaInterface != nullptr)
	{
		CSNetworking::IAPSystem::Transaction transaction;
		transaction.m_productId = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(in_productId);
		transaction.m_transactionId = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(in_transactionId);
		transaction.m_receipt = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(in_receipt);
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&ChilliSource::Android::AmazonIAPJavaInterface::OnTransactionStatusUpdated, javaInterface.get(), in_result, transaction));
	}
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chillisource_amazon_AmazonIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* in_env, jobject in_this, jstring in_productId, jstring in_transactionId)
{
	ChilliSource::Android::AmazonIAPJavaInterfaceSPtr javaInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::AmazonIAPJavaInterface>();
	if (javaInterface != nullptr)
	{
		std::string productId = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(in_productId);
		std::string transactionId = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(in_transactionId);
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(&ChilliSource::Android::AmazonIAPJavaInterface::OnTransactionClosed, javaInterface.get(), productId, transactionId));
	}
}

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(AmazonIAPJavaInterface);
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		AmazonIAPJavaInterface::AmazonIAPJavaInterface(const std::string& in_privateKey, const std::string& in_udid)
		{
			CreateNativeInterface("com/chillisource/amazon/AmazonIAPNativeInterface");
			CreateMethodReference("Init", "(Ljava/lang/String;Ljava/lang/String;)V");
			CreateMethodReference("IsPurchasingEnabled", "()Z");
			CreateMethodReference("RequestProductDescriptions", "([Ljava/lang/String;)V");
			CreateMethodReference("CancelProductDescriptionsRequest", "()V");
			CreateMethodReference("RequestProductPurchase", "(Ljava/lang/String;)V");
			CreateMethodReference("CloseTransaction", "(Ljava/lang/String;Ljava/lang/String;)V");
			CreateMethodReference("RestoreManagedPurchases", "()V");
			CreateMethodReference("StartListeningForTransactionUpdates", "()V");
			CreateMethodReference("StopListeningForTransactionUpdates", "()V");

			//initialise the system
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring privateKey = JavaInterfaceUtils::CreateJStringFromSTDString(in_privateKey);
			jstring udid = JavaInterfaceUtils::CreateJStringFromSTDString(in_udid);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("Init"), privateKey, udid);
			env->DeleteLocalRef(privateKey);
			env->DeleteLocalRef(udid);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool AmazonIAPJavaInterface::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == AmazonIAPJavaInterface::InterfaceID;
		}
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        bool AmazonIAPJavaInterface::IsPurchasingEnabled()
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallBooleanMethod(GetJavaObject(), GetMethodID("IsPurchasingEnabled"));
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::StartListeningForTransactionUpdates(const Networking::IAPSystem::TransactionStatusDelegate& in_delegate)
        {
        	CS_ASSERT(in_delegate != nullptr, "Cannot have null transaction delegate");

        	m_transactionStatusDelegate = in_delegate;

        	JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	env->CallVoidMethod(GetJavaObject(), GetMethodID("StartListeningForTransactionUpdates"));
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::StopListeningForTransactionUpdates()
        {
        	m_transactionStatusDelegate = nullptr;

        	JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	env->CallVoidMethod(GetJavaObject(), GetMethodID("StopListeningForTransactionUpdates"));
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::RequestProductDescriptions(const std::vector<std::string>& in_productIds, const Networking::IAPSystem::ProductDescDelegate& in_delegate)
        {
            CS_ASSERT(in_productIds.empty() == false, "Cannot request no product descriptions");
            CS_ASSERT(in_delegate != nullptr, "Cannot have null product description delegate");
            CS_ASSERT(m_productsRequestDelegate == nullptr, "Only 1 product description request can be active at a time");

        	m_productsRequestDelegate = in_delegate;

        	JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

			jstring jstrEmptyString = env->NewStringUTF("");
			jclass jStringClass = env->FindClass("java/lang/String");

        	jobjectArray productIDs = env->NewObjectArray(in_productIds.size(), jStringClass, jstrEmptyString);
        	u32 productIDIndex = 0;
        	for(std::vector<std::string>::const_iterator it = in_productIds.begin(); it != in_productIds.end(); ++it)
        	{
        		jstring productID = JavaInterfaceUtils::CreateJStringFromSTDString(*it);
        		env->SetObjectArrayElement(productIDs, productIDIndex, productID);
        		env->DeleteLocalRef(productID);
        		productIDIndex++;
        	}

        	env->CallVoidMethod(GetJavaObject(), GetMethodID("RequestProductDescriptions"), productIDs);
        	env->DeleteLocalRef(productIDs);
			env->DeleteLocalRef(jstrEmptyString);
			env->DeleteLocalRef(jStringClass);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::OnProductDescriptionsRequestComplete(const std::vector<Networking::IAPSystem::ProductDesc>& in_products)
        {
        	if(m_productsRequestDelegate != nullptr)
        	{
        		m_productsRequestDelegate(in_products);
        		m_productsRequestDelegate = nullptr;
        	}
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::CancelProductDescriptionsRequest()
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("CancelProductDescriptionsRequest"));
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::RequestProductPurchase(const std::string& in_productId)
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring productID = JavaInterfaceUtils::CreateJStringFromSTDString(in_productId);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("RequestProductPurchase"), productID);
			env->DeleteLocalRef(productID);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::OnTransactionStatusUpdated(u32 in_statusId, const Networking::IAPSystem::Transaction& in_transaction)
        {
        	if(m_transactionStatusDelegate != nullptr)
        	{
				Networking::IAPSystem::Transaction::Status status = Networking::IAPSystem::Transaction::Status::k_failed;

				//This requires a little bit of faith. These numbers correspond to the constants defined
				//in the java class CPurchaseTransaction
				const u32 k_purchaseSucceeded = 0;
				const u32 k_purchaseResumed = 1;
				const u32 k_purchaseRestored = 2;
				const u32 k_purchaseFailed = 3;

				switch(in_statusId)
				{
					case k_purchaseSucceeded:
						status = Networking::IAPSystem::Transaction::Status::k_succeeded;
						break;
					case k_purchaseResumed:
						status = Networking::IAPSystem::Transaction::Status::k_resumed;
						break;
					case k_purchaseRestored:
						status = Networking::IAPSystem::Transaction::Status::k_restored;
						break;
					case k_purchaseFailed:
					default:
						status = Networking::IAPSystem::Transaction::Status::k_failed;
						break;
				}

				Networking::IAPSystem::TransactionSPtr transaction(new Networking::IAPSystem::Transaction());
				transaction->m_productId = in_transaction.m_productId;
				transaction->m_transactionId = in_transaction.m_transactionId;
				transaction->m_receipt = in_transaction.m_receipt;

				m_transactionStatusDelegate(status, transaction);
        	}
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::CloseTransaction(const std::string& in_productId, const std::string& in_transactionId, const Networking::IAPSystem::TransactionCloseDelegate& in_delegate)
        {
        	CS_ASSERT(in_delegate != nullptr, "Cannot have null transaction close delegate");
        	CS_ASSERT(m_transactionCloseDelegate == nullptr, "Only 1 transaction can be closed at a time");

        	m_transactionCloseDelegate = in_delegate;

			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring productId = JavaInterfaceUtils::CreateJStringFromSTDString(in_productId);
			jstring transactionId = JavaInterfaceUtils::CreateJStringFromSTDString(in_transactionId);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("CloseTransaction"), productId, transactionId);
			env->DeleteLocalRef(productId);
			env->DeleteLocalRef(transactionId);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::OnTransactionClosed(const std::string& in_productId, const std::string& in_transactionId)
        {
        	if(m_transactionCloseDelegate != nullptr)
        	{
        		m_transactionCloseDelegate(in_productId, in_transactionId, true);
        		m_transactionCloseDelegate = nullptr;
        	}
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::RestoreManagedPurchases()
        {
        	JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	env->CallVoidMethod(GetJavaObject(), GetMethodID("RestoreManagedPurchases"));
        }
	}
}

#endif
