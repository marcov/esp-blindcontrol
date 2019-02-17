#ifndef __NVCONFIG_HH__
#define __NVCONFIG_HH__

#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"
#include "credentials.hh"


class NVConfig {
public:
    enum wifi_mode {
        WIFI_MODE_AP,
        WIFI_MODE_STA
    };

    wifi_mode wifiMode;
    String wifiSsid;
    String wifiPassword;

    void init() {
        EEPROM.begin(EEPROM_SIZE);
        EEPROM.read(EEPROM_START_ADDR);

        if (!eeprom_valid()) {
            eeprom_defaults();
        }

        eeprom_fetch();
    }

    void eeprom_fetch() {
        EEPROM.read(EEPROM_START_ADDR);

        auto pos = EEPROM_CTRL_CFG_START;
        wifiMode = static_cast<wifi_mode>(EEPROM[pos++]);

        constexpr int max_str_len = 40;
        char readBuffer[max_str_len + 1];

        int len = EEPROM[pos++];
        if (len > max_str_len) len = max_str_len;
        memcpy(readBuffer, &EEPROM[pos], len);
        readBuffer[len] = '\0';
        pos += len;
        wifiSsid = readBuffer;

        len = EEPROM[pos++];
        if (len > max_str_len) len = max_str_len;
        memcpy(readBuffer, &EEPROM[pos], len);
        readBuffer[len] = '\0';
        pos += len;
        wifiPassword = readBuffer;
    }

    void eeprom_store() {
        auto pos = EEPROM_CTRL_CFG_START;

        EEPROM[pos++] = wifiMode;

        int len = wifiSsid.length();
        EEPROM[pos++] = len;
        memcpy(&EEPROM.getDataPtr()[pos], wifiSsid.c_str(), len);
        pos += len;

        len = wifiPassword.length();
        EEPROM[pos++] = len;
        memcpy(&EEPROM.getDataPtr()[pos], wifiPassword.c_str(), len);
        pos += len;

        EEPROM.commit();
    }

    void eeprom_defaults() {

        for (auto i = 0; i < sizeof(EEPROM_START_TAG); i++) {
            EEPROM[i] = (EEPROM_START_TAG >> i) & 0xFF;
        }

        wifiMode     = WIFI_MODE_AP;

        wifiSsid     = Credentials::wifiSsid;
        wifiPassword = Credentials::wifiPasswd;

        eeprom_store();
        EEPROM.read(EEPROM_START_ADDR);
    }

private:
    union eeprom_variable {
        uint8_t  arr[4];
        float    fvalue;
        int      ivalue;
    };

    bool eeprom_valid() {
        for (auto i = 0; i < sizeof(EEPROM_START_TAG); i++) {
            if (EEPROM[i] != ((EEPROM_START_TAG >> i) & 0xFF)) {
                return false;
            }
        }
        return true;
    }

    void store_var(uint8_t *dst, eeprom_variable v) {
        memcpy(dst, v.arr, sizeof(v.arr));
    }

    void store_var(uint8_t *dst, float f) {
        eeprom_variable v;
        v.fvalue = f;
        store_var(dst, v);
    }

    void store_var(uint8_t *dst, int i) {
        eeprom_variable v;
        v.ivalue = i;
        store_var(dst, v);
    }

    eeprom_variable fetch_var(uint8_t *src) {
        eeprom_variable v;
        memcpy(v.arr, src, sizeof(v.arr));
        return v;
    }
};

extern NVConfig nvConfig;

#endif /* #ifndef __NVCONFIG_HH__ */
