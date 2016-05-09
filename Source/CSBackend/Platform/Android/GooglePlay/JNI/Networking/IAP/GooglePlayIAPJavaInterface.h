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

#ifdef CS_ANDROIDFLAVOUR_GOOGLEPLAY

#ifndef _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_NETWORKING_GOOGLEPLAYIAPJAVAINTERFACE_H_
#define _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_NETWORKING_GOOGLEPLAYIAPJAVAINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Networking/IAP/IAPSystem.h>

#include <CSBackend/Platform/Android/GooglePlay/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterface.h>


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

            //------------------------------------------------------
            /// Delegate that is triggered when a request for product
            /// informations completes.
            ///
            /// @author T Kane
            ///
            /// @param in_productDesc - List of product descriptions
            /// @param in_currencyCodes - List of currency codes
            ///         corresponding with each entry of in_productDesc
            /// @param in_unformattedPrices - List of unformatted prices
            ///         corresponding with each entry of in_productDesc
            //------------------------------------------------------
            using OnProductDescriptionsRequestCompleteDelegate = std::function<void(const std::vector<ChilliSource::IAPSystem::ProductDesc>& in_productDesc, const std::vector<std::string>& in_currencyCodes, const std::vector<std::string>& in_unformattedPrices)>;

			//--------------------------------------------------------------
			/// Constructor
			///
			/// @author S Downie
			///
			/// @param in_publicKey - Google Play Store public key
			//--------------------------------------------------------------
			GooglePlayIAPJavaInterface();
			//--------------------------------------------------------------
			/// @author S Downie
			///
			/// @param in_interfaceId - Interface ID
			///
			/// @return whether or not this object implements the given interface.
			//--------------------------------------------------------------
			bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
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
            /// @param in_delegate - Delegate
            //---------------------------------------------------------------
            void StartListeningForTransactionUpdates(const ChilliSource::IAPSystem::TransactionStatusDelegate& in_delegate);
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
			/// @param in_productIds - List of product IDs to request descriptions for
            /// @param in_delegate - Delegate to invoke when the request completes
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            void RequestProductDescriptions(const std::vector<std::string>& in_productIds, const OnProductDescriptionsRequestCompleteDelegate& in_delegate);
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
            /// @param in_productId - Product ID
            /// @param in_type - Product type (managed, unmanaged)
            //---------------------------------------------------------------
            void RequestProductPurchase(const std::string& in_productId, ChilliSource::IAPSystem::ProductRegInfo::Type in_type);
            //---------------------------------------------------------------
			/// Tell the store to close the transaction as complete.
            /// NOTE: This should only be called after the goods have been
            /// awarded.
            ///
            /// @author S Downie
            ///
            /// @param in_productId - Product ID
            /// @param in_transactionId - Transaction ID
            /// @param in_delegate - Delegate to call when closed successfully
            //---------------------------------------------------------------
            void CloseTransaction(const std::string& in_productId, const std::string& in_transactionId, const ChilliSource::IAPSystem::TransactionCloseDelegate& in_delegate);
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
            /// @param in_products - List of product descriptions
            /// @param in_currencyCodes - List of currency codes
            /// @param in_unformattedPrices - List of unformatted prices
            //---------------------------------------------------------------
            void OnProductDescriptionsRequestComplete(const std::vector<ChilliSource::IAPSystem::ProductDesc>& in_products, const std::vector<std::string>& in_currencyCodes, const std::vector<std::string>& in_unformattedPrices);
            //---------------------------------------------------------------
    		/// Called when transaction status changes
            ///
            /// @author S Downie
            ///
            /// @param in_statusId - Status ID
            /// @param in_transaction - Transaction description
            //---------------------------------------------------------------
            void OnTransactionStatusUpdated(u32 in_statusId, const ChilliSource::IAPSystem::Transaction& in_transaction);
            //---------------------------------------------------------------
    		/// Delegate called when the transaction is closed either
            /// with success or failure
            ///
            /// @author S Downie
            ///
            /// @param in_productID - Product ID
            /// @param in_transactionId - Transaction ID
            /// @param in_success - Whether the closing of the transaction was successful
            //---------------------------------------------------------------
            void OnTransactionClosed(const std::string& in_productID, const std::string& in_transactionId, bool in_success);

		private:
            OnProductDescriptionsRequestCompleteDelegate m_productsRequestDelegate;
            ChilliSource::IAPSystem::TransactionStatusDelegate m_transactionStatusDelegate;
            ChilliSource::IAPSystem::TransactionCloseDelegate m_transactionCloseDelegate;
		};
	}
}

#endif

#endif

#endif
