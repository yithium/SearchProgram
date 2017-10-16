#include "Cryptography.h"

// construct with key
Cryptography::Cryptography(const std::string& key) : _keyStr(key), _keyLen(key.length())
{
}

// encrypt using key
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

// decrypt using key
// decryption works the same way as encrypt in this case, therefore calls encrypt
std::string Cryptography::Decrypt(std::string input)
{
	return Encrypt(input);
}
