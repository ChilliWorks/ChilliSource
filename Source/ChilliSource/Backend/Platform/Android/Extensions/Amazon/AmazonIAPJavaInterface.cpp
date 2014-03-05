//
//  AmazonIAPJavaInterface.cpp
//  MoFlow
//
//  Created by Ian Copland on 10/12/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Extensions/Amazon/AmazonIAPJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/JSON/json.h>
#include <jni.h>

extern "C"
{
	void Java_com_chillisource_amazon_AmazonIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* inpEnv, jobject inThis, jobjectArray inaIDs, jobjectArray inaNames, jobjectArray inaDescs, jobjectArray inaPrices);
	void Java_com_chillisource_amazon_AmazonIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* inpEnv, jobject inThis, jint inudwResult, jstring instrProductID, jstring instrTransactionID, jstring instrReceipt);
	void Java_com_chillisource_amazon_AmazonIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* inpEnv, jobject inThis, jstring instrProductID, jstring instrTransactionID);
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
void Java_com_chillisource_amazon_AmazonIAPNativeInterface_NativeOnProductsDescriptionsRequestComplete(JNIEnv* inpEnv, jobject inThis, jobjectArray inaIDs, jobjectArray inaNames, jobjectArray inaDescs, jobjectArray inaPrices)
{
	ChilliSource::Android::AmazonIAPJavaInterfacePtr pInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::AmazonIAPJavaInterface>();
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

		CSCore::TaskScheduler::ScheduleMainThreadTask(CSCore::Task<const std::vector<CSNetworking::IAPProductDesc>&>(pInterface.get(), &ChilliSource::Android::AmazonIAPJavaInterface::OnProductDescriptionsRequestComplete, aProducts));
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
void Java_com_chillisource_amazon_AmazonIAPNativeInterface_NativeOnTransactionStatusUpdated(JNIEnv* inpEnv, jobject inThis, jint inudwResult, jstring instrProductID, jstring instrTransactionID, jstring instrReceipt)
{
	ChilliSource::Android::AmazonIAPJavaInterfacePtr pInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::AmazonIAPJavaInterface>();
	if (pInterface != NULL)
	{
		CSNetworking::IAPTransaction sTransaction;
		sTransaction.strProductID = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrProductID);
		sTransaction.strTransactionID = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrTransactionID);
		sTransaction.strReceipt = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrReceipt);
		CSCore::TaskScheduler::ScheduleMainThreadTask(CSCore::Task<u32, const CSNetworking::IAPTransaction&>(pInterface.get(), &ChilliSource::Android::AmazonIAPJavaInterface::OnTransactionStatusUpdated, inudwResult, sTransaction));
	}
}
//--------------------------------------------------------------------------------------
/// On Transaction Closed
///
/// @param JNI pEnvironment
/// @param Pointer to the calling function
//--------------------------------------------------------------------------------------
void Java_com_chillisource_amazon_AmazonIAPNativeInterface_NativeOnTransactionClosed(JNIEnv* inpEnv, jobject inThis, jstring instrProductID, jstring instrTransactionID)
{
	ChilliSource::Android::AmazonIAPJavaInterfacePtr pInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::AmazonIAPJavaInterface>();
	if (pInterface != NULL)
	{
		std::string strProductID = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrProductID);
		std::string strTransactionID = ChilliSource::Android::JavaInterfaceUtils::CreateSTDStringFromJString(instrTransactionID);
		CSCore::TaskScheduler::ScheduleMainThreadTask(CSCore::Task<const std::string&, const std::string&>(pInterface.get(), &ChilliSource::Android::AmazonIAPJavaInterface::OnTransactionClosed, strProductID, strTransactionID));
	}
}

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(AmazonIAPJavaInterface);
		//--------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------
		AmazonIAPJavaInterface::AmazonIAPJavaInterface(const std::string& instrPrivateKey, const std::string& instrUDID)
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
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrPrivateKey = JavaInterfaceUtils::CreateJStringFromSTDString(instrPrivateKey);
			jstring jstrUDID = JavaInterfaceUtils::CreateJStringFromSTDString(instrUDID);
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Init"), jstrPrivateKey, jstrUDID);
			pEnv->DeleteLocalRef(jstrPrivateKey);
			pEnv->DeleteLocalRef(jstrUDID);
		}
		//--------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------
		bool AmazonIAPJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == AmazonIAPJavaInterface::InterfaceID);
		}
        //---------------------------------------------------------------
		/// Is Purchasing Enabled
        //---------------------------------------------------------------
        bool AmazonIAPJavaInterface::IsPurchasingEnabled()
        {
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallBooleanMethod(GetJavaObject(), GetMethodID("IsPurchasingEnabled"));
        }
        //---------------------------------------------------------------
		/// Start Listening For Transaction Updates
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::StartListeningForTransactionUpdates(const Networking::IAPTransactionDelegate& inRequestDelegate)
        {
        	mTransactionStatusDelegate = inRequestDelegate;

        	JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("StartListeningForTransactionUpdates"));
        }
        //---------------------------------------------------------------
		/// Stop Listening For Transaction Updates
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::StopListeningForTransactionUpdates()
        {
        	mTransactionStatusDelegate = NULL;

        	JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("StopListeningForTransactionUpdates"));
        }
        //---------------------------------------------------------------
		/// Request Product Descriptions
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::RequestProductDescriptions(const std::vector<std::string>& inaProductIDs, const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
        	mProductsRequestDelegate = inRequestDelegate;
        	if(mProductsRequestDelegate != NULL)
        	{
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

				jobjectArray jaProductIDs = pEnv->NewObjectArray(inaProductIDs.size(), pEnv->FindClass("java/lang/String"), pEnv->NewStringUTF(""));
				u32 udwProductIDIndex = 0;
				for(std::vector<std::string>::const_iterator it = inaProductIDs.begin(); it != inaProductIDs.end(); ++it)
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
        void AmazonIAPJavaInterface::OnProductDescriptionsRequestComplete(const std::vector<Networking::IAPProductDesc>& inaProducts)
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
        void AmazonIAPJavaInterface::CancelProductDescriptionsRequest()
        {
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("CancelProductDescriptionsRequest"));
        }
        //---------------------------------------------------------------
		/// Request Product Purchase
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::RequestProductPurchase(const std::string& instrProductID)
        {
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrProductID = JavaInterfaceUtils::CreateJStringFromSTDString(instrProductID);
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("RequestProductPurchase"), jstrProductID);
			pEnv->DeleteLocalRef(jstrProductID);
        }
        //---------------------------------------------------------------
		/// On Transaction Status Updated
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::OnTransactionStatusUpdated(u32 inudwStatus, const Networking::IAPTransaction& inTransaction)
        {
        	if(mTransactionStatusDelegate != NULL)
        	{
				Networking::IAPTransaction::Status eStatus = Networking::IAPTransaction::Status::k_failed;

				//This requires a little bit of faith. These numbers correspond to the constants defined
				//in the java class CPurchaseTransaction
				const u32 kudwPurchaseSucceeded = 0;
				const u32 kudwPurchaseResumed = 1;
				const u32 kudwPurchaseRestored = 2;
				const u32 kudwPurchaseFailed = 3;

				switch(inudwStatus)
				{
					case kudwPurchaseSucceeded:
						eStatus = Networking::IAPTransaction::Status::k_succeeded;
						break;
					case kudwPurchaseResumed:
						eStatus = Networking::IAPTransaction::Status::k_resumed;
						break;
					case kudwPurchaseRestored:
						eStatus = Networking::IAPTransaction::Status::k_restored;
						break;
					case kudwPurchaseFailed:
					default:
						eStatus = Networking::IAPTransaction::Status::k_failed;
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
        void AmazonIAPJavaInterface::CloseTransaction(const std::string& instrProductID, const std::string& instrTransactionID, const Networking::IAPTransactionCloseDelegate& inDelegate)
        {
        	mTransactionCloseDelegate = inDelegate;

			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrProductID = JavaInterfaceUtils::CreateJStringFromSTDString(instrProductID);
			jstring jstrTransactionID = JavaInterfaceUtils::CreateJStringFromSTDString(instrTransactionID);
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("CloseTransaction"), jstrProductID, jstrTransactionID);
			pEnv->DeleteLocalRef(jstrProductID);
			pEnv->DeleteLocalRef(jstrTransactionID);
        }
        //---------------------------------------------------------------
		/// On Transaction Closed
        //---------------------------------------------------------------
        void AmazonIAPJavaInterface::OnTransactionClosed(const std::string& instrProductID, const std::string& instrTransactionID)
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
        void AmazonIAPJavaInterface::RestoreManagedPurchases()
        {
        	JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
        	pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("RestoreManagedPurchases"));
        }
	}
}
