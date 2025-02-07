#include <Adafruit_NeoPixel.h>

#define RX_BUF_SIZE 1024

byte pixelPin = 4;
byte pixelCount = 64;

int inByte;

String inputString = "";        // A string to hold incoming data
boolean stringComplete = false; // Whether the string is complete

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(pixelCount, pixelPin, NEO_GRB + NEO_KHZ800);

void setup()
{
    Serial.setRxBufferSize(RX_BUF_SIZE);
    Serial.begin(19200);
    pixels.begin();

    // pixels.setBrightness(255 / 2);
}

// the loop function runs over and over again forever
void loop()
{
    // method_2();
    method_1();
    pixels.show();
    // if (stringComplete == true)
    // {
    //     Serial.println(inputString);

    //     delay(250);

    //     stringComplete = false;
    //     inputString = "";
    // }

    // pixels.clear();
    Serial.flush();
}

void method_1()
{
    while (Serial.available())
    {
        // inByte = Serial.read();
        byte i_code = Serial.read();
        byte r_code = Serial.read();
        byte g_code = Serial.read();
        byte b_code = Serial.read();

        // pixels.setPixelColor(0, r_code, g_code, b_code);

        pixels.setPixelColor(i_code, r_code, g_code, b_code);
        // pixels.show();

        // char inChar = (char)inByte;

        // if (inChar == '\n')
        //     stringComplete = true;
        // else
        //     inputString += inChar;

        // delay(100);
    }
}

void method_2()
{
    static bool recibiendo = false;
    static byte longitud = 0;
    static byte bytesRecibidos = 0;
    static uint8_t buffer[256];
    static uint8_t checksumRecibido = 0;

    while (Serial.available())
    {
        uint8_t c = Serial.read();

        if (!recibiendo)
        {
            if (c == 0x01)
            { // Inicio de paquete
                recibiendo = true;
                bytesRecibidos = 0;
                Serial.println("Recibiendo");
            }
        }
        else
        {
            // Si es el primer byte después de STX, es la longitud
            if (bytesRecibidos == 0)
            {
                longitud = c;
                bytesRecibidos++;
                Serial.print("Longitud de datos: ");
                Serial.println(longitud);
            }
            else if (bytesRecibidos <= longitud)
            {
                buffer[bytesRecibidos - 1] = c;
                bytesRecibidos++;
            }
            else if (bytesRecibidos == longitud + 1)
            {
                checksumRecibido = c; // Recibimos el checksum
                bytesRecibidos++;
                Serial.print("Checksum: ");
                Serial.println(checksumRecibido);
            }
            else if (c == 0x04)
            {
                Serial.println("Transmision terminada");

                // Aquí ya se tiene el paquete completo, se verifica el checksum
                uint8_t checksumCalculado = 0;
                for (int i = 0; i < longitud; i++)
                {
                    checksumCalculado += buffer[i];

                    checksumCalculado = checksumCalculado % 256;
                }
                if (checksumCalculado == checksumRecibido)
                {
                    Serial.println("Procesando datos");

                    // Procesar datos
                    updateMatrix(buffer, longitud);
                }
                else
                {
                    Serial.println("Checksum invalido");
                    // Error en la transmisión, se descarta el paquete
                }
                // Reiniciar para esperar el siguiente paquete
                recibiendo = false;
                bytesRecibidos = 0;
            }
            else
            {
                // Error: byte inesperado, se reinicia la recepción
                recibiendo = false;
                bytesRecibidos = 0;
            }
        }
    }
}

void updateMatrix(uint8_t buffer[], byte length)
{
    for (byte i = 0; i < length; i++)
    {
        pixels.setPixelColor(i, buffer[i]);
        Serial.print(buffer[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void method_3()
{
    for (size_t i = 0; i < 255; i++)
    {
        pixels.setPixelColor(0, i);
        pixels.setPixelColor(1, i);
        pixels.setPixelColor(2, i);

        pixels.show();
    }
}

void method_4()
{
    //          0            1              2
    // [01][length...][1D][data...][1D][checksum][04]

    static int step = 0;
    static bool recibiendo = false;
    static int longitud = 0;
    static uint8_t *buffer;
    static uint8_t checksumRecibido = 0;

    if (Serial.available())
    {
        uint8_t c = Serial.read();

        if (c == 0x01)
        {
            recibiendo = true;
        }
    }

    while (Serial.available() && recibiendo)
    {
        uint8_t c = Serial.read();

        if (c == 0x1D)
            step += 1;

        switch (step)
        {
        case 0:
            longitud = Serial.read() * 4;
            break;

        case 1:
            buffer = RecibePackage(longitud);
            break;

        case 2:
            checksumRecibido = ReadChecksum();
            recibiendo = false;
            break;
        default:
            break;
        }
    }

    // Serial.println("Transmision terminada");
    // Serial.println("Verificando datos");

    uint8_t checksumCalculado = 0;
    for (int i = 0; i < longitud; i++)
    {
        checksumCalculado += buffer[i];

        checksumCalculado = checksumCalculado % 256;
    }
    if (checksumCalculado == checksumRecibido)
    {
        Serial.println(" Checksum correcto, Procesando datos");

        // Procesar datos
        UpdateMatrix_2(buffer, longitud);
    }
    else
    {
        Serial.println("Checksum invalido");
        // Error en la transmisión, se descarta el paquete
    }
}

void UpdateMatrix_2(uint8_t buffer[], byte length)
{
    Serial.print("Tamaño del buffer es de ");
    Serial.println(sizeof(buffer));

    for (size_t i = 0; i < sizeof(buffer); i += 4)
    {
        byte i_code = Serial.read();
        byte r_code = Serial.read();
        byte g_code = Serial.read();
        byte b_code = Serial.read();

        pixels.setPixelColor(i_code, r_code, g_code, b_code);
        Serial.println(i_code);
    }
}

uint8_t ReadPackageLength()
{
    uint8_t c = 0;
    while (Serial.available())
    {
        byte b = Serial.read();

        if (b == 0x1D)
            return c;

        c += b;
    }
}

byte *RecibePackage(uint8_t length)
{
    byte data[length];
    uint8_t index = 0;

    if (Serial.available())
    {
        for (size_t i = 0; i < length; i++)
        {
            data[i] = Serial.read();
        }

        return data;
    }

    return data;
}

uint8_t ReadChecksum()
{
    uint8_t checkksum = Serial.read() % 256;

    return checkksum;
}