//
//  GooglePlayIAPJavaInterface.cpp
//  MoFlow
//
//  Created by Scott Downie on 14/06/2013.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Extensions/GooglePlay/GooglePlayIAPJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/JSON/json.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <jni.h>

const std::string kstrPendingCacheFile = "GoogleIAPPendingTransactions.json";

extern "C"
{
	void Java_com_chillisource_googleplay_GooglePlayIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* inpEnv, jobject inThis, jobjectArray inaIDs, jobjectArray inaNames, jobjectArray inaDescs, jobjectArray inaPrices);
	void Java_com_chillisource_googleplay_GooglePlayIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* inpEnv, jobject inThis, jint inudwResult, jstring instrProductID, jstring instrTransactionID, jstring instrReceipt);
	void Java_com_chillisource_googleplay_GooglePlayIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* inpEnv, jobject inThis, jstring instrProductID, jstring instrTransactionID);
}

//--------------------------------------------------------------------------------------
/// On Products Descriptions Request Complete
///
/// @param JNI environment
/// @param Pointer to the calling function
/// @param Array of product IDs
/// @param Array of product Names
/// @param Array of product Descriptions
/// @param Array of product Prices
//--------------------------------------------------------------------------------------
void Java_com_chillisource_googleplay_GooglePlayIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* inpEnv, jobject inThis, jobjectArray inaIDs, jobjectArray inaNames, jobjectArray inaDescs, jobjectArray inaPrices)
{
	ChilliSource::Android::GooglePlayIAPJavaInterfacePtr pInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::GooglePlayIAPJavaInterface>();
	if (pInterface != NULL)
	{
		u32 udwNumProducts = inpEnv->GetArrayLength(inaIDs);
		std::vector<CSNetworking::IAPProductDesc> aProducts;
		aProducts.reserve(udwNumProducts);

		for(u32 i=0; i<udwNumProducts; ++i)
		{
			CSNetworking::IAPProductDesc sDesc;
			jstring jstrID = (jstring)inpEnv->GetObjectArrayElement(inaIDs, i);
			sDesc.strID = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(jstrID);

			jstring jstrName = (jstring)inpEnv->GetObjectArrayElement(inaNames, i);
			sDesc.strName = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(jstrName);

			jstring jstrDesc = (jstring)inpEnv->GetObjectArrayElement(inaDescs, i);
			sDesc.strDescription = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(jstrDesc);

			jstring jstrPrice = (jstring)inpEnv->GetObjectArrayElement(inaPrices, i);
			sDesc.strFormattedPrice = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(jstrPrice);

			aProducts.push_back(sDesc);
		}

		CSCore::TaskScheduler::ScheduleMainThreadTask(CSCore::Task<const std::vector<CSNetworking::IAPProductDesc>&>(pInterface.get(), &ChilliSource::Android::GooglePlayIAPJavaInterface::OnProductDescriptionsRequestComplete, aProducts));
	}
}
//--------------------------------------------------------------------------------------
/// On Transaction Status Update
///
/// @param JNI environment
/// @param Pointer to the calling function
/// @param Result (SUCCESS - 0, FAILED - 1)
/// @param Product ID
/// @param Transaction ID
/// @param Receipt
//--------------------------------------------------------------------------------------
void Java_com_chillisource_googleplay_GooglePlayIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* inpEnv, jobject inThis, jint inudwResult, jstring instrProductID, jstring instrTransactionID, jstring instrReceipt)
{
	ChilliSource::Android::GooglePlayIAPJavaInterfacePtr pInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::GooglePlayIAPJavaInterface>();
	if (pInterface != NULL)
	{
		CSNetworking::IAPTransaction sTransaction;
		sTransaction.strProductID = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrProductID);
		sTransaction.strTransactionID = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrTransactionID);
		sTransaction.strReceipt = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrReceipt);

		CSCore::TaskScheduler::ScheduleMainThreadTask(CSCore::Task<u32, const CSNetworking::IAPTransaction&>(pInterface.get(), &ChilliSource::Android::GooglePlayIAPJavaInterface::OnTransactionStatusUpdated, inudwResult, sTransaction));
	}
}
//--------------------------------------------------------------------------------------
/// On Transaction Closed
///
/// @param JNI environment
/// @param Pointer to the calling function
//--------------------------------------------------------------------------------------
void Java_com_chillisource_googleplay_GooglePlayIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* inpEnv, jobject inThis, jstring instrProductID, jstring instrTransactionID)
{
	ChilliSource::Android::GooglePlayIAPJavaInterfacePtr pInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::GooglePlayIAPJavaInterface>();
	if (pInterface != NULL)
	{
		const std::string strProductID = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrProductID);
		const std::string strTransactionID = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrTransactionID);
		CSCore::TaskScheduler::ScheduleMainThreadTask(CSCore::Task<std::string, std::string>(pInterface.get(), &ChilliSource::Android::GooglePlayIAPJavaInterface::OnTransactionClosed, strProductID, strTransactionID));
	}
}

namespace ChilliSource
{
	namespace Android
	{
		namespace
		{
			//---------------------------------------------------------------
			/// Add To Pending Cache
			///
			/// Adds the given transaction ID to the "pending transaction"
			/// cache.
			///
			/// @param The transaction ID.
			//---------------------------------------------------------------
			void AddToPendingCache(const std::string& instrTransactionID)
			{
				Json::Value jData;
				Core::Utils::ReadJson(Core::StorageLocation::k_cache, kstrPendingCacheFile, &jData);
				jData[instrTransactionID] = 0;
				Core::Utils::StringToFile(Core::StorageLocation::k_cache, kstrPendingCacheFile, jData.toUnformattedString());
			}
			//---------------------------------------------------------------
			/// Remove From Pending Cache
			///
			/// Removes the given transaction ID from the "pending transaction"
			/// cache.
			///
			/// @param The transaction ID.
			//---------------------------------------------------------------
			void RemoveFromPendingCache(const std::string& instrTransactionID)
			{
				//Remove the transaction from the pending cache
				Json::Value jData;
				if(Core::Utils::ReadJson(Core::StorageLocation::k_cache, kstrPendingCacheFile, &jData))
				{
					jData.removeMember(instrTransactionID);
					Core::Utils::StringToFile(Core::StorageLocation::k_cache, kstrPendingCacheFile, jData.toUnformattedString());
				}
			}
		}

		CS_DEFINE_NAMEDTYPE(CGooglePlayIAPJavaInterface);

		GooglePlayIAPJavaInterface::GooglePlayIAPJavaInterface(const std::string& instrPublicKey)
		{
			CreateNativeInterface("com/chillisource/googleplay/GooglePlayIAPNativeInterface");
			CreateMethodReference("Init", "(Ljava/lang/String;)V");
			CreateMethodReference("IsPurchasingEnabled", "()Z");
			CreateMethodReference("RequestProductDescriptions", "([Ljava/lang/String;)V");
			CreateMethodReference("CancelProductDescriptionsRequest", "()V");
			CreateMethodReference("RequestProductPurchase", "(Ljava/lang/String;I)V");
			CreateMethodReference("CloseTransaction", "(Ljava/lang/String;Ljava/lang/String;)V");
			CreateMethodReference("RestoreManagedPurchases", "()V");
			CreateMethodReference("RestorePendingUnmanagedTransactions", "()V");
			CreateMethodReference("RestorePendingManagedTransactions", "([Ljava/lang/String;)V");

			//initialise the system
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrPublicKey = JavaInterfaceUtils::CreateJStringFromSTDString(instrPublicKey);
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Init"), jstrPublicKey);
			pEnv->DeleteLocalRef(jstrPublicKey);
		}
		//--------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------
		bool GooglePlayIAPJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == GooglePlayIAPJavaInterface::InterfaceID);
		}
        //---------------------------------------------------------------
		/// Is Purchasing Enabled
        //---------------------------------------------------------------
        bool GooglePlayIAPJavaInterface::IsPurchasingEnabled()
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallBooleanMethod(GetJavaObject(), GetMethodID("IsPurchasingEnabled"));
        }
        //---------------------------------------------------------------
		/// Start Listening For Transaction Updates
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::StartListeningForTransactionUpdates(const Networking::IAPTransactionDelegate& inRequestDelegate)
        {
        	mTransactionStatusDelegate = inRequestDelegate;

        	JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	env->CallVoidMethod(GetJavaObject(), GetMethodID("RestorePendingUnmanagedTransactions"));

        	Json::Value jData;
        	if(Core::Utils::ReadJson(Core::StorageLocation::k_cache, kstrPendingCacheFile, &jData))
        	{
        		const Json::Value::Members aMembers = jData.getMemberNames();
        		jobjectArray jaTransactionIDs = env->NewObjectArray(aMembers.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));

        		u32 udwIndex = 0;
        		for(Json::Value::Members::const_iterator it = aMembers.begin(); it != aMembers.end(); ++it, ++udwIndex)
        		{
    				jstring jstrTransactionID = JavaInterfaceUtils::CreateJStringFromSTDString(*it);
    				env->SetObjectArrayElement(jaTransactionIDs, udwIndex, jstrTransactionID);
    				env->DeleteLocalRef(jstrTransactionID);
        		}

            	env->CallVoidMethod(GetJavaObject(), GetMethodID("RestorePendingManagedTransactions"), jaTransactionIDs);
            	env->DeleteLocalRef(jaTransactionIDs);
        	}
        }
        //---------------------------------------------------------------
		/// Stop Listening For Transaction Updates
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::StopListeningForTransactionUpdates()
        {
        	mTransactionStatusDelegate = NULL;
        }
        //---------------------------------------------------------------
		/// Request Product Descriptions
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::RequestProductDescriptions(const std::vector<std::string>& inaProductIDs, const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
        	mProductsRequestDelegate = inRequestDelegate;

        	if(mProductsRequestDelegate == NULL)
        		return;

        	JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

			jobjectArray jaProductIDs = env->NewObjectArray(inaProductIDs.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));

			u32 udwProductIDIndex = 0;
			for(std::vector<std::string>::const_iterator it = inaProductIDs.begin(); it != inaProductIDs.end(); ++it)
			{
				jstring jstrProductID = JavaInterfaceUtils::CreateJStringFromSTDString(*it);
				env->SetObjectArrayElement(jaProductIDs, udwProductIDIndex, jstrProductID);
				env->DeleteLocalRef(jstrProductID);
				udwProductIDIndex++;
			}

			env->CallVoidMethod(GetJavaObject(), GetMethodID("RequestProductDescriptions"), jaProductIDs);

			env->DeleteLocalRef(jaProductIDs);
        }
        //---------------------------------------------------------------
		/// On Product Descriptions Request Complete
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::OnProductDescriptionsRequestComplete(const std::vector<Networking::IAPProductDesc>& inaProducts)
        {
        	if(mProductsRequestDelegate == NULL)
        		return;

        	mProductsRequestDelegate(inaProducts);
        }
        //---------------------------------------------------------------
		/// Cancel Product Descriptions Request
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::CancelProductDescriptionsRequest()
        {
			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("CancelProductDescriptionsRequest"));
        }
        //---------------------------------------------------------------
		/// Request Product Purchase
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::RequestProductPurchase(const std::string& instrProductID, Networking::IAPProductRegInfo::Type ineType)
        {
        	const u32 kManagedType = 0;
        	const u32 kUnManagedType = 1;
        	u32 udwType = kUnManagedType;

        	switch(ineType)
        	{
        		case Networking::IAPProductRegInfo::Type::k_managed:
        			udwType = kManagedType;
        			break;
        		case Networking::IAPProductRegInfo::Type::k_unmanaged:
        			udwType = kUnManagedType;
        			break;
        	}

			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrProductID = JavaInterfaceUtils::CreateJStringFromSTDString(instrProductID);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("RequestProductPurchase"), jstrProductID, udwType);
			env->DeleteLocalRef(jstrProductID);
        }
        //---------------------------------------------------------------
		/// On Transaction Status Updated
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::OnTransactionStatusUpdated(u32 inudwStatus, const Networking::IAPTransaction& inTransaction)
        {
        	if(mTransactionStatusDelegate == NULL)
        		return;

        	Networking::IAPTransaction::Status eStatus = Networking::IAPTransaction::Status::k_failed;

        	//This requires a little bit of faith. These numbers correspond to the constants defined
        	//in the java transaction description
        	//    	public final static int SUCCEEDED = 0;
        	//    	public final static int FAILED = 1;
        	//    	public final static int CANCELLED = 2;
    		//		public final static int RESTORED = 3;
    		//		public final static int RESUMED = 4;
        	//		public final static int REFUNDED = 5;
        	switch(inudwStatus)
        	{
        		case 0:
        			eStatus = Networking::IAPTransaction::Status::k_succeeded;
        			AddToPendingCache(inTransaction.strTransactionID);
        			break;
        		case 1:
        			eStatus = Networking::IAPTransaction::Status::k_failed;
        			break;
        		case 2:
        			eStatus = Networking::IAPTransaction::Status::k_cancelled;
        			break;
        		case 3:
        			eStatus = Networking::IAPTransaction::Status::k_restored;
        			break;
        		case 4:
        			eStatus = Networking::IAPTransaction::Status::k_resumed;
        			break;
           		case 5:
            		eStatus = Networking::IAPTransaction::Status::k_refunded;
            		break;
        	}

        	Networking::IAPTransactionPtr pTransaction(new Networking::IAPTransaction());
        	pTransaction->strProductID = inTransaction.strProductID;
        	pTransaction->strTransactionID = inTransaction.strTransactionID;
        	pTransaction->strReceipt = inTransaction.strReceipt;

        	mTransactionStatusDelegate(eStatus, pTransaction);
        }
        //---------------------------------------------------------------
		/// Close Transaction
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::CloseTransaction(const std::string& instrProductID, const std::string& instrTransactionID, const Networking::IAPTransactionCloseDelegate& inDelegate)
        {
        	mTransactionCloseDelegate = inDelegate;

			JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrProductID = JavaInterfaceUtils::CreateJStringFromSTDString(instrProductID);
			jstring jstrTransactionID = JavaInterfaceUtils::CreateJStringFromSTDString(instrTransactionID);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("CloseTransaction"), jstrProductID, jstrTransactionID);
			env->DeleteLocalRef(jstrProductID);
			env->DeleteLocalRef(jstrTransactionID);

			RemoveFromPendingCache(instrTransactionID);
        }
        //---------------------------------------------------------------
		/// On Transaction Closed
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::OnTransactionClosed(std::string instrProductID, std::string instrTransactionID)
        {
        	if(mTransactionCloseDelegate)
        	{
        		mTransactionCloseDelegate(instrProductID, instrTransactionID);
        	}
        }
        //---------------------------------------------------------------
		/// Restore Managed Purchases
        //---------------------------------------------------------------
        void GooglePlayIAPJavaInterface::RestoreManagedPurchases()
        {
        	JNIEnv* env = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	env->CallVoidMethod(GetJavaObject(), GetMethodID("RestoreManagedPurchases"));
        }
	}
}
