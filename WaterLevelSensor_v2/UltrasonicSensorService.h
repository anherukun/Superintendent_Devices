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

        // Serial.println(duration_us);
        
        // COMPROBACION: SI LA LECTURA DE DURACION ES CERO, SE RETORNARA LA LECTURA ANTERIOR
        if (duration_us == 0)
            return distance_cm;

        // COMPROBACION: SI LA LECTURA ES DE UNA DISTANCIA MAYOR A 600, SE RETORNARA LA
        // LECTURA ANTERIOR

        if (0.017 * duration_us > 600)
            return distance_cm;

        // SE ALMACENARA LA NUEVA LECTURA
        distance_cm = 0.017 * duration_us;

        // SE RETORNA LA LECTURA OBTENIDA
        return distance_cm;
    }
};