//
//  HMAC_SHA1.h
//  moFlow
//
//  Modified by Robert Henning on 01/03/2012.
//  Copyright 2012 Tag Games. All rights reserved.

/*
	100% free public domain implementation of the HMAC-SHA1 algorithm
	by Chien-Chung, Chung (Jim Chung) <jimchung1221@gmail.com>
*/


#ifndef __MO_FLO_CORE_CRYPTOGRAPHIC_HMAC_SHA1_H__
#define __MO_FLO_CORE_CRYPTOGRAPHIC_HMAC_SHA1_H__

#include <ChilliSource/Core/Cryptographic/SHA1.h>

//typedef unsigned char BYTE ;

namespace moFlo
{
	namespace Core
	{
		class CHMAC_SHA1 : public moFlo::Core::CSHA1
		{
			private:
				s8 m_ipad[64];
				s8 m_opad[64];

				char * szReport ;
				char * SHA1_Key ;
				char * AppendBuf1 ;
				char * AppendBuf2 ;


			public:

				enum {
					SHA1_DIGEST_LENGTH	= 20,
					SHA1_BLOCK_SIZE		= 64,
					HMAC_BUF_LEN		= 4096
				} ;

				CHMAC_SHA1()
					:szReport(new char[HMAC_BUF_LEN]),
					 AppendBuf1(new char[HMAC_BUF_LEN]),
					 AppendBuf2(new char[HMAC_BUF_LEN]),
					 SHA1_Key(new char[HMAC_BUF_LEN])
				{}

				~CHMAC_SHA1()
				{
					delete[] szReport ;
					delete[] AppendBuf1 ;
					delete[] AppendBuf2 ;
					delete[] SHA1_Key ;
				}

				void HMAC_SHA1(u8 *text, int text_len, u8 *key, int key_len, u8 *digest);
		};
	}
}

#endif // __MO_FLO_CORE_CRYPTOGRAPHIC_HMAC_SHA1_H__
