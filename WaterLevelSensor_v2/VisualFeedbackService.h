class VisualFeedbackService
{
private:
    const byte FEEDBACK_LED = 9;
    const byte FEEDBACK_CHANNEL = 0;

    boolean currentState = false;

public:
    VisualFeedbackService()
    {
        ledcSetup(FEEDBACK_CHANNEL, 5000, 8);
        ledcAttachPin(FEEDBACK_LED, FEEDBACK_CHANNEL);
        
        Off();
    }

    /// @brief Enciende el indicador visual
    void On(byte brightness = 254)
    {
        ledcWrite(FEEDBACK_CHANNEL, brightness);
    }

    /// @brief Apaga el indicador visual
    void Off()
    {
        ledcWrite(FEEDBACK_CHANNEL, 0);
    }

    /// @brief Alterna el indicador visual entre apagado y encendido. Requiere control del tiempo...
    void Blink()
    {
        if (currentState)
            ledcWrite(FEEDBACK_CHANNEL, 0);

        else
            ledcWrite(FEEDBACK_CHANNEL, 254);

        currentState = !currentState;
    }

    /// @brief Completa un ciclo de encendido 0 apagado en un tiempo determinado, con un efecto visual 'FadeInOut'
    /// @param cycleTime Milisegundos de un ciclo de encendido y apagado
    void FadeInOut(int16_t cycleTime)
    {
        int16_t halfTime = cycleTime / 2;
        byte resolution = 254;

        if (currentState)
        {
            byte brighness = 254;

            for (size_t i = 0; i < resolution; i++)
            {
                brighness = brighness - 1; // ((brighness / resolution) * 1);
                ledcWrite(FEEDBACK_CHANNEL, brighness);

                // Serial.println(brighness);
                delay(cycleTime / resolution);
            }
        }

        else
        {
            byte brighness = 0;

            for (size_t i = resolution; i > 0; i--)
            {
                brighness += 1; // ((brighness / resolution) * i);
                ledcWrite(FEEDBACK_CHANNEL, brighness);

                // Serial.println(brighness);
                delay(cycleTime / resolution);
            }
        }

        currentState = !currentState;
    }
};