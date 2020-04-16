#include"stdafx.h"
#include"Crypt.h"

bool Crypt::Encrypt(const char* source, char* destination, std::size_t length)
{
	if (source == nullptr || destination == nullptr || length <= 0) return false;

	bool isEven = length % 2 == 0 ? true : false;
	std::size_t halfLength = static_cast<std::size_t>((length + 1) * 0.5);
	std::size_t tempLength = halfLength;

	///Odd
	for (std::size_t i = 0; i < halfLength; i++)
		destination[i] = source[i * 2];

	if (!isEven) halfLength -= 1;

	///Even
	for (std::size_t i = 0; i < halfLength; i++)
		destination[i + tempLength] = source[(i * 2) + 1];

	destination[length] = '\0';

	return true;
}

bool Crypt::Decrypt(const char* source, char* destination, std::size_t length)
{
	if (source == nullptr || destination == nullptr || length <= 0) return false;

	bool isEven = length % 2 == 0 ? true : false;
	std::size_t halfLength = static_cast<std::size_t>((length + 1) * 0.5);
	std::size_t tempLength = halfLength;

	///Even
	for (std::size_t i = 0; i < halfLength; i++)
		destination[i * 2] = source[i];

	if (!isEven) halfLength -= 1;

	///Odd
	for (std::size_t i = 0; i < halfLength; i++)
		destination[(i * 2) + 1] = source[tempLength + i];

	destination[length] = '\0';

	return true;
}