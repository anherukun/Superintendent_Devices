#ifndef CAMERAPROGRAM_H
#define CAMERAPROGRAM_H

#include "ShooterManager.h"
#include "CameraPRGMModes.h"

class CameraProgram
{
private:
    ShooterManager &shooter;

    byte PRGM_Mode = PRGM_MODE_IDLE;
    int Exposition_Time;
    byte Burst_Count;
    byte Reburst_Count;
    int Burst_Time_Between;
    bool Focus;

    void RunSingleShoot()
    {
        shooter.Focus();
        shooter.Shoot();
        shooter.Release();
    }

public:
    CameraProgram(ShooterManager &s) : shooter(s) {}

    byte GetPROGRAM() { return PRGM_Mode; }
    int GetExpositionTime() { return Exposition_Time; }
    byte GetBurstCount() { return Burst_Count; }
    byte GetReburstCount() { return Reburst_Count; }
    int GetBurstTimeBetween() { return Burst_Time_Between; }

    String GetExpositionTimePretty()
    {
        if (Exposition_Time >= 60000)
            return String((float)Exposition_Time / 60000) + " m";

        return String((float)Exposition_Time / 1000) + " s";
    }

    String GetBurstTimeBetweenPretty()
    {
        if (Burst_Time_Between >= 3600000)
            return String((float)Burst_Time_Between / 3600000) + "h";
        if (Burst_Time_Between >= 60000)
            return String((float)Burst_Time_Between / 60000) + " m";

        return String((float)Burst_Time_Between / 1000) + " s";
    }

    void LoadProgram(byte pgrm, int exp_time = 0, byte burst_c = 0, byte rebusrt_c = 0, int burst_time_b = 0)
    {
        PRGM_Mode = pgrm;
        Exposition_Time = exp_time;
        Burst_Count = burst_c;
        Reburst_Count = rebusrt_c;
        Burst_Time_Between = burst_time_b;
    }

    void RunProgram()
    {
        switch (PRGM_Mode)
        {
        case PRGM_MODE_SINGLE_SHOOT:
            RunSingleShoot();
            break;

        case PRGM_MODE_SINGLE_SHOOT_BULB:
            // ui.SetMode("Bulb S.");
            break;

        case PRGM_MODE_BURST_SHOOT:
            // ui.SetMode("Burst S.");
            break;

        case PRGM_MODE_BURST_SHOOT_BULB:
            // ui.SetMode("Bulb B.");
            break;

        case PRGM_MODE_REBURST_SHOOT:
            // ui.SetMode("Reburst");
            break;

        default:
            break;
        }
    }
};

#endif