#include "SpriteInformation.h"

void SpriteInformation::ScaleCoords(float factor)
{
    rect.field_38 *= factor;
    rect.field_48 *= factor;
    rect.field_58 *= factor;
    rect.field_5C *= factor;
    rect.field_60 *= factor;
    rect.field_30 *= factor;
    rect.field_34 *= factor;
}
