#ifndef UIMANAGER_H
#define UIMANAGER_H

// Eliminar del directorio C:\Users\angel\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.14\tools\sdk\esp32c3\include
// la carpeta qrcode
#include "qrcode.h"
#include "CameraProgram.h"
#include "Icons.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c // initialize with the I2C addr 0x3C Typically eBay OLED's
// #define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    //   QT-PY / XIAO

#define SAFE_Y_1 9
#define SAFE_Y_2 55

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class UIManager
{
private:
    String Status_Value;
    String Info02_Value;

    String Mode_Value;
    String ExpoTime_Value;

    void ShowTaskBar()
    {
        display.fillRect(0, 8, 128, 1, SH110X_WHITE);
        display.setCursor(0, 0);
        display.setTextSize(1);
        display.setTextColor(SH110X_BLACK, SH110X_WHITE);
        display.print("Status: ");
        display.setTextColor(SH110X_WHITE, SH110X_BLACK);
        display.print(Status_Value);
        display.display();
    }

    void ShowInfoBar()
    {
        // String ip_string = WiFi.softAPIP().toString();
        // Serial.println(ip_string);

        display.fillRect(0, 55, 128, 1, SH110X_WHITE);
        display.setCursor(128 - (Info02_Value.length() * 6), 56);
        display.setTextSize(1);
        display.setTextColor(SH110X_BLACK, SH110X_WHITE);
        display.print(Info02_Value);
        display.display();
    }

public:
    UIManager() {}
    UIManager(int sda, int scl)
    {
        Wire.begin(sda, scl);

        delay(250);                       // wait for the OLED to power up
        display.begin(i2c_Address, true); // Address 0x3C default

        display.display();
        delay(50);

        // Clear the buffer.
        display.clearDisplay();
        display.display();
    }

    void Setup(int sda, int scl)
    {
        Wire.begin(sda, scl);

        delay(250);                       // wait for the OLED to power up
        display.begin(i2c_Address, true); // Address 0x3C default

        display.display();
        delay(50);

        // Clear the buffer.
        display.clearDisplay();
        display.display();
    }

    void SetStatus(String v) { this->Status_Value = v; }
    void SetInfo02(String v) { this->Info02_Value = v; }
    void SetMode(String v) { this->Mode_Value = v; }
    void SetExpoTime(String v) { this->ExpoTime_Value = v; }

    void ClearDisplay() { display.clearDisplay(); }

    void UpdateUI(byte pgrm_option)
    {
        ClearDisplay();

        ShowTaskBar();
        SelectUILayout(pgrm_option);
        ShowInfoBar();
    }

    void SelectUILayout(byte pgrm_option)
    {
        switch (pgrm_option)
        {
        case PRGM_MODE_SINGLE_SHOOT:
            LoadSingleShootModeUI();
            break;
        case PRGM_MODE_SINGLE_SHOOT_BULB:
            LoadSingleShootBulbModeUI();
            break;

        default:
            break;
        }
    }

    void LoadSingleShootModeUI()
    {
        display.setCursor(0, SAFE_Y_1 + 1);
        display.setTextSize(1);
        display.print("M: ");
        display.println(Mode_Value);
        display.display();
    }

    void LoadSingleShootBulbModeUI()
    {
        display.setCursor(0, SAFE_Y_1 + 1);
        display.setTextSize(1);
        display.print("M: ");
        display.println(Mode_Value);
        display.drawBitmap(display.getCursorX(), display.getCursorY(), ROUND_CLOCK, 8, 8, SH110X_WHITE);
        display.println(ExpoTime_Value);
        display.display();
    }

    void PrintWifiQR(String ssid, String password)
    {
        //  "WIFI:S:MiRedWiFi;T:WPA;P:contraseña123;H:false;;"
        delay(100);
        String wifiData = "WIFI:S:" + ssid + ";T:WPA;P:" + password + ";H:false;;";
        const char *ptrWifiData = wifiData.c_str();

        Serial.println(ptrWifiData);
        delay(100);

        QRCode qrcode;
        uint8_t qrcodeData[qrcode_getBufferSize(3)];
        qrcode_initText(&qrcode, qrcodeData, 3, 0, ptrWifiData);

        Serial.println("QR Generated, printing on Screen");
        // Clear the display
        ClearDisplay();

        display.setTextSize(1);
        // display.setCursor(0, 0);
        display.setCursor((SCREEN_WIDTH / 2) - (15 * 6) / 2, 0);
        display.setTextColor(SH110X_BLACK, SH110X_WHITE); // 'inverted' text
        display.println("Scan to connect");

        // Calculate the scale factor
        int scale = min(SCREEN_WIDTH / qrcode.size, (SCREEN_HEIGHT - 6) / qrcode.size);

        // Calculate horizontal shift
        int shiftX = (SCREEN_WIDTH - qrcode.size * scale) / 2;

        // Calculate horizontal shift
        int shiftY = ((SCREEN_HEIGHT + 6) - qrcode.size * scale) / 2;

        // Draw the QR code on the display
        for (uint8_t y = 0; y < qrcode.size; y++)
        {
            for (uint8_t x = 0; x < qrcode.size; x++)
            {
                if (qrcode_getModule(&qrcode, x, y))
                {
                    display.fillRect(shiftX + x * scale, shiftY + y * scale, scale, scale, SH110X_WHITE);
                }
            }
        }

        // Update the display
        display.display();
    }
};

#endif