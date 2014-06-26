//
//  EmailComposer.h
//  Chilli Source
//  Created by Stuart McGaw on 08/06/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_SOCIAL_COMMUNICATIONS_EMAILCOMPOSER_H_
#define _CHILLISOURCE_SOCIAL_COMMUNICATIONS_EMAILCOMPOSER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/System/StateSystem.h>

#include <functional>
#include <vector>

namespace ChilliSource 
{
	namespace Social 
	{
        //---------------------------------------------------------------
        /// A state system for presenting the user with an email
        /// composition view.
        ///
        /// @author S McGaw
        //---------------------------------------------------------------
        class EmailComposer : public Core::StateSystem
        {
		public:
			CS_DECLARE_NAMEDTYPE(EmailComposer);
			//-------------------------------------------------------
            /// An enum describing the result of the email intent.
            ///
            /// @author S McGaw
            //-------------------------------------------------------
			enum class SendResult
            {
                k_succeed,
                k_failed,
                k_cancelled
			};
            //-------------------------------------------------------
            /// Describes the format of the email contents.
            ///
            /// @author Ian Copland
            //-------------------------------------------------------
			enum class ContentFormat
            {
                k_plainText,
                k_html
			};
            //-------------------------------------------------------
            /// A container for information describing a single
            /// attachment file.
            ///
            /// @author Ian Copland
            //-------------------------------------------------------
            struct Attachment
            {
                Core::StorageLocation m_storageLocation;
                std::string m_filename;
                std::string m_mimeType;
            };
            //-------------------------------------------------------
            /// A delegate that is called when the email view is
            /// dismissed, returning the result.
            ///
            /// @author S McGaw
            ///
            /// @param The result of the email composition.
            //-------------------------------------------------------
			typedef std::function<void(SendResult)> SendResultDelegate;
			//-------------------------------------------------------
            /// Displays the email activity with the given recipients,
            /// subject and contents.
            ///
            /// @author S Downie
            ///
            /// @param A list of recipients (UTF8).
            /// @param The subject (UTF8).
            /// @param The body of the email (UTF8).
            /// @param The format of the body of the email.
            /// @param The callback describing the result of the email.
            //-------------------------------------------------------
			virtual void Present(const std::vector<std::string>& in_recipientAddresses, const std::string& in_subject, const std::string& in_contents, ContentFormat in_contentFormat,
                                 const SendResultDelegate& in_callback) = 0;
            //-------------------------------------------------------
            /// Displays the email activity with the given recipients,
            /// subject and contents, and adds a list of attachments
            /// to the email.
            ///
            /// @author Ian Copland
            ///
            /// @param A list of recipients (UTF8).
            /// @param The subject (UTF8).
            /// @param The body of the email (UTF8).
            /// @param The format of the body of the email.
            /// @param The attachment.
            /// @param The callback describing the result of the email.
            //-------------------------------------------------------
            virtual void PresentWithAttachment(const std::vector<std::string>& in_recipientAddresses, const std::string& in_subject, const std::string& in_contents, ContentFormat in_contentFormat,
                                               const Attachment& in_attachment, const SendResultDelegate& in_callback) = 0;
            //-------------------------------------------------------
			/// Dismisses the activity if it is currently displayed.
            ///
            /// @author S Downie
			//-------------------------------------------------------
			virtual void Dismiss() = 0;
			//-------------------------------------------------------
			/// Destructor
			//-------------------------------------------------------
			virtual ~EmailComposer() {};
        protected:
            friend class Core::State;
            
            //-------------------------------------------------------
            /// Creates a new instance of the email composition
            /// activity.
            ///
            /// @author Ian Copland
            ///
            /// @return the new activity instance.
            //-------------------------------------------------------
            static EmailComposerUPtr Create();
            //-------------------------------------------------------
			/// Protected constructor to force use of the factory
            /// method.
            ///
            /// @author Ian Copland.
			//-------------------------------------------------------
            EmailComposer() = default;
		};
		
	}
}

#endif
