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
    int   tId;
    relay_t activeRelay;

    unsigned nextPos;

    void startMoving(relay_t pin, unsigned long timeMs)
    {
        if ((activeRelay != RELAY_NONE) ||
            (digitalRead(RELAY_UP) != IDLE_POLARITY) ||
            (digitalRead(RELAY_DOWN) != IDLE_POLARITY)) {
            return;
        }

        digitalWrite(pin, MOVE_POLARITY);
        activeRelay = pin;

        tId = moveTimer.after(timeMs, timerCallback);
    }

    void stopMoving(void) {
        Serial.println("end move");
        digitalWrite(activeRelay, IDLE_POLARITY);
        activeRelay = RELAY_NONE;
        moveCtr++;
        lastMoved = 0;
    }


public:
    unsigned uptime;
    unsigned lastMoved;
    unsigned moveCtr;
    static constexpr unsigned TOP_T_MS        = BLIND_BOTTOM_2_TOP_MS;
    static constexpr unsigned BOTTOM_T_MS     = BLIND_TOP_2_BOTTOM_MS;
    static constexpr unsigned UPDOWN_MIN_T_MS = 1 * 1000;
    static constexpr unsigned UPDOWN_MAX_T_MS = (TOP_T_MS >= BOTTOM_T_MS) ? TOP_T_MS : BOTTOM_T_MS;

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
        tId = -1;
    }

    int up(unsigned ms) {
        if (ms < UPDOWN_MIN_T_MS || ms > UPDOWN_MAX_T_MS) {
            ms = UPDOWN_MIN_T_MS;
        }
        Serial.println("going up");
        if (activeRelay != RELAY_NONE) {
            return -1;
        }
        startMoving(RELAY_UP, ms);
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
        startMoving(RELAY_DOWN, ms);
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

    int stop(bool timerCallback) {
        if (!timerCallback &&  tId != -1) {
            moveTimer.stop(tId);
        }

        tId = -1;

        if (timerCallback || moving()) {
            stopMoving();
        }

        return 0;
    }
};

extern BlindControl blindCtl;

static inline void timerCallback(void) {
    blindCtl.stop(true);
}


#endif /* #ifndef __BLINDCONTROL_HH__ */
