#include"stdafx.h"
#include <cstring>
#include "Encryption.h"

const Encryption::UINT32 Encryption::sha256Key[64] = 
{ 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

void Encryption::transform(const Encryption::BYTE *message, Encryption::UINT32 blockSize)
{
	int j;
	Encryption::UINT32 w[64];
	Encryption::UINT32 wv[8];
	Encryption::UINT32 t1, t2;
	const Encryption::BYTE *subBlock;

	for (size_t i = 0; i < blockSize; i++)
	{
		subBlock = message + (i << 6);

		for (j = 0; j < 16; j++)
			SHA2_PACK32(&subBlock[j << 2], &w[j]);

		for (j = 16; j < 64; j++)
			w[j] = SHA256_F4(w[j - 2]) + w[j - 7] + SHA256_F3(w[j - 15]) + w[j - 16];

		for (j = 0; j < 8; j++)
			wv[j] = h[j];
		
		for (j = 0; j < 64; j++)
		{
			t1 = wv[7] + SHA256_F2(wv[4]) + SHA2_CH(wv[4], wv[5], wv[6]) + sha256Key[j] + w[j];
			t2 = SHA256_F1(wv[0]) + SHA2_MAJ(wv[0], wv[1], wv[2]);
			wv[7] = wv[6];
			wv[6] = wv[5];
			wv[5] = wv[4];
			wv[4] = wv[3] + t1;
			wv[3] = wv[2];
			wv[2] = wv[1];
			wv[1] = wv[0];
			wv[0] = t1 + t2;
		}

		for (j = 0; j < 8; j++)
			h[j] += wv[j];
	}
}

void Encryption::init()
{
	h[0] = 0x6a09e667;
	h[1] = 0xbb67ae85;
	h[2] = 0x3c6ef372;
	h[3] = 0xa54ff53a;
	h[4] = 0x510e527f;
	h[5] = 0x9b05688c;
	h[6] = 0x1f83d9ab;
	h[7] = 0x5be0cd19;
	length = 0;
	totalLength = 0;
}

void Encryption::update(const Encryption::BYTE* message, Encryption::UINT32 len)
{
	Encryption::UINT32 blockNb;
	Encryption::UINT32 newLength, remLength, tmpLength;
	const Encryption::BYTE* shiftedMessage;

	tmpLength = SHA224_256BlockSize - length;
	remLength = len < tmpLength ? len : tmpLength;
	memcpy_s(&block[length], remLength, message, remLength);
	if (length + len < SHA224_256BlockSize) 
	{
		length += len;
		return;
	}
	newLength = len - remLength;
	blockNb = newLength / SHA224_256BlockSize;
	shiftedMessage = message + remLength;
	transform(block, 1);
	transform(shiftedMessage, blockNb);
	remLength = newLength % SHA224_256BlockSize;
	memcpy_s(block, remLength, &shiftedMessage[blockNb << 6], remLength);
	length = remLength;
	totalLength += (blockNb + 1) << 6;
}

void Encryption::final(Encryption::BYTE *digest)
{
	Encryption::UINT32 blockNb;
	Encryption::UINT32 pmLength;
	Encryption::UINT32 lenB;
	int i{};
	blockNb = (1 + ((SHA224_256BlockSize - 9) < (length % SHA224_256BlockSize)));
	lenB = (totalLength + length) << 3;
	pmLength = blockNb << 6;
	memset(block + length, 0, pmLength - length);
	block[length] = 0x80;
	SHA2_UNPACK32(lenB, block + pmLength - 4);
	transform(block, blockNb);
	for (i = 0; i < 8; i++) 
		SHA2_UNPACK32(h[i], &digest[i << 2]);
	
}

std::string Encryption::EncryptWithSHA256(std::string& input)
{
	CriticalSectionSync sync(cs);

	unsigned char digest[Encryption::digestSize];
	memset(digest, 0, Encryption::digestSize);

	Encryption ctx = Encryption();
	ctx.init();
	ctx.update(reinterpret_cast<Encryption::BYTE*>(const_cast<char*>(input.c_str())), static_cast<unsigned int>(input.length()));
	ctx.final(digest);

	char buf[2 * Encryption::digestSize + 1]{ 0, };
	buf[2 * Encryption::digestSize] = 0;
	for (int i = 0; i < Encryption::digestSize; i++)
		sprintf(buf + i * 2, "%02x", digest[i]);

	return std::string(buf);
}