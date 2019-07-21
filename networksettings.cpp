#include "networksettings.h"
#include "ui_networksettings.h"
#include "settings.h"

#include <QDebug>

NetworkSettings::NetworkSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkSettings)
{
    ui->setupUi(this);

    ifList = QNetworkInterface::allInterfaces();
    for (auto interface : ifList)
    {
        ui->networkInterface->addItem(interface.humanReadableName());
    }

    ui->lookupInterval->setSingleStep(500);
    ui->lookupInterval->setRange(500, 60000);
    ui->lookupInterval->setSuffix("ms");

    connect(ui->networkInterface, SIGNAL(currentIndexChanged(int)), this, SLOT(networkInterfaceChanged()));
    connect(ui->lookupInterval, SIGNAL(valueChanged(int)), this, SLOT(lookupIntervalChanged()));
}

NetworkSettings::~NetworkSettings()
{
    delete ui;
}

void NetworkSettings::resetEditSettings(Settings* settings)
{
    editSettings = settings;

    auto idx = editSettings->getNetworkInterfaceIndex();

    if (0 <= idx && idx < ifList.size())
        ui->networkInterface->setCurrentIndex(idx);
    ui->lookupInterval->setValue(editSettings->getLookupInterval());
}

bool NetworkSettings::checkValid()
{
    return true;
}

void NetworkSettings::networkInterfaceChanged()
{
    editSettings->setNetworkInterfaceIndex(ui->networkInterface->currentIndex());
}

void NetworkSettings::lookupIntervalChanged()
{
    editSettings->setLookupInterval(ui->lookupInterval->value());
}
