//
//  HMAC_SHA1.cpp
//  moFlow
//
//  Modified by Robert Henning on 01/03/2012.
//  Copyright 2012 Tag Games. All rights reserved.
//

//******************************************************************************
//* HMAC_SHA1.cpp : Implementation of HMAC SHA1 algorithm
//*                 Comfort to RFC 2104
//*
//******************************************************************************
#include <ChilliSource/Core/Cryptographic/HMAC_SHA1.h>
#include <iostream>
#include <memory>

namespace ChilliSource
{
	namespace Core
	{
		void CHMAC_SHA1::HMAC_SHA1(u8 *text, int text_len, u8 *key, int key_len, u8 *digest)
		{
			memset(SHA1_Key, 0, SHA1_BLOCK_SIZE);

			/* repeated 64 times for values in ipad and opad */
			memset(m_ipad, 0x36, sizeof(m_ipad));
			memset(m_opad, 0x5c, sizeof(m_opad));

			/* STEP 1 */
			if (key_len > SHA1_BLOCK_SIZE)
			{
				CSHA1::Reset();
				CSHA1::Update((u8 *)key, key_len);
				CSHA1::Final();

				CSHA1::GetHash((u8 *)SHA1_Key);
			}
			else
				memcpy(SHA1_Key, key, key_len);

			/* STEP 2 */
			for (int i=0; i<sizeof(m_ipad); i++)
			{
				m_ipad[i] ^= SHA1_Key[i];
			}

			/* STEP 3 */
			memcpy(AppendBuf1, m_ipad, sizeof(m_ipad));
			memcpy(AppendBuf1 + sizeof(m_ipad), text, text_len);

			/* STEP 4 */
			CSHA1::Reset();
			CSHA1::Update((u8 *)AppendBuf1, sizeof(m_ipad) + text_len);
			CSHA1::Final();

			CSHA1::GetHash((u8 *)szReport);

			/* STEP 5 */
			for (int j=0; j<sizeof(m_opad); j++)
			{
				m_opad[j] ^= SHA1_Key[j];
			}

			/* STEP 6 */
			memcpy(AppendBuf2, m_opad, sizeof(m_opad));
			memcpy(AppendBuf2 + sizeof(m_opad), szReport, SHA1_DIGEST_LENGTH);

			/*STEP 7 */
			CSHA1::Reset();
			CSHA1::Update((u8 *)AppendBuf2, sizeof(m_opad) + SHA1_DIGEST_LENGTH);
			CSHA1::Final();

			CSHA1::GetHash((u8 *)digest);
		}
	}
}
