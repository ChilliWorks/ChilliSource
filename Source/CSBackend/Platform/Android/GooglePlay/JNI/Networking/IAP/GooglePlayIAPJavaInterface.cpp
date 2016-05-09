//
//  GooglePlayIAPJavaInterface.cpp
//  Chilli Source
//  Created by Scott Downie on 14/06/2013.
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

#ifdef CS_ANDROIDFLAVOUR_GOOGLEPLAY

#include <CSBackend/Platform/Android/GooglePlay/JNI/Networking/IAP/GooglePlayIAPJavaInterface.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Json/JsonUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <json/json.h>

#include <jni.h>

extern "C"
{
	//--------------------------------------------------------------------------------------
	/// Called by Java when the product descriptions have been fetched
	///
	/// @author S Downie
	///
	/// @param in_env - JNI environment
	/// @param in_this - Pointer to the calling function
	/// @param in_productIds - Array of product IDs
	/// @param in_names - Array of product Names
	/// @param in_descs - Array of product Descriptions
	/// @param in_prices - Array of product Prices
	/// @param in_currencyCodes - Array of currency codes
	/// @param in_unformattedPrices - Array of unformatted prices
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_networking_GooglePlayIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* in_env, jobject in_this, jobjectArray in_productIds, jobjectArray in_names, jobjectArray in_descs, jobjectArray in_prices, jobjectArray in_currencyCodes, jobjectArray in_unformattedPrices);
	//--------------------------------------------------------------------------------------
	/// Called by Java when the transaction status updates
	///
	/// @author S Downie
	///
	/// @param in_env - JNI environment
	/// @param in_this - Pointer to the calling function
	/// @param in_result - Result (SUCCESS - 0, FAILED - 1)
	/// @param in_productId - Product ID
	/// @param in_transactionId - Transaction ID
	/// @param in_receipt - Receipt
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_networking_GooglePlayIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* in_env, jobject in_this, jint in_result, jstring in_productId, jstring in_transactionId, jstring in_receipt);
	//--------------------------------------------------------------------------------------
	/// Called by Java when the transaction close result returns
	///
	/// @param in_env - JNI environment
	/// @param in_this - Pointer to the calling function
	/// @param in_productId - Product ID
	/// @param in_transactionId - Transaction ID
	/// @param in_success - Success or failure
	//--------------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_networking_GooglePlayIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* in_env, jobject in_this, jstring in_productId, jstring in_transactionId, jboolean in_success);
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_networking_GooglePlayIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* in_env, jobject in_this, jobjectArray in_productIds, jobjectArray in_names, jobjectArray in_descs, jobjectArray in_prices, jobjectArray in_currencyCodes, jobjectArray in_unformattedPrices)
{
	CSBackend::Android::GooglePlayIAPJavaInterfaceSPtr javaInterface = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::GooglePlayIAPJavaInterface>();
	if (javaInterface != nullptr)
	{
		u32 numProducts = in_env->GetArrayLength(in_productIds);
		std::vector<ChilliSource::IAPSystem::ProductDesc> products;
		products.reserve(numProducts);

        std::vector<std::string> currencyCodes;
        currencyCodes.reserve(numProducts);

        std::vector<std::string> unformattedPrices;
        unformattedPrices.reserve(numProducts);

		for(u32 i=0; i<numProducts; ++i)
		{
			ChilliSource::IAPSystem::ProductDesc desc;

			jstring id = static_cast<jstring>(in_env->GetObjectArrayElement(in_productIds, i));
			desc.m_id = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(id);

			jstring name = static_cast<jstring>(in_env->GetObjectArrayElement(in_names, i));
			desc.m_name = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(name);

			jstring description = static_cast<jstring>(in_env->GetObjectArrayElement(in_descs, i));
			desc.m_description = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(description);

			jstring price = static_cast<jstring>(in_env->GetObjectArrayElement(in_prices, i));
			desc.m_formattedPrice = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(price);

			products.push_back(desc);

			// Populate the extra product information
			std::string currencyCode = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(static_cast<jstring>(in_env->GetObjectArrayElement(in_currencyCodes, i)));
			currencyCodes.push_back(currencyCode);
			std::string unformattedPrice = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(static_cast<jstring>(in_env->GetObjectArrayElement(in_unformattedPrices, i)));
			unformattedPrices.push_back(unformattedPrice);
		}

		ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
		{
			javaInterface->OnProductDescriptionsRequestComplete(products, currencyCodes, unformattedPrices);
		});
	}
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_networking_GooglePlayIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* in_env, jobject in_this, jint in_result, jstring in_productId, jstring in_transactionId, jstring in_receipt)
{
	CSBackend::Android::GooglePlayIAPJavaInterfaceSPtr javaInterface = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::GooglePlayIAPJavaInterface>();
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
void Java_com_chilliworks_chillisource_networking_GooglePlayIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* in_env, jobject in_this, jstring in_productId, jstring in_transactionId, jboolean in_success)
{
	CSBackend::Android::GooglePlayIAPJavaInterfaceSPtr javaInterface = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::GooglePlayIAPJavaInterface>();
	if (javaInterface != nullptr)
	{
		std::string productId = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(in_productId);
		std::string transactionId = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(in_transactionId);

		ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
		{
			javaInterface->OnTransactionClosed(productId, transactionId, in_success);
		});
	}
}

namespace CSBackend
{
	namespace Android
	{
		namespace
		{
			const std::string k_pendingCacheFile("GoogleIAPPendingTransactions.json");

			//---------------------------------------------------------------
			/// Adds the given transaction ID to the "pending transaction"
			/// cache.
			///
			/// @author S Downie
			///
			/// @param The transaction ID.
			//---------------------------------------------------------------
			void AddToPendingCache(const std::string& in_transactionId)
			{
				Json::Value jData;
				ChilliSource::JsonUtils::ReadJson(ChilliSource::StorageLocation::k_cache, k_pendingCacheFile, jData);
				jData[in_transactionId] = 0;
				ChilliSource::Utils::StringToFile(ChilliSource::StorageLocation::k_cache, k_pendingCacheFile, jData.toStyledString());
			}
			//---------------------------------------------------------------
			/// Removes the given transaction ID from the "pending transaction"
			/// cache.
			///
			/// @author S Downie
			///
			/// @param The transaction ID.
			//---------------------------------------------------------------
			void RemoveFromPendingCache(const std::string& in_transactionId)
			{
				//Remove the transaction from the pending cache
				Json::Value jData;
				if(ChilliSource::JsonUtils::ReadJson(ChilliSource::StorageLocation::k_cache, k_pendingCacheFile, jData))
				{
					jData.removeMember(in_transactionId);
					ChilliSource::Utils::StringToFile(ChilliSource::StorageLocation::k_cache, k_pendingCacheFile, jData.toStyledString());
				}
			}
		}

		CS_DEFINE_NAMEDTYPE(GooglePlayIAPJavaInterface);

		//--------------------------------------------------------------
		//--------------------------------------------------------------
		GooglePlayIAPJavaInterface::GooglePlayIAPJavaInterface()
		{
			CreateNativeInterface("com/chilliworks/chillisource/networking/GooglePlayIAPNativeInterface");
			CreateMethodReference("Init", "()V");
			CreateMethodReference("IsPurchasingEnabled", "()Z");
			CreateMethodReference("RequestProductDescriptions", "([Ljava/lang/String;)V");
			CreateMethodReference("CancelProductDescriptionsRequest", "()V");
			CreateMethodReference("RequestProductPurchase", "(Ljava/lang/String;I)V");
			CreateMethodReference("CloseTransaction", "(Ljava/lang/String;Ljava/lang/String;)V");
			CreateMethodReference("RestoreManagedPurchases", "()V");
			CreateMethodReference("RestorePendingUnmanagedTransactions", "()V");
			CreateMethodReference("RestorePendingManagedTransactions", "([Ljava/lang/String;)V");

			//initialise the system
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("Init"));
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool GooglePlayIAPJavaInterface::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == GooglePlayIAPJavaInterface::InterfaceID);
		}
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        bool GooglePlayIAPJavaInterface::IsPurchasingEnabled()
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallBooleanMethod(GetJavaObject(), GetMethodID("IsPurchasingEnabled"));
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::StartListeningForTransactionUpdates(const ChilliSource::IAPSystem::TransactionStatusDelegate& in_delegate)
        {
        	CS_ASSERT(in_delegate != nullptr, "Cannot have null transaction delegate");

        	m_transactionStatusDelegate = in_delegate;

        	JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	env->CallVoidMethod(GetJavaObject(), GetMethodID("RestorePendingUnmanagedTransactions"));

        	Json::Value jData;
        	if(ChilliSource::JsonUtils::ReadJson(ChilliSource::StorageLocation::k_cache, k_pendingCacheFile, jData))
        	{
        		const Json::Value::Members& members = jData.getMemberNames();
        		jobjectArray transactionIDs = env->NewObjectArray(members.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));

        		u32 index = 0;
        		for(Json::Value::Members::const_iterator it = members.begin(); it != members.end(); ++it, ++index)
        		{
    				jstring jtransactionId = JavaUtils::CreateJStringFromSTDString(*it);
    				env->SetObjectArrayElement(transactionIDs, index, jtransactionId);
    				env->DeleteLocalRef(jtransactionId);
        		}

            	env->CallVoidMethod(GetJavaObject(), GetMethodID("RestorePendingManagedTransactions"), transactionIDs);
            	env->DeleteLocalRef(transactionIDs);
        	}
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::StopListeningForTransactionUpdates()
        {
        	m_transactionStatusDelegate = nullptr;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::RequestProductDescriptions(const std::vector<std::string>& in_productIds, const OnProductDescriptionsRequestCompleteDelegate& in_delegate)
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
				jstring jproductId = JavaUtils::CreateJStringFromSTDString(*it);
				env->SetObjectArrayElement(productIDs, productIDIndex, jproductId);
				env->DeleteLocalRef(jproductId);
				productIDIndex++;
			}

			env->CallVoidMethod(GetJavaObject(), GetMethodID("RequestProductDescriptions"), productIDs);

			env->DeleteLocalRef(productIDs);
			env->DeleteLocalRef(jstrEmptyString);
			env->DeleteLocalRef(jStringClass);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::OnProductDescriptionsRequestComplete(const std::vector<ChilliSource::IAPSystem::ProductDesc>& in_products, const std::vector<std::string>& in_currencyCodes, const std::vector<std::string>& in_unformattedPrices)
        {
        	if(m_productsRequestDelegate == nullptr)
        	{
        		return;
        	}

        	m_productsRequestDelegate(in_products, in_currencyCodes, in_unformattedPrices);
        	m_productsRequestDelegate = nullptr;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::CancelProductDescriptionsRequest()
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("CancelProductDescriptionsRequest"));
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::RequestProductPurchase(const std::string& in_productId, ChilliSource::IAPSystem::ProductRegInfo::Type in_type)
        {
        	const u32 k_managedType = 0;
        	const u32 k_unManagedType = 1;
        	u32 type = k_unManagedType;

        	switch(in_type)
        	{
        		case ChilliSource::IAPSystem::ProductRegInfo::Type::k_managed:
        			type = k_managedType;
        			break;
        		case ChilliSource::IAPSystem::ProductRegInfo::Type::k_unmanaged:
        			type = k_unManagedType;
        			break;
        	}

			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jproductId = JavaUtils::CreateJStringFromSTDString(in_productId);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("RequestProductPurchase"), jproductId, type);
			env->DeleteLocalRef(jproductId);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::OnTransactionStatusUpdated(u32 in_statusId, const ChilliSource::IAPSystem::Transaction& in_transaction)
        {
        	if(m_transactionStatusDelegate == nullptr)
        		return;

        	ChilliSource::IAPSystem::Transaction::Status status = ChilliSource::IAPSystem::Transaction::Status::k_failed;

        	//This requires a little bit of faith. These numbers correspond to the constants defined
        	//in the java transaction description
        	//    	public final static int SUCCEEDED = 0;
        	//    	public final static int FAILED = 1;
        	//    	public final static int CANCELLED = 2;
    		//		public final static int RESTORED = 3;
    		//		public final static int RESUMED = 4;
        	//		public final static int REFUNDED = 5;
        	switch(in_statusId)
        	{
        		case 0:
        			status = ChilliSource::IAPSystem::Transaction::Status::k_succeeded;
        			AddToPendingCache(in_transaction.m_transactionId);
        			break;
        		case 1:
        			status = ChilliSource::IAPSystem::Transaction::Status::k_failed;
        			break;
        		case 2:
        			status = ChilliSource::IAPSystem::Transaction::Status::k_cancelled;
        			break;
        		case 3:
        			status = ChilliSource::IAPSystem::Transaction::Status::k_restored;
        			break;
        		case 4:
        			status = ChilliSource::IAPSystem::Transaction::Status::k_resumed;
        			break;
           		case 5:
           			status = ChilliSource::IAPSystem::Transaction::Status::k_refunded;
            		break;
        	}

        	ChilliSource::IAPSystem::TransactionSPtr transaction(new ChilliSource::IAPSystem::Transaction());
        	transaction->m_productId = in_transaction.m_productId;
        	transaction->m_transactionId = in_transaction.m_transactionId;
        	transaction->m_receipt = in_transaction.m_receipt;

        	m_transactionStatusDelegate(status, transaction);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::CloseTransaction(const std::string& in_productId, const std::string& in_transactionId, const ChilliSource::IAPSystem::TransactionCloseDelegate& in_delegate)
        {
        	CS_ASSERT(in_delegate != nullptr, "Cannot have null transaction close delegate");
        	CS_ASSERT(m_transactionCloseDelegate == nullptr, "Only 1 transaction can be closed at a time");

        	m_transactionCloseDelegate = in_delegate;

			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jproductId = JavaUtils::CreateJStringFromSTDString(in_productId);
			jstring jtransactionId = JavaUtils::CreateJStringFromSTDString(in_transactionId);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("CloseTransaction"), jproductId, jtransactionId);
			env->DeleteLocalRef(jproductId);
			env->DeleteLocalRef(jtransactionId);

			RemoveFromPendingCache(in_transactionId);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::OnTransactionClosed(const std::string& in_productId, const std::string& in_transactionId, bool in_success)
        {
        	if(m_transactionCloseDelegate)
        	{
        		m_transactionCloseDelegate(in_productId, in_transactionId, in_success);
        		m_transactionCloseDelegate = nullptr;
        	}
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::RestoreManagedPurchases()
        {
        	JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	env->CallVoidMethod(GetJavaObject(), GetMethodID("RestoreManagedPurchases"));
        }
	}
}

#endif

#endif
