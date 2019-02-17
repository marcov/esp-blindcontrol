/*
 Gates Control
*/
#include <pgmspace.h>
#include <Ticker.h>
#include "config.h"
#include "webpages.h"
#include "webserver.h"
#include "wdt.h"
#include "credentials.hh"
#include "wifi.h"
#include "blindcontrol.hh"
#include "nvconfig.hh"

#define WDT_TIMEOUT_MS              30000

////////////////////////////////////////////////////////////////////////////////
BlindControl blindCtl;
Ticker uptimeCtrTimer;
NVConfig  nvConfig;

/******************************************************************************/

static void  uptimeCtrCb(void) {
    blindCtl.updateCounters();
}


void setup(void)
{
    blindCtl.init();

    digitalWrite(GPIO_LED,  HIGH);
    pinMode(GPIO_LED,       OUTPUT);
    for (unsigned i = 0; i < 10; i++) {
        digitalWrite(GPIO_LED,  !digitalRead(GPIO_LED));
        delay(100);
    }
    digitalWrite(GPIO_LED,  HIGH);

    pinMode(GPIO_SWITCH, INPUT);

    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("....starting up");

    uptimeCtrTimer.attach_ms(1000, uptimeCtrCb);

    wifiSetup(false, true, Credentials::wifiSsid, Credentials::wifiPasswd);

    webpagesInit();

    wdtInit(WDT_TIMEOUT_MS);
    wdtEnable();

    Serial.println("setup done");
}

static void button_check()
{
    constexpr int reset_counter_val = 6;
    static int prevValue = 1;
    static int lastChangeTs;
    static int resetCtr = 0;
    static bool nextUp;
    int curValue = digitalRead(GPIO_SWITCH);

    if (millis() - lastChangeTs >= 1000) {

        if (prevValue == 1 && curValue == 0) {
            lastChangeTs = millis();

            if (!blindCtl.moving()) {
                if (nextUp) blindCtl.up(BlindControl::UPDOWN_MIN_T_SEC);
                else        blindCtl.down(BlindControl::UPDOWN_MIN_T_SEC);
                nextUp = !nextUp;
            }

        } else if (prevValue == 0 && curValue == 0) {
            lastChangeTs = millis();
            digitalWrite(GPIO_LED,  !digitalRead(GPIO_LED));
            if (resetCtr++ >= reset_counter_val) {
                //nvConfig.eeprom_defaults();
                ESP.restart();  // normal reboot
            }

        } else if (resetCtr > 0) {
            resetCtr = 0;
            digitalWrite(GPIO_LED, HIGH);
        }
    }

    prevValue = curValue;
}

void loop(void)
{
    wdtKick();

    if (wifiHasConnected()) {
        // flash led to say we have just connected...?

        unsigned prev_state = digitalRead(GPIO_LED);

        digitalWrite(GPIO_LED, !prev_state);
        delay(100);
        digitalWrite(GPIO_LED, prev_state);
    }

    button_check();
    webserverLoop();
    blindCtl.loop();
}
