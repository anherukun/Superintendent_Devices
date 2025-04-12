#include "GPIO_Definitions.h"
#include "UIManager.h"

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
