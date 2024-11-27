#pragma once
#include <cstdint>
#include <string>

class IEncryption {
public:
	IEncryption(const std::string& algoName, int algoType);
	virtual ~IEncryption() = default;

	virtual void EncryptStream(uint8_t* buffer, size_t length) = 0;
	virtual void DecryptStream(uint8_t* buffer, size_t length) = 0;

	std::string mAlgoName{};
	int mAlgoType = 0;
};
