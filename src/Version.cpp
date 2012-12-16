#include "Version.h"
#include "memory.h"


int EEPROMVersion EEMEM;

int Version::getCurrentEEPROMVersion()
{
	return eeprom_read(&EEPROMVersion);
}

void Version::restoreDefault()
{
	eeprom_write(&EEPROMVersion, CHEALI_CHARGER_EEPROM_VERSION);
}
