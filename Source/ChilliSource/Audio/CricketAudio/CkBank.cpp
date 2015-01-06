//
//  CkBank.cpp
//  Chilli Source
//  Created by Ian Copland on 30/12/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/Audio/CricketAudio/CkBank.h>

#include <ck/ck.h>
#include <ck/bank.h>

namespace ChilliSource
{
	namespace Audio
	{
		CS_DEFINE_NAMEDTYPE(CkBank);
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		CkBankUPtr CkBank::Create()
		{
			return CkBankUPtr(new CkBank());
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool CkBank::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (CkBank::InterfaceID == in_interfaceId);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkBank::Build(::CkBank* in_CkAudioBank)
		{
			CS_ASSERT(m_bank == nullptr, "Cannot call Build() on a CkBank more than once.");
			CS_ASSERT(in_CkAudioBank != nullptr, "Cannot Build() a CkBank with a null bank pointer.");

			m_bank = in_CkAudioBank;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		::CkBank* CkBank::GetBank() const
		{
			CS_ASSERT(m_bank != nullptr, "Bank has not yet been built!");

			return m_bank;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		CkBank::~CkBank()
		{
			if (m_bank != nullptr)
			{
				m_bank->destroy();
				m_bank = nullptr;
			}
		}
	}
}