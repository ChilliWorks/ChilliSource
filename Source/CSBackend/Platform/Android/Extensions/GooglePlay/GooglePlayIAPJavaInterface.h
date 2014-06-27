//
//  GooglePlayIAPJavaInterface.h
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

#ifdef CS_ANDROIDEXTENSION_GOOGLEPLAY

#ifndef _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_GOOGLEPLAYIAPJAVAINTERFACE_H_
#define _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_GOOGLEPLAYIAPJAVAINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>
#include <ChilliSource/Networking/IAP/IAPSystem.h>

namespace CSBackend
{
	namespace Android
	{
		//--------------------------------------------------------------
		/// Interface class that allows communication between the
		/// Google Play IAP system and the Java Google Play billing SDK
		///
		/// @author S Downie
		//--------------------------------------------------------------
		class GooglePlayIAPJavaInterface final : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(GooglePlayIAPJavaInterface);

			//--------------------------------------------------------------
			/// Constructor
			///
			/// @author S Downie
			///
			/// @param Google Play Store public key
			//--------------------------------------------------------------
			GooglePlayIAPJavaInterface(const std::string& in_publicKey);
			//--------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface ID
			///
			/// @return whether or not this object implements the given interface.
			//--------------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether the purchasing is allowed by the device/OS
            //---------------------------------------------------------------
            bool IsPurchasingEnabled();
            //---------------------------------------------------------------
			/// Calling this function will set the listener to which any
            /// transaction events are directed. This is not necessarily
            /// in response to a user action it may be previously outstanding
            /// transactions.
            ///
            /// @author S Downie
            ///
            /// @param Delegate
            //---------------------------------------------------------------
            void StartListeningForTransactionUpdates(const CSNetworking::IAPSystem::TransactionStatusDelegate& in_delegate);
            //---------------------------------------------------------------
            /// Prevent any more transaction uppdates from being triggered.
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            void StopListeningForTransactionUpdates();
            //---------------------------------------------------------------
            /// Starts a request to the store for details of the products.
            /// These details are name, description and price
            ///
			/// @param List of product IDs to request descriptions for
            /// @param Delegate to invoke when the request completes
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            void RequestProductDescriptions(const std::vector<std::string>& in_productIds, const CSNetworking::IAPSystem::ProductDescDelegate& in_delegate);
            //---------------------------------------------------------------
			/// Prevent the completion delegate being called for
            /// any pending product description requests and attempt to
            /// cancel the request to the store.
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            void CancelProductDescriptionsRequest();
            //---------------------------------------------------------------
			/// Make a request to the store to purchase the item.
            /// This will trigger a call to the transaction listener delegate
            ///
            /// @author S Downie
            ///
            /// @param Product ID
            /// @param Product type (managed, unmanaged)
            //---------------------------------------------------------------
            void RequestProductPurchase(const std::string& in_productId, CSNetworking::IAPSystem::ProductRegInfo::Type in_type);
            //---------------------------------------------------------------
			/// Tell the store to close the transaction as complete.
            /// NOTE: This should only be called after the goods have been
            /// awarded.
            ///
            /// @author S Downie
            ///
            /// @param Product ID
            /// @param Transaction ID
            /// @param Delegate to call when closed successfully
            //---------------------------------------------------------------
            void CloseTransaction(const std::string& in_productId, const std::string& in_transactionId, const CSNetworking::IAPSystem::TransactionCloseDelegate& in_delegate);
            //---------------------------------------------------------------
            /// Request that the store trigger new purchase requests for
            /// owned non-consumable items
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            void RestoreManagedPurchases();

            //---Only called by JNI - Otherwise would be private
            //---------------------------------------------------------------
    		/// Called when product descriptions request complete
            ///
            /// @author S Downie
            ///
            /// @param List of product descriptions
            //---------------------------------------------------------------
            void OnProductDescriptionsRequestComplete(const std::vector<CSNetworking::IAPSystem::ProductDesc>& in_products);
            //---------------------------------------------------------------
    		/// Called when transaction status changes
            ///
            /// @author S Downie
            ///
            /// @param Status ID
            /// @param Transaction description
            //---------------------------------------------------------------
            void OnTransactionStatusUpdated(u32 in_statusId, const CSNetworking::IAPSystem::Transaction& in_transaction);
            //---------------------------------------------------------------
    		/// Delegate called when the transaction is closed either
            /// with success or failure
            ///
            /// @author S Downie
            ///
            /// @param Product ID
            /// @param Transaction ID
            /// @param Whether the closing of the transaction was successful
            //---------------------------------------------------------------
            void OnTransactionClosed(const std::string& in_productID, const std::string& in_transactionId, bool in_success);

		private:

            CSNetworking::IAPSystem::ProductDescDelegate m_productsRequestDelegate;
            CSNetworking::IAPSystem::TransactionStatusDelegate m_transactionStatusDelegate;
            CSNetworking::IAPSystem::TransactionCloseDelegate m_transactionCloseDelegate;
		};
	}
}

#endif

#endif

#endif
