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

#define WDT_TIMEOUT_MS              30000

////////////////////////////////////////////////////////////////////////////////
BlindControl blindCtl;
Ticker uptimeCtrTimer;

/******************************************************************************/

static void  uptimeCtrCb(void) {
    blindCtl.updateCounters();
}


void setup(void)
{
    digitalWrite(GPIO_LED,  HIGH);
    pinMode(GPIO_LED,       OUTPUT);
    for (unsigned i = 0; i < 10; i++) {
        digitalWrite(GPIO_LED,  !digitalRead(GPIO_LED));
        delay(100);
    }
    digitalWrite(GPIO_LED,  HIGH);

    Serial.begin(115200);
    Serial.println("....Started up!");

    blindCtl.init();
    //pinMode(BTN_PIN, INPUT);

    uptimeCtrTimer.attach_ms(1000, uptimeCtrCb);

    wifiSetup(false, true, Credentials::wifiSsid, Credentials::wifiPasswd);

    webpagesInit();

    wdtInit(WDT_TIMEOUT_MS);
    wdtEnable();

    Serial.println("setup done");
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

    webserverLoop();
    blindCtl.loop();
}
