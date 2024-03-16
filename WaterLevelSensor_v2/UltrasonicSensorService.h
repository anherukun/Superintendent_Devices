class UltrasonicSensorService
{
private:
    const byte TRIG = D8;
    const byte ECHO = D7;
    float duration_us, distance_cm;

public:
    UltrasonicSensorService()
    {
        pinMode(TRIG, OUTPUT);
        pinMode(ECHO, INPUT);
    }

    float GetDistance()
    {
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);

        duration_us = pulseIn(ECHO, HIGH);
        distance_cm = 0.017 * duration_us;

        if (distance_cm > 600)
            return 0;
            
        return distance_cm;
    }
};