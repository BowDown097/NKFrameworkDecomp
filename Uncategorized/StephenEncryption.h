#pragma once
#include "Encryption.h"

class CStephenEncryption : public IEncryption {
public:
	CStephenEncryption();
	~CStephenEncryption() override = default;

	void EncryptStream(uint8_t* buffer, size_t length) override;
	void DecryptStream(uint8_t* buffer, size_t length) override;
};