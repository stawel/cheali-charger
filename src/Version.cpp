#include "Version.h"
#include "memory.h"


int EEPROMVersion EEMEM;

int Version::getCurrentEEPROMVersion()
{
    return eeprom::read(&EEPROMVersion);
}

void Version::restoreDefault()
{
    eeprom::write(&EEPROMVersion, CHEALI_CHARGER_EEPROM_VERSION);
}
