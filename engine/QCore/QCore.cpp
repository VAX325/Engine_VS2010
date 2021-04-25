#include "Base_include.h"
#include "QCore.h"

bool CORE_INITILAIZED = false;

void QCORE_PUBLIC CoreInit()
{
    InitLogManager(false);
    CORE_INITILAIZED = true;
}

bool QCORE_PUBLIC IsCoreInitilaized()
{
    return CORE_INITILAIZED;
}