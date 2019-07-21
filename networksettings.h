#ifndef NETWORKSETTINGS_H
#define NETWORKSETTINGS_H

#include <QtCore>
#include <QWidget>
#include <QtNetwork>

#include "settingsitem.h"

namespace Ui {
class NetworkSettings;
}

class NetworkSettings : public QWidget, SettingsItem
{
    Q_OBJECT

public:
    explicit NetworkSettings(QWidget *parent = 0);
    ~NetworkSettings();
    void resetEditSettings(Settings*);
    bool checkValid();

private:
    Ui::NetworkSettings *ui;
    Settings *editSettings;
    QList<QNetworkInterface> ifList;

private slots:
    void networkInterfaceChanged();
    void lookupIntervalChanged();
};

#endif // NETWORKSETTINGS_H
