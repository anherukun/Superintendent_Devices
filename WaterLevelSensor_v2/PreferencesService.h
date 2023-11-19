#include <Arduino.h>
#include <Preferences.h>

class PreferencesService
{
private:
    Preferences prefs;

public:
    PreferencesService(const char *name, bool readonly = false)
    {
        prefs = Preferences();

        prefs.begin(name, readonly);
    }

    void SetString(const char *key, const String value)
    {
        prefs.putString(key, value);
    }

    void Set(const char *key, const int32_t value)
    {
        prefs.putInt(key, value);
    }

    void Set(const char *key, const double_t value)
    {
        prefs.putDouble(key, value);
    }

    void Set(const char *key, const bool value)
    {
        prefs.putBool(key, value);
    }

    String GetString(const char *key)
    {
        return prefs.getString(key);
    }

    int32_t GetInt(const char *key)
    {
        return prefs.getInt(key);
    }

    double_t GetDouble(const char *key)
    {
        return prefs.getDouble(key);
    }

    bool GetBoolean(const char *key)
    {
        return prefs.getBool(key);
    }

    bool HasKey(const char *key)
    {
        return prefs.isKey(key);
    }

    void Remove(const char *key)
    {
        prefs.remove(key);
    }

    void Close()
    {
        prefs.end();
    }
};