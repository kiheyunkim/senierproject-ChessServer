#ifndef _ENCRYPTION_H_
#define _ENCRYPTION_H_
/*
this class 
*/
#include "CriticalSection.h"
#include <string>

#define SHA2_SHFR(x, n)    (x >> n)
#define SHA2_ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA2_ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define SHA2_CH(x, y, z)  ((x & y) ^ (~x & z))
#define SHA2_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA256_F1(x) (SHA2_ROTR(x,  2) ^ SHA2_ROTR(x, 13) ^ SHA2_ROTR(x, 22))
#define SHA256_F2(x) (SHA2_ROTR(x,  6) ^ SHA2_ROTR(x, 11) ^ SHA2_ROTR(x, 25))
#define SHA256_F3(x) (SHA2_ROTR(x,  7) ^ SHA2_ROTR(x, 18) ^ SHA2_SHFR(x,  3))
#define SHA256_F4(x) (SHA2_ROTR(x, 17) ^ SHA2_ROTR(x, 19) ^ SHA2_SHFR(x, 10))
#define SHA2_UNPACK32(x, str)						 \
{													 \
    *((str) + 3) = (unsigned int) ((x)      );       \
    *((str) + 2) = (unsigned int) ((x) >>  8);       \
    *((str) + 1) = (unsigned int) ((x) >> 16);       \
    *((str) + 0) = (unsigned int) ((x) >> 24);       \
}
#define SHA2_PACK32(str, x)							 \
{													 \
    *(x) =   ((unsigned int) *((str) + 3)      )     \
           | ((unsigned int) *((str) + 2) <<  8)     \
           | ((unsigned int) *((str) + 1) << 16)     \
           | ((unsigned int) *((str) + 0) << 24);    \
}

class Encryption
{
private:
	CriticalSection cs;

private:
	using BYTE =  unsigned char;
	using UINT32 = unsigned int;
	using UINT64 = unsigned long long;

private:
	const static UINT32 sha256Key[];
	static const UINT32 SHA224_256BlockSize = (512 / 8);
	static const UINT32 digestSize = (256 / 8);

private:
	void transform(const BYTE *message, UINT32 block_nb);
	UINT32 totalLength;
	UINT32 length;
	BYTE block[2 * SHA224_256BlockSize];
	UINT32 h[8];

private:
	void init();
	void update(const BYTE* message, UINT32 len);
	void final(BYTE* digest);

public:
	std::string EncryptWithSHA256(std::string& input);
};
#endif