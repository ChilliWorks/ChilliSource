//
//  AmazonIAPJavaInterface.cpp
//  Chilli Source
//  Created by Ian Copland on 10/12/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifdef CS_ANDROIDFLAVOUR_AMAZON

#include <CSBackend/Platform/Android/Amazon/JNI/Networking/IAP/AmazonIAPJavaInterface.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <json/json.h>

#include <jni.h>

extern "C"
{
	//--------------------------------------------------------------------------------------
	/// Called by Java when product descriptions request completes
	///
	/// @author Ian Copland
	///
	/// @param JNI Environment
	/// @param Pointer to the calling object
	/// @param Array of product IDs
	/// @param Array of product Names
	/// @param Array of product Descriptions
	/// @param Array of product Prices
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_networking_AmazonIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* in_env, jobject in_this, jobjectArray in_productIds, jobjectArray in_names, jobjectArray in_descs, jobjectArray in_prices);
	//--------------------------------------------------------------------------------------
	/// Called by Java when a transaction status changes
	///
	/// @author Ian Copland
	///
	/// @param JNI Environment
	/// @param Pointer to the calling object
	/// @param Result
	/// @param Product ID
	/// @param Transaction ID
	/// @param Receipt
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_networking_AmazonIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* in_env, jobject in_this, jint inudwResult, jstring in_productId, jstring in_transactionId, jstring in_receipt);
	//--------------------------------------------------------------------------------------
	/// Called by Java when a transaction complete request completes
	///
	/// @author Ian Copland
	///
	/// @param JNI Environment
	/// @param Pointer to the calling function
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_networking_AmazonIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* in_env, jobject in_this, jstring in_productId, jstring in_transactionId);
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_networking_AmazonIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* in_env, jobject in_this, jobjectArray in_productIds, jobjectArray in_names, jobjectArray in_descs, jobjectArray in_prices)
{
	CSBackend::Android::AmazonIAPJavaInterfaceSPtr javaInterface = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::AmazonIAPJavaInterface>();
	if (javaInterface != nullptr)
	{
		u32 numProducts = in_env->GetArrayLength(in_productIds);
		std::vector<ChilliSource::IAPSystem::ProductDesc> products;
		products.reserve(numProducts);

		for(u32 i=0; i<numProducts; ++i)
		{
			ChilliSource::IAPSystem::ProductDesc desc;
			jstring id = (jstring)in_env->GetObjectArrayElement(in_productIds, i);
			desc.m_id = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(id);

			jstring name = (jstring)in_env->GetObjectArrayElement(in_names, i);
			desc.m_name = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(name);

			jstring description = (jstring)in_env->GetObjectArrayElement(in_descs, i);
			desc.m_description = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(description);

			jstring price = (jstring)in_env->GetObjectArrayElement(in_prices, i);
			desc.m_formattedPrice = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(price);

			products.push_back(desc);
		}

		ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
		{
			javaInterface->OnProductDescriptionsRequestComplete(products);
		});
	}
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_networking_AmazonIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* in_env, jobject in_this, jint in_result, jstring in_productId, jstring in_transactionId, jstring in_receipt)
{
	CSBackend::Android::AmazonIAPJavaInterfaceSPtr javaInterface = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::AmazonIAPJavaInterface>();
	if (javaInterface != nullptr)
	{
		ChilliSource::IAPSystem::Transaction transaction;
		transaction.m_productId = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(in_productId);
		transaction.m_transactionId = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(in_transactionId);
		transaction.m_receipt = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(in_receipt);

		ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
		{
			javaInterface->OnTransactionStatusUpdated(in_result, transaction);
		});
	}
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_networking_AmazonIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* in_env, jobject in_this, jstring in_productId, jstring in_transactionId)
{
	CSBackend::Android::AmazonIAPJavaInterfaceSPtr javaInterface = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::AmazonIAPJavaInterface>();
	if (javaInterface != nullptr)
	{
		std::string productId = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(in_productId);
		std::string transactionId = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(in_transactionId);

		ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
		{
			javaInterface->OnTransactionClosed(productId, transactionId);
		});
	}
}

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(AmazonIAPJavaInterface);
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		AmazonIAPJavaInterface::AmazonIAPJavaInterface(const std::string& in_privateKey, const std::string& in_udid)
		{
			CreateNativeInterface("com/chilliworks/chillisource/networking/AmazonIAPNativeInterface");
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
			jstring privateKey = JavaUtils::CreateJStringFromSTDString(in_privateKey);
			jstring udid = JavaUtils::CreateJStringFromSTDString(in_udid);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("Init"), privateKey, udid);
			env->DeleteLocalRef(privateKey);
			env->DeleteLocalRef(udid);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool AmazonIAPJavaInterface::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
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
        void AmazonIAPJavaInterface::StartListeningForTransactionUpdates(const ChilliSource::IAPSystem::TransactionStatusDelegate& in_delegate)
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
        void AmazonIAPJavaInterface::RequestProductDescriptions(const std::vector<std::string>& in_productIds, const ChilliSource::IAPSystem::ProductDescDelegate& in_delegate)
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
        		jstring productID = JavaUtils::CreateJStringFromSTDString(*it);
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
        void AmazonIAPJavaInterface::OnProductDescriptionsRequestComplete(const std::vector<ChilliSource::IAPSystem::ProductDesc>& in_products)
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
			jstring productID = JavaUtils::CreateJStringFromSTDString(in_productId);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("RequestProductPurchase"), productID);
			env->DeleteLocalRef(productID);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::OnTransactionStatusUpdated(u32 in_statusId, const ChilliSource::IAPSystem::Transaction& in_transaction)
        {
        	if(m_transactionStatusDelegate != nullptr)
        	{
				ChilliSource::IAPSystem::Transaction::Status status = ChilliSource::IAPSystem::Transaction::Status::k_failed;

				//This requires a little bit of faith. These numbers correspond to the constants defined
				//in the java class CPurchaseTransaction
				const u32 k_purchaseSucceeded = 0;
				const u32 k_purchaseResumed = 1;
				const u32 k_purchaseRestored = 2;
				const u32 k_purchaseFailed = 3;

				switch(in_statusId)
				{
					case k_purchaseSucceeded:
						status = ChilliSource::IAPSystem::Transaction::Status::k_succeeded;
						break;
					case k_purchaseResumed:
						status = ChilliSource::IAPSystem::Transaction::Status::k_resumed;
						break;
					case k_purchaseRestored:
						status = ChilliSource::IAPSystem::Transaction::Status::k_restored;
						break;
					case k_purchaseFailed:
					default:
						status = ChilliSource::IAPSystem::Transaction::Status::k_failed;
						break;
				}

				ChilliSource::IAPSystem::TransactionSPtr transaction(new ChilliSource::IAPSystem::Transaction());
				transaction->m_productId = in_transaction.m_productId;
				transaction->m_transactionId = in_transaction.m_transactionId;
				transaction->m_receipt = in_transaction.m_receipt;

				m_transactionStatusDelegate(status, transaction);
        	}
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::CloseTransaction(const std::string& in_productId, const std::string& in_transactionId, const ChilliSource::IAPSystem::TransactionCloseDelegate& in_delegate)
        {
        	CS_ASSERT(in_delegate != nullptr, "Cannot have null transaction close delegate");
        	CS_ASSERT(m_transactionCloseDelegate == nullptr, "Only 1 transaction can be closed at a time");

        	m_transactionCloseDelegate = in_delegate;

			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring productId = JavaUtils::CreateJStringFromSTDString(in_productId);
			jstring transactionId = JavaUtils::CreateJStringFromSTDString(in_transactionId);
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

#endif
