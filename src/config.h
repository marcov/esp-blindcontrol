#ifndef __CONFIG_H__
#define __CONFIG_H__

#define GPIO_RL1    12
#define GPIO_RL2    5
#if defined(BOARD_SONOFF_DUAL)
#  define GPIO_LED    13
#elif defined(BOARD_D1_MINI)
#  define GPIO_LED    2
#else
#  error No board defined or board not supported
#endif
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

#define FW_VERSION          "1.5"
// Changelog:
// 1.5
// - New WIFI management
// 1.4
// - Fix html pages title
// - Add repeat function to UP and DOWN buttons, to repeat going up or
// down
// 1.3
// - Add "STOP" button
// 1.2
// - Show last opened in hh:mm:ss format
// 1.1
// - Use ms timings instead of s
// - Calibrate top/bottom based on actual blind travel time
// 1.0
// - Initial release

#endif /* #ifndef __CONFIG_H__ */
