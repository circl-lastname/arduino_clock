# Clock for the Arduino starter kit
This is a 24-hour alarm clock sketch which supports multiple alarms and saves settings to EEPROM.

`config.h` contains the pin assignments, you should change them based on your configuration.

## Wiring
* `BTN_OK` -> OK button <- Ground
* `BTN_RIGHT` -> RIGHT button <- Ground
* `SPKR` -> Piezo speaker <- Ground
* `BKLT` -> Resistor (1k used) -> LCD `LED+` pin
* LCD `VSS` pin <- Ground
* LCD `VDD` pin <- +5V
* LCD `V0` pin <- Ground
* `LCD_RS` -> LCD `RS` pin
* LCD `R/W` pin <- Ground
* `LCD_E` -> LCD `E` pin
* `LCD_B4` -> LCD `DB4` pin
* `LCD_B5` -> LCD `DB5` pin
* `LCD_B6` -> LCD `DB6` pin
* `LCD_B7` -> LCD `DB7` pin
* LCD `LED-` pin <- Ground
* `LED` -> Arduino onboard LED or external EEPROM activity LED
