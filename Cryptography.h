#include <string>

class Cryptography
{
public:
	Cryptography(const std::string&);
	std::string Encrypt(std::string);
	std::string Decrypt(std::string);
private:
	const std::string _keyStr;
	const size_t _keyLen;
};