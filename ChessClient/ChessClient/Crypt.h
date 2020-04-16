#ifndef _CRYPT_H_
#define _CRYPT_H_

class Crypt
{
public:
	static bool Encrypt(const char* source, char* destination, std::size_t length);
	static bool Decrypt(const char* source, char* destination, std::size_t length);
};

#endif // !_CRYPT_H_