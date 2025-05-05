#include "SpriteInformation.h"

void SpriteInformation::ScaleCoords(float factor)
{
    field_38 *= factor;
    field_48 *= factor;
    field_58 *= factor;
    field_5C *= factor;
    field_60 *= factor;
    field_30 *= factor;
    field_34 *= factor;
}
