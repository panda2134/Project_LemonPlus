#ifndef SETTINGSITEM_H
#define SETTINGSITEM_H

#include "settings.h"

class SettingsItem {
public:
    virtual void resetEditSettings(Settings*) = 0;
    virtual bool checkValid() = 0;
};

#endif // SETTINGSITEM_H
