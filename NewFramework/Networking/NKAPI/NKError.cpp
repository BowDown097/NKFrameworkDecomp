#include "NKError.h"

bool NKError::IsValid() const
{
    return static_cast<int>(type) > 0 || !reason.empty() || !typeStr.empty() || !fix.empty();
}
