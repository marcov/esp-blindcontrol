#ifndef __CONFIG_H__
#define __CONFIG_H__

#define GPIO_RL1    12
#define GPIO_RL2    5
#define GPIO_LED    13
#define GPIO_SWITCH 10
#define GPIO_BTN0   0
#define GPIO_BTN1   9

#define SERIAL_BAUDRATE 115200

// TODO: these times may change over time, so it would be
// nice to store them in EEPROM and have them customizable.
#define BLIND_BOTTOM_2_TOP_MS 30700
#define BLIND_TOP_2_BOTTOM_MS 29500

#define EEPROM_SIZE             512
#define EEPROM_START_ADDR       0
#define EEPROM_START_TAG        0xCAFEBABE
#define EEPROM_CTRL_CFG_START   8


#define FW_VERSION          "1.0"
// Changelog:
// 1.0
// - Initial release

#endif /* #ifndef __CONFIG_H__ */
