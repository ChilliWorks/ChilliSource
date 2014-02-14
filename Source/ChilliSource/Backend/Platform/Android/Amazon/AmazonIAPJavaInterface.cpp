//
//  AmazonIAPJavaInterface.cpp
//  MoFlow
//
//  Created by Ian Copland on 10/12/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Amazon/AmazonIAPJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Main/Utils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/JSON/json.h>
#include <jni.h>

extern "C"
{
	void Java_com_taggames_moflow_amazon_iap_CAmazonIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* inpEnv, jobject inThis, jobjectArray inaIDs, jobjectArray inaNames, jobjectArray inaDescs, jobjectArray inaPrices);
	void Java_com_taggames_moflow_amazon_iap_CAmazonIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* inpEnv, jobject inThis, jint inudwResult, jstring instrProductID, jstring instrTransactionID, jstring instrReceipt);
	void Java_com_taggames_moflow_amazon_iap_CAmazonIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* inpEnv, jobject inThis, jstring instrProductID, jstring instrTransactionID);
}

//--------------------------------------------------------------------------------------
/// On Products Descriptions Request Complete
///
/// @param JNI pEnvironment
/// @param Pointer to the calling object
/// @param Array of product IDs
/// @param Array of product Names
/// @param Array of product Descriptions
/// @param Array of product Prices
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_amazon_iap_CAmazonIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* inpEnv, jobject inThis, jobjectArray inaIDs, jobjectArray inaNames, jobjectArray inaDescs, jobjectArray inaPrices)
{
	moFlo::AndroidPlatform::AmazonIAPJavaInterfacePtr pInterface = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<moFlo::AndroidPlatform::CAmazonIAPJavaInterface>();
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

		moFlo::CTaskScheduler::ScheduleMainThreadTask(moFlo::Task1<const DYNAMIC_ARRAY<moNetworking::IAPProductDesc>&>(pInterface.get(), &moFlo::AndroidPlatform::CAmazonIAPJavaInterface::OnProductDescriptionsRequestComplete, aProducts));
	}
}
//--------------------------------------------------------------------------------------
/// On Transaction Status Update
///
/// @param JNI pEnvironment
/// @param Pointer to the calling object
/// @param Result
/// @param Product ID
/// @param Transaction ID
/// @param Receipt
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_amazon_iap_CAmazonIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* inpEnv, jobject inThis, jint inudwResult, jstring instrProductID, jstring instrTransactionID, jstring instrReceipt)
{
	moFlo::AndroidPlatform::AmazonIAPJavaInterfacePtr pInterface = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<moFlo::AndroidPlatform::CAmazonIAPJavaInterface>();
	if (pInterface != NULL)
	{
		moNetworking::IAPTransaction sTransaction;
		sTransaction.strProductID = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(instrProductID);
		sTransaction.strTransactionID = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(instrTransactionID);
		sTransaction.strReceipt = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(instrReceipt);
		moFlo::CTaskScheduler::ScheduleMainThreadTask(moFlo::Task2<u32, const moNetworking::IAPTransaction&>(pInterface.get(), &moFlo::AndroidPlatform::CAmazonIAPJavaInterface::OnTransactionStatusUpdated, inudwResult, sTransaction));
	}
}
//--------------------------------------------------------------------------------------
/// On Transaction Closed
///
/// @param JNI pEnvironment
/// @param Pointer to the calling function
//--------------------------------------------------------------------------------------
void Java_com_taggames_moflow_amazon_iap_CAmazonIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* inpEnv, jobject inThis, jstring instrProductID, jstring instrTransactionID)
{
	moFlo::AndroidPlatform::AmazonIAPJavaInterfacePtr pInterface = moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<moFlo::AndroidPlatform::CAmazonIAPJavaInterface>();
	if (pInterface != NULL)
	{
		std::string strProductID = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(instrProductID);
		std::string strTransactionID = moFlo::AndroidPlatform::JavaInterfaceUtils::CreateSTDStringFromJString(instrTransactionID);
		moFlo::CTaskScheduler::ScheduleMainThreadTask(moFlo::Task2<const std::string&, const std::string&>(pInterface.get(), &moFlo::AndroidPlatform::CAmazonIAPJavaInterface::OnTransactionClosed, strProductID, strTransactionID));
	}
}

namespace moFlo
{
	namespace AndroidPlatform
	{
		DEFINE_NAMED_INTERFACE(CAmazonIAPJavaInterface);
		//--------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------
		CAmazonIAPJavaInterface::CAmazonIAPJavaInterface(const std::string& instrPrivateKey, const std::string& instrUDID)
		{
			CreateNativeInterface("com/taggames/moflow/amazon/iap/CAmazonIAPNativeInterface");
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
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrPrivateKey = JavaInterfaceUtils::CreateJStringFromSTDString(instrPrivateKey);
			jstring jstrUDID = JavaInterfaceUtils::CreateJStringFromSTDString(instrUDID);
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Init"), jstrPrivateKey, jstrUDID);
			pEnv->DeleteLocalRef(jstrPrivateKey);
			pEnv->DeleteLocalRef(jstrUDID);
		}
		//--------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------
		bool CAmazonIAPJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CAmazonIAPJavaInterface::InterfaceID);
		}
        //---------------------------------------------------------------
		/// Is Purchasing Enabled
        //---------------------------------------------------------------
        bool CAmazonIAPJavaInterface::IsPurchasingEnabled()
        {
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallBooleanMethod(GetJavaObject(), GetMethodID("IsPurchasingEnabled"));
        }
        //---------------------------------------------------------------
		/// Start Listening For Transaction Updates
        //---------------------------------------------------------------
        void CAmazonIAPJavaInterface::StartListeningForTransactionUpdates(const Networking::IAPTransactionDelegate& inRequestDelegate)
        {
        	mTransactionStatusDelegate = inRequestDelegate;

        	JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("StartListeningForTransactionUpdates"));
        }
        //---------------------------------------------------------------
		/// Stop Listening For Transaction Updates
        //---------------------------------------------------------------
        void CAmazonIAPJavaInterface::StopListeningForTransactionUpdates()
        {
        	mTransactionStatusDelegate = NULL;

        	JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("StopListeningForTransactionUpdates"));
        }
        //---------------------------------------------------------------
		/// Request Product Descriptions
        //---------------------------------------------------------------
        void CAmazonIAPJavaInterface::RequestProductDescriptions(const DYNAMIC_ARRAY<std::string>& inaProductIDs, const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
        	mProductsRequestDelegate = inRequestDelegate;
        	if(mProductsRequestDelegate != NULL)
        	{
				JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

				jobjectArray jaProductIDs = pEnv->NewObjectArray(inaProductIDs.size(), pEnv->FindClass("java/lang/String"), pEnv->NewStringUTF(""));
				u32 udwProductIDIndex = 0;
				for(DYNAMIC_ARRAY<std::string>::const_iterator it = inaProductIDs.begin(); it != inaProductIDs.end(); ++it)
				{
					jstring jstrProductID = JavaInterfaceUtils::CreateJStringFromSTDString(*it);
					pEnv->SetObjectArrayElement(jaProductIDs, udwProductIDIndex, jstrProductID);
					pEnv->DeleteLocalRef(jstrProductID);
					udwProductIDIndex++;
				}

				pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("RequestProductDescriptions"), jaProductIDs);
				pEnv->DeleteLocalRef(jaProductIDs);
        	}
        }
        //---------------------------------------------------------------
		/// On Product Descriptions Request Complete
        //---------------------------------------------------------------
        void CAmazonIAPJavaInterface::OnProductDescriptionsRequestComplete(const DYNAMIC_ARRAY<moNetworking::IAPProductDesc>& inaProducts)
        {
        	if(mProductsRequestDelegate != NULL)
        	{
        		mProductsRequestDelegate(inaProducts);
        		mProductsRequestDelegate = NULL;
        	}
        }
        //---------------------------------------------------------------
		/// Cancel Product Descriptions Request
        //---------------------------------------------------------------
        void CAmazonIAPJavaInterface::CancelProductDescriptionsRequest()
        {
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("CancelProductDescriptionsRequest"));
        }
        //---------------------------------------------------------------
		/// Request Product Purchase
        //---------------------------------------------------------------
        void CAmazonIAPJavaInterface::RequestProductPurchase(const std::string& instrProductID)
        {
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrProductID = JavaInterfaceUtils::CreateJStringFromSTDString(instrProductID);
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("RequestProductPurchase"), jstrProductID);
			pEnv->DeleteLocalRef(jstrProductID);
        }
        //---------------------------------------------------------------
		/// On Transaction Status Updated
        //---------------------------------------------------------------
        void CAmazonIAPJavaInterface::OnTransactionStatusUpdated(u32 inudwStatus, const moNetworking::IAPTransaction& inTransaction)
        {
        	if(mTransactionStatusDelegate != NULL)
        	{
				Networking::IAPTransaction::Status eStatus = Networking::IAPTransaction::FAILED;

				//This requires a little bit of faith. These numbers correspond to the constants defined
				//in the java class CPurchaseTransaction
				const u32 kudwPurchaseSucceeded = 0;
				const u32 kudwPurchaseResumed = 1;
				const u32 kudwPurchaseRestored = 2;
				const u32 kudwPurchaseFailed = 3;

				switch(inudwStatus)
				{
					case kudwPurchaseSucceeded:
						eStatus = Networking::IAPTransaction::SUCCEEDED;
						break;
					case kudwPurchaseResumed:
						eStatus = Networking::IAPTransaction::RESUMED;
						break;
					case kudwPurchaseRestored:
						eStatus = Networking::IAPTransaction::RESTORED;
						break;
					case kudwPurchaseFailed:
					default:
						eStatus = Networking::IAPTransaction::FAILED;
						break;
				}

				Networking::IAPTransactionPtr pTransaction(new Networking::IAPTransaction());
				pTransaction->strProductID = inTransaction.strProductID;
				pTransaction->strTransactionID = inTransaction.strTransactionID;
				pTransaction->strReceipt = inTransaction.strReceipt;

				mTransactionStatusDelegate(eStatus, pTransaction);
        	}
        }
        //---------------------------------------------------------------
		/// Close Transaction
        //---------------------------------------------------------------
        void CAmazonIAPJavaInterface::CloseTransaction(const std::string& instrProductID, const std::string& instrTransactionID, const Networking::IAPTransactionCloseDelegate& inDelegate)
        {
        	mTransactionCloseDelegate = inDelegate;

			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrProductID = JavaInterfaceUtils::CreateJStringFromSTDString(instrProductID);
			jstring jstrTransactionID = JavaInterfaceUtils::CreateJStringFromSTDString(instrTransactionID);
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("CloseTransaction"), jstrProductID, jstrTransactionID);
			pEnv->DeleteLocalRef(jstrProductID);
			pEnv->DeleteLocalRef(jstrTransactionID);
        }
        //---------------------------------------------------------------
		/// On Transaction Closed
        //---------------------------------------------------------------
        void CAmazonIAPJavaInterface::OnTransactionClosed(const std::string& instrProductID, const std::string& instrTransactionID)
        {
        	if(mTransactionCloseDelegate != NULL)
        	{
        		mTransactionCloseDelegate(instrProductID, instrTransactionID);
        		mTransactionCloseDelegate = NULL;
        	}
        }
        //---------------------------------------------------------------
		/// Restore Managed Purchases
        //---------------------------------------------------------------
        void CAmazonIAPJavaInterface::RestoreManagedPurchases()
        {
        	JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("RestoreManagedPurchases"));
        }
	}
}
