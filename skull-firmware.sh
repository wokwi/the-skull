avrdude -pattiny45 -cusbtiny -e -U eeprom:w:skull-firmware.eep:i -U flash:w:skull-firmware.hex:i -Uefuse:w:0xff:m -Uhfuse:w:0xdf:m -Ulfuse:w:0x62:m -Ulock:w:0xFC:m
