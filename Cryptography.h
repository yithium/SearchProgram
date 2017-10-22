/*
  Simple XOR encryption and decryption over a string as key
*/

#include <string>  // for holding data

// To create functional object for encrypting and decrypting.
class Cryptography
{
public:
  // construct with key for encrypting and decrypting
  Cryptography(const std::string&);
  
  // encrypt and decrypt function
  std::string Encrypt(std::string);
  std::string Decrypt(std::string);
  
private:
  const std::string _keyStr;
  const size_t _keyLen;
};