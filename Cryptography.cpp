#include "Cryptography.h"

Cryptography::Cryptography(const std::string& key) : _keyStr(key), _keyLen(key.length())
{
}

std::string Cryptography::Encrypt(std::string input)
{
	size_t strLen = input.length();
	size_t idx = 0;
	for(; idx < strLen; idx += _keyLen)
	{
		for(size_t offset = 0; offset < _keyLen && offset + idx < strLen; ++offset)
		{
			input[idx + offset] ^= _keyStr[offset];
		}
	}
	return input;
}

std::string Cryptography::Decrypt(std::string input)
{
	return Encrypt(input);
}
