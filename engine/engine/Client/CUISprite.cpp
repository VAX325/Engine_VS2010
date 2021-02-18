#include "Base_include.h"
#include "CUISprite.h"

CUISprite::CUISprite(float x, float y, CSprite* texture, bool visability) : BaseUIElement(x, y, texture, visability)
{

}

CUISprite::CUISprite(float x, float y, float w, float h, CSprite* texture, bool visability) : BaseUIElement(x, y, texture, visability)
{

}