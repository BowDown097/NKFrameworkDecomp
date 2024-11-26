#include "Encryption.h"

IEncryption::IEncryption(const std::string& algoName, int algoType) : mAlgoName(algoName), mAlgoType(algoType) {}