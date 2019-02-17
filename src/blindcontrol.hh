#ifndef __BLINDCONTROL_HH__
#define __BLINDCONTROL_HH__

#include "config.h"
#include <Timer.h>

static inline void timerCallback(void);

class BlindControl {

private:
    typedef enum {
        RELAY_UP   = GPIO_RL1,
        RELAY_DOWN = GPIO_RL2,
        RELAY_NONE = 0xFF,
    } relay_t;

    const unsigned UP_DURATION_MS = 1000;
    const unsigned DOWN_DURATION_MS = 1000;
    Timer moveTimer;
    relay_t activeRelay;

    void relayPulse(relay_t pin, unsigned long timeMs)
    {
        if ((digitalRead(RELAY_UP) != LOW) ||
            (digitalRead(RELAY_DOWN) != LOW)) {
            return;
        }

        digitalWrite(pin, HIGH);
        digitalWrite(GPIO_LED, LOW);
        activeRelay = pin;

        moveTimer.after(timeMs, timerCallback);
    }

public:

    unsigned uptime;
    unsigned lastMoved;
    unsigned moveCtr;

    void moveCallback(void) {
        digitalWrite(activeRelay, LOW);
        digitalWrite(GPIO_LED, HIGH);
        activeRelay = RELAY_NONE;
    }

    void updateCounters() {
        uptime++;
        lastMoved++;
    }

    void init() {
        activeRelay = RELAY_NONE;
        digitalWrite(GPIO_RL1, LOW);
        digitalWrite(GPIO_RL2, LOW);

        pinMode(GPIO_RL1, OUTPUT);
        pinMode(GPIO_RL2, OUTPUT);
    }

    int up() {
        Serial.println("going up");
        if (activeRelay != RELAY_NONE) {
            return -1;
        }
        relayPulse(RELAY_UP, UP_DURATION_MS);
        moveCtr++;
        return 0;
    }

    int down() {
        Serial.println("going down");
        if (activeRelay != RELAY_NONE) {
            return -1;
        }
        relayPulse(RELAY_DOWN, DOWN_DURATION_MS);
        moveCtr++;
        return 0;
    }

    void loop() {
        moveTimer.update();
    }
};

extern BlindControl blindCtl;

static inline void timerCallback(void) {
    blindCtl.moveCallback();
}


#endif /* #ifndef __BLINDCONTROL_HH__ */
