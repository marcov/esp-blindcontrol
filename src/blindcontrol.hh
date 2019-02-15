#ifndef __BLINDCONTROL_HH__
#define __BLINDCONTROL_HH__

#include "config.h"

class BlindControl {

private:
    typedef enum {
        RELAY_UP   = GPIO_RL1,
        RELAY_DOWN = GPIO_RL2,
    };

    const int UP_DURATION_MS = 10000;
    const int DOWN_DURATION_MS = 10000;

    void relayPulse(unsigned pin, unsigned timeMs)
    {
        digitalWrite(pin, LOW);
        digitalWrite(GPIO_LED, LOW);
        delay(timeMs);
        digitalWrite(pin, HIGH);
        digitalWrite(GPIO_LED, HIGH);
    }

public:
    unsigned uptime;
    unsigned lastMoved;
    unsigned moveCtr;

    void updateCounters() {
        uptime++;
        lastMoved++;
    }

    void init() {
        digitalWrite(GPIO_RL1, HIGH);
        digitalWrite(GPIO_RL2, HIGH);

        pinMode(GPIO_RL1, OUTPUT);
        pinMode(GPIO_RL1, OUTPUT);
    }

    int up() {
        relayPulse(RELAY_UP, UP_DURATION_MS);
        moveCtr++;
        return 0;
    }

    int down() {
        relayPulse(RELAY_DOWN, DOWN_DURATION_MS);
        moveCtr++;
        return 0;
    }
};

extern BlindControl blindCtl;

#endif /* #ifndef __BLINDCONTROL_HH__ */
