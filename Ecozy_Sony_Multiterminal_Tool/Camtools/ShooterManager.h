#ifndef SHOOTERMANAGER_H
#define SHOOTERMANAGER_H

#include "../GPIO_Definitions.h"
#include "../UI/UIManager.h"
// #include "CameraProgram.h"

class ShooterManager
{
private:
    UIManager &ui;

public:
    // ShooterManager() : ui() {}
    ShooterManager(UIManager &u) : ui(u)
    {
        pinMode(GPIO_SHUTTER, OUTPUT);
        pinMode(GPIO_FOCUS, OUTPUT);
        pinMode(GPIO_POWER, OUTPUT);

        digitalWrite(GPIO_SHUTTER, LOW);
        digitalWrite(GPIO_FOCUS, LOW);
        digitalWrite(GPIO_POWER, LOW);
    }

    void Focus()
    {
        digitalWrite(GPIO_FOCUS, HIGH);
        delay(100);
    }

    void Focus(int d)
    {
        digitalWrite(GPIO_FOCUS, HIGH);
        delay(d);
    }

    void Shoot(int millis = 0)
    {
        digitalWrite(GPIO_SHUTTER, HIGH);
        delay(millis + 2);
        digitalWrite(GPIO_SHUTTER, LOW);
    }

    void Release(bool shutter = false, bool focus = false) 
    {
        if (shutter)
            digitalWrite(GPIO_SHUTTER, LOW);
        if (focus)
            digitalWrite(GPIO_FOCUS, LOW);
    }

    void TakeShoot()
    {
        ui.SetStatus("Focussing");
        delay(100);
        digitalWrite(GPIO_FOCUS, HIGH);

        ui.SetStatus("Shooting");
        delay(1000);
        digitalWrite(GPIO_SHUTTER, HIGH);
        delay(100);
        digitalWrite(GPIO_SHUTTER, LOW);
        digitalWrite(GPIO_FOCUS, LOW);
        ui.SetStatus("Idle");
    }
};

// ShooterManager::ShooterManager(/* args */)
// {
// }

#endif