#ifndef __BLINDCONTROL_HH__
#define __BLINDCONTROL_HH__

#include "config.h"
#include <Timer.h>

#define max(A,B)  ((A) >= (B) ? (A) : (B))

static inline void timerCallback(void);

class BlindControl {

private:
    typedef enum {
        RELAY_UP   = GPIO_RL1,
        RELAY_DOWN = GPIO_RL2,
        RELAY_NONE = 0xFF,
    } relay_t;

    enum  {
        MOVE_POLARITY = HIGH,
        IDLE_POLARITY = LOW,
    };
    Timer moveTimer;
    relay_t activeRelay;

    unsigned nextPos;

    void relayPulse(relay_t pin, unsigned long timeMs)
    {
        if ((activeRelay != RELAY_NONE) ||
            (digitalRead(RELAY_UP) != IDLE_POLARITY) ||
            (digitalRead(RELAY_DOWN) != IDLE_POLARITY)) {
            return;
        }

        digitalWrite(pin, MOVE_POLARITY);
        activeRelay = pin;

        moveTimer.after(timeMs, timerCallback);
    }

public:
    unsigned uptime;
    unsigned lastMoved;
    unsigned moveCtr;
    static constexpr unsigned TOP_T_MS        = BLIND_BOTTOM_2_TOP_MS;
    static constexpr unsigned BOTTOM_T_MS     = BLIND_TOP_2_BOTTOM_MS;
    static constexpr unsigned UPDOWN_MIN_T_MS = 1 * 1000;
    static constexpr unsigned UPDOWN_MAX_T_MS = (TOP_T_MS >= BOTTOM_T_MS) ? TOP_T_MS : BOTTOM_T_MS;

    void moveCallback(void) {
        Serial.println("end move");
        digitalWrite(activeRelay, IDLE_POLARITY);
        activeRelay = RELAY_NONE;
        moveCtr++;
        lastMoved = 0;
    }

    void updateCounters() {
        uptime++;
        lastMoved++;
    }

    void init() {
        activeRelay = RELAY_NONE;
        digitalWrite(GPIO_RL1, IDLE_POLARITY);
        digitalWrite(GPIO_RL2, IDLE_POLARITY);

        pinMode(GPIO_RL1, OUTPUT);
        pinMode(GPIO_RL2, OUTPUT);
    }

    int up(unsigned ms) {
        if (ms < UPDOWN_MIN_T_MS || ms > UPDOWN_MAX_T_MS) {
            ms = UPDOWN_MIN_T_MS;
        }
        Serial.println("going up");
        if (activeRelay != RELAY_NONE) {
            return -1;
        }
        relayPulse(RELAY_UP, ms);
        return 0;
    }

    int down(unsigned ms) {
        if (ms < UPDOWN_MIN_T_MS || ms > UPDOWN_MAX_T_MS) {
            ms = UPDOWN_MIN_T_MS;
        }
        Serial.println("going down");
        if (activeRelay != RELAY_NONE) {
            return -1;
        }
        relayPulse(RELAY_DOWN, ms);
        return 0;
    }

    int top() {
        return up(TOP_T_MS);
    }

    int bottom() {
        return down(BOTTOM_T_MS);
    }

    void loop() {
        moveTimer.update();
    }

    bool moving() {
        return (activeRelay != RELAY_NONE);
    }
};

extern BlindControl blindCtl;

static inline void timerCallback(void) {
    blindCtl.moveCallback();
}


#endif /* #ifndef __BLINDCONTROL_HH__ */
