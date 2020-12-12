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
static constexpr unsigned int k_MaxConnectTime = 1000UL;
WIFI* g_WIFI;
static Ticker uptimeCtrTimer;
NVConfig  nvConfig;

/******************************************************************************/

static
void
uptimeCtrCb (
    void
    )
{
    blindCtl.updateCounters();
}

static
void
ConnectedCallback (
    void
    )
{
    unsigned int originalState;

    originalState = digitalRead(GPIO_LED);

    for (int i = 0; i < 4; i++)
    {
        //
        // Flash led to say we have just connected.
        //
        digitalWrite(GPIO_LED, (i % 2) ? originalState : !originalState);
        delay(100);
    }

    digitalWrite(GPIO_LED, originalState);
}

void setup (
    void
    )
{
    blindCtl.init();
    g_WIFI = new WIFI(false, true, Credentials::wifiSsid,Credentials::wifiPasswd, k_MaxConnectTime);

    //
    // Signal we are up!
    //
    digitalWrite(GPIO_LED,  HIGH);
    pinMode(GPIO_LED,       OUTPUT);
    for (unsigned i = 0; i < 10; i++) {
        digitalWrite(GPIO_LED,  !digitalRead(GPIO_LED));
        delay(100);
    }
    digitalWrite(GPIO_LED,  HIGH);

    pinMode(GPIO_SWITCH, INPUT);

    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("...starting up");

    uptimeCtrTimer.attach_ms(1000, static_cast<Ticker::callback_function_t>(uptimeCtrCb));

    g_WIFI->Connect(5000);
    if (g_WIFI->IsConnected())
    {
        ConnectedCallback();
    }

    webpagesInit();

    wdtInit(WDT_TIMEOUT_MS);
    wdtEnable();

    Serial.println("setup done");
}

static void button_check (
    void
    )
{
    typedef enum ButtonState
    {
        ButtonPressed = 0,
        ButtonReleased = 1,
    } ButtonState;

    constexpr int reset_counter_val = 6;
    static ButtonState cachedButtonState = ButtonReleased;
    static int lastChangeTs;
    static int resetCtr = 0;
    static bool nextUp;
    ButtonState currentButtonState;

    currentButtonState = static_cast<ButtonState>(digitalRead(GPIO_SWITCH));

    //
    // Only check every 1s
    //
    if (millis() - lastChangeTs < 1000)
    {
        goto Exit;
    }

    if (cachedButtonState == ButtonReleased && currentButtonState == ButtonPressed)
    {
        lastChangeTs = millis();

        if (!blindCtl.moving())
        {
            if (nextUp) blindCtl.up(BlindControl::UPDOWN_MIN_T_MS);
            else        blindCtl.down(BlindControl::UPDOWN_MIN_T_MS);
            nextUp = !nextUp;
        }
    }
    else if (cachedButtonState == ButtonPressed && currentButtonState == ButtonPressed)
    {
        lastChangeTs = millis();
        digitalWrite(GPIO_LED,  !digitalRead(GPIO_LED));
        if (resetCtr++ >= reset_counter_val)
        {
            //nvConfig.eeprom_defaults();
            ESP.restart();  // normal reboot
        }
    }
    else if (resetCtr > 0)
    {
        resetCtr = 0;
        digitalWrite(GPIO_LED, HIGH);
    }

Exit:
    cachedButtonState = currentButtonState;
}

void loop (
    void
    )
{
    wdtKick();

    g_WIFI->Loop(ConnectedCallback);

    button_check();
    webserverLoop();
    blindCtl.loop();
}
