#include "StephenEncryption.h"

CStephenEncryption::CStephenEncryption() : IEncryption("\"StephenEncryption", 1) {}

void CStephenEncryption::EncryptStream(uint8_t* buffer, size_t length) {
	if (length == 0)
		return;

	int8_t count = 0x15;
	for (auto i = 0; i < length; i++) {
		buffer[i] = buffer[i] + count + (i / 6) * -6;
		count++;
	}
}

void CStephenEncryption::DecryptStream(uint8_t* buffer, size_t length) {
	if (length == 0)
		return;

	int8_t count = -0x15;
	for (auto i = 0; i < length; i++) {
		buffer[i] = buffer[i] + (i / 6) * 6 + count;
		count--;
	}
}