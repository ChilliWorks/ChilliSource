//
//  GooglePlayIAPJavaInterface.cpp
//  MoFlow
//
//  Created by Scott Downie on 14/06/2013.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Platform/Android/GooglePlay/GooglePlayIAPJavaInterface.h>

#include <jni.h>
#include <ChilliSource/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Platform/Android/JavaInterface/JavaInterfaceUtils.h>

#include <ChilliSource/Core/Utils.h>
#include <ChilliSource/Core/JSON/json.h>
#include <ChilliSource/Core/TaskScheduler.h>

const std::string kstrPendingCacheFile = "GoogleIAPPendingTransactions.json";

extern "C"
{
	void Java_com_taggames_moflow_googleplay_iap_CGooglePlayIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* inpEnv, jobject inThis, jobjectArray inaIDs, jobjectArray inaNames, jobjectArray inaDescs, jobjectArray inaPrices);
	void Java_com_taggames_moflow_googleplay_iap_CGooglePlayIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* inpEnv, jobject inThis, jint inudwResult, jstring instrProductID, jstring instrTransactionID, jstring instrReceipt);
	void Java_com_taggames_moflow_googleplay_iap_CGooglePlayIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* inpEnv, jobject inThis, jstring instrProductID, jstring instrTransactionID);

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
void Java_com_taggames_moflow_googleplay_iap_CGooglePlayIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* inpEnv, jobject inThis, jobjectArray inaIDs, jobjectArray inaNames, jobjectArray inaDescs, jobjectArray inaPrices)
{
	moFlo::AndroidPlatform::GooglePlayIAPJavaInterfacePtr pInterface = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<moFlo::AndroidPlatform::CGooglePlayIAPJavaInterface>();
	if (pInterface != NULL)
	{
		u32 udwNumProducts = inpEnv->GetArrayLength(inaIDs);
		DYNAMIC_ARRAY<moNetworking::IAPProductDesc> aProducts;
		aProducts.reserve(udwNumProducts);

		for(u32 i=0; i<udwNumProducts; ++i)
		{
			moNetworking::IAPProductDesc sDesc;
			jstring jstrID = (jstring)inpEnv->GetObjectArrayElement(inaIDs, i);
			sDesc.strID = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(jstrID);

			jstring jstrName = (jstring)inpEnv->GetObjectArrayElement(inaNames, i);
			sDesc.strName = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(jstrName);

			jstring jstrDesc = (jstring)inpEnv->GetObjectArrayElement(inaDescs, i);
			sDesc.strDescription = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(jstrDesc);

			jstring jstrPrice = (jstring)inpEnv->GetObjectArrayElement(inaPrices, i);
			sDesc.strFormattedPrice = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(jstrPrice);

			aProducts.push_back(sDesc);
		}

		moFlo::CTaskScheduler::ScheduleMainThreadTask(moFlo::Task1<const DYNAMIC_ARRAY<moNetworking::IAPProductDesc>&>(pInterface.get(), &moFlo::AndroidPlatform::CGooglePlayIAPJavaInterface::OnProductDescriptionsRequestComplete, aProducts));
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
void Java_com_taggames_moflow_googleplay_iap_CGooglePlayIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* inpEnv, jobject inThis, jint inudwResult, jstring instrProductID, jstring instrTransactionID, jstring instrReceipt)
{
	moFlo::AndroidPlatform::GooglePlayIAPJavaInterfacePtr pInterface = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<moFlo::AndroidPlatform::CGooglePlayIAPJavaInterface>();
	if (pInterface != NULL)
	{
		moNetworking::IAPTransaction sTransaction;
		sTransaction.strProductID = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(instrProductID);
		sTransaction.strTransactionID = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(instrTransactionID);
		sTransaction.strReceipt = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(instrReceipt);

		moFlo::CTaskScheduler::ScheduleMainThreadTask(moFlo::Task2<u32, const moNetworking::IAPTransaction&>(pInterface.get(), &moFlo::AndroidPlatform::CGooglePlayIAPJavaInterface::OnTransactionStatusUpdated, inudwResult, sTransaction));
	}
}
//--------------------------------------------------------------------------------------
/// On Transaction Closed
///
/// @param JNI environment
/// @param Pointer to the calling function
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_googleplay_iap_CGooglePlayIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* inpEnv, jobject inThis, jstring instrProductID, jstring instrTransactionID)
{
	moFlo::AndroidPlatform::GooglePlayIAPJavaInterfacePtr pInterface = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<moFlo::AndroidPlatform::CGooglePlayIAPJavaInterface>();
	if (pInterface != NULL)
	{
		const std::string strProductID = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(instrProductID);
		const std::string strTransactionID = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(instrTransactionID);
		moFlo::CTaskScheduler::ScheduleMainThreadTask(moFlo::Task2<std::string, std::string>(pInterface.get(), &moFlo::AndroidPlatform::CGooglePlayIAPJavaInterface::OnTransactionClosed, strProductID, strTransactionID));
	}
}

namespace moFlo
{
	namespace AndroidPlatform
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
				Core::CUtils::ReadJson(Core::SL_CACHE, kstrPendingCacheFile, &jData);
				jData[instrTransactionID] = 0;
				Core::CUtils::StringToFile(Core::SL_CACHE, kstrPendingCacheFile, jData.toUnformattedString());
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
				if(Core::CUtils::ReadJson(Core::SL_CACHE, kstrPendingCacheFile, &jData))
				{
					jData.removeMember(instrTransactionID);
					Core::CUtils::StringToFile(Core::SL_CACHE, kstrPendingCacheFile, jData.toUnformattedString());
				}
			}
		}

		DEFINE_NAMED_INTERFACE(CGooglePlayIAPJavaInterface);

		CGooglePlayIAPJavaInterface::CGooglePlayIAPJavaInterface(const std::string& instrPublicKey)
		{
			CreateNativeInterface("com/taggames/moflow/googleplay/iap/CGooglePlayIAPNativeInterface");
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
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrPublicKey = JavaInterfaceUtils::CreateJStringFromSTDString(instrPublicKey);
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Init"), jstrPublicKey);
			pEnv->DeleteLocalRef(jstrPublicKey);
		}
		//--------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------
		bool CGooglePlayIAPJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CGooglePlayIAPJavaInterface::InterfaceID);
		}
        //---------------------------------------------------------------
		/// Is Purchasing Enabled
        //---------------------------------------------------------------
        bool CGooglePlayIAPJavaInterface::IsPurchasingEnabled()
        {
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallBooleanMethod(GetJavaObject(), GetMethodID("IsPurchasingEnabled"));
        }
        //---------------------------------------------------------------
		/// Start Listening For Transaction Updates
        //---------------------------------------------------------------
        void CGooglePlayIAPJavaInterface::StartListeningForTransactionUpdates(const Networking::IAPTransactionDelegate& inRequestDelegate)
        {
        	mTransactionStatusDelegate = inRequestDelegate;

        	JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	env->CallVoidMethod(GetJavaObject(), GetMethodID("RestorePendingUnmanagedTransactions"));

        	Json::Value jData;
        	if(Core::CUtils::ReadJson(Core::SL_CACHE, kstrPendingCacheFile, &jData))
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
        void CGooglePlayIAPJavaInterface::StopListeningForTransactionUpdates()
        {
        	mTransactionStatusDelegate = NULL;
        }
        //---------------------------------------------------------------
		/// Request Product Descriptions
        //---------------------------------------------------------------
        void CGooglePlayIAPJavaInterface::RequestProductDescriptions(const DYNAMIC_ARRAY<std::string>& inaProductIDs, const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
        	mProductsRequestDelegate = inRequestDelegate;

        	if(mProductsRequestDelegate == NULL)
        		return;

        	JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

			jobjectArray jaProductIDs = env->NewObjectArray(inaProductIDs.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));

			u32 udwProductIDIndex = 0;
			for(DYNAMIC_ARRAY<std::string>::const_iterator it = inaProductIDs.begin(); it != inaProductIDs.end(); ++it)
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
        void CGooglePlayIAPJavaInterface::OnProductDescriptionsRequestComplete(const DYNAMIC_ARRAY<moNetworking::IAPProductDesc>& inaProducts)
        {
        	if(mProductsRequestDelegate == NULL)
        		return;

        	mProductsRequestDelegate(inaProducts);
        }
        //---------------------------------------------------------------
		/// Cancel Product Descriptions Request
        //---------------------------------------------------------------
        void CGooglePlayIAPJavaInterface::CancelProductDescriptionsRequest()
        {
			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("CancelProductDescriptionsRequest"));
        }
        //---------------------------------------------------------------
		/// Request Product Purchase
        //---------------------------------------------------------------
        void CGooglePlayIAPJavaInterface::RequestProductPurchase(const std::string& instrProductID, Networking::IAPProductRegInfo::Type ineType)
        {
        	const u32 kManagedType = 0;
        	const u32 kUnManagedType = 1;
        	u32 udwType = kUnManagedType;

        	switch(ineType)
        	{
        		case Networking::IAPProductRegInfo::MANAGED:
        			udwType = kManagedType;
        			break;
        		case Networking::IAPProductRegInfo::UNMANAGED:
        			udwType = kUnManagedType;
        			break;
        	}

			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrProductID = JavaInterfaceUtils::CreateJStringFromSTDString(instrProductID);
			env->CallVoidMethod(GetJavaObject(), GetMethodID("RequestProductPurchase"), jstrProductID, udwType);
			env->DeleteLocalRef(jstrProductID);
        }
        //---------------------------------------------------------------
		/// On Transaction Status Updated
        //---------------------------------------------------------------
        void CGooglePlayIAPJavaInterface::OnTransactionStatusUpdated(u32 inudwStatus, const moNetworking::IAPTransaction& inTransaction)
        {
        	if(mTransactionStatusDelegate == NULL)
        		return;

        	Networking::IAPTransaction::Status eStatus = Networking::IAPTransaction::FAILED;

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
        			eStatus = Networking::IAPTransaction::SUCCEEDED;
        			AddToPendingCache(inTransaction.strTransactionID);
        			break;
        		case 1:
        			eStatus = Networking::IAPTransaction::FAILED;
        			break;
        		case 2:
        			eStatus = Networking::IAPTransaction::CANCELLED;
        			break;
        		case 3:
        			eStatus = Networking::IAPTransaction::RESTORED;
        			break;
        		case 4:
        			eStatus = Networking::IAPTransaction::RESUMED;
        			break;
           		case 5:
            		eStatus = Networking::IAPTransaction::REFUNDED;
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
        void CGooglePlayIAPJavaInterface::CloseTransaction(const std::string& instrProductID, const std::string& instrTransactionID, const Networking::IAPTransactionCloseDelegate& inDelegate)
        {
        	mTransactionCloseDelegate = inDelegate;

			JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
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
        void CGooglePlayIAPJavaInterface::OnTransactionClosed(std::string instrProductID, std::string instrTransactionID)
        {
        	if(mTransactionCloseDelegate)
        	{
        		mTransactionCloseDelegate(instrProductID, instrTransactionID);
        	}
        }
        //---------------------------------------------------------------
		/// Restore Managed Purchases
        //---------------------------------------------------------------
        void CGooglePlayIAPJavaInterface::RestoreManagedPurchases()
        {
        	JNIEnv* env = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	env->CallVoidMethod(GetJavaObject(), GetMethodID("RestoreManagedPurchases"));
        }
	}
}
