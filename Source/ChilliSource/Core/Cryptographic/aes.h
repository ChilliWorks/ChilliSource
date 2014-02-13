/* The MIT License

   Copyright (C) 2011 Zilong Tan (tzlloch@gmail.com)

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

/*
 * aes.h
 *
 * @version 3.0 (December 2000)
 *
 * Optimised ANSI C code for the Rijndael cipher (now AES)
 *
 * @author Vincent Rijmen <vincent.rijmen@esat.kuleuven.ac.be>
 * @author Antoon Bosselaers <antoon.bosselaers@esat.kuleuven.ac.be>
 * @author Paulo Barreto <paulo.barreto@terra.com.br>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ULIB_AES_H
#define __ULIB_AES_H

#include <stdint.h>

#define AES_MAXNR 14
#define AES_BLOCK_SIZE 16

struct aes_key_st {
	uint32_t rd_key[4 *(AES_MAXNR + 1)];
	int rounds;
};

typedef struct aes_key_st AES_KEY;

#ifdef __cplusplus
extern "C" {
#endif

	/* bits can be 128, 192 or 256 */
	int  AES_set_encrypt_key(const unsigned char *userKey, const int bits,
				AES_KEY *key);
	int  AES_set_decrypt_key(const unsigned char *userKey, const int bits,
				AES_KEY *key);

	void AES_encrypt(const unsigned char *in, unsigned char *out,
			 const AES_KEY *key);
	void AES_decrypt(const unsigned char *in, unsigned char *out,
			 const AES_KEY *key);

	void AES_cbc_encrypt(const unsigned char *in, unsigned char *out, unsigned char *ivec,
			     unsigned long nblock, const AES_KEY *key);

	void AES_cbc_decrypt(const unsigned char *in, unsigned char *out, unsigned char *ivec,
			     unsigned long nblock, const AES_KEY *key);


#ifdef __cplusplus
}
#endif

#endif
