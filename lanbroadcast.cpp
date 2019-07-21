#include "lanbroadcast.h"

#include <QDebug>
#include <QUdpSocket>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QDataStream>
#include <QBuffer>

LanBroadcast::LanBroadcast(QObject *parent) : QObject(parent)
{
}

QNetworkInterface LanBroadcast::getInterface() {
    return QNetworkInterface::allInterfaces().at(interfaceId);
}

void LanBroadcast::startBroadcastTimer()
{
    udp4 = new QUdpSocket(); udp6 = new QUdpSocket(); // ensure that they are created in new thread

    udp4->bind(QHostAddress(QHostAddress::AnyIPv4));
    udp6->bind(QHostAddress(QHostAddress::AnyIPv6));
    udp4->setMulticastInterface(getInterface());
    udp6->setMulticastInterface(getInterface());

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(doBroadcast()));
    timer->setInterval(3000);
    timer->start();
}

void LanBroadcast::doBroadcast()
{
    const auto uniqueId = QString("LEMONPLUS");
    auto hostname = QHostInfo::localHostName();
    QList<QNetworkAddressEntry> addrList = getInterface().addressEntries();
    for (auto addr : addrList) {
        QByteArray datagram;
        QDataStream stream(&datagram, QIODevice::WriteOnly);

        stream.setVersion(QDataStream::Qt_5_7);

        stream << uniqueId << hostname << addr.ip();

        if (addr.ip().protocol() == QAbstractSocket::IPv4Protocol) {
            udp4->writeDatagram(datagram, grpAddr4, 53666);
        } else { // v6 address
            udp4->writeDatagram(datagram, grpAddr6, 53666);
        }
    }
}

quint16 LanBroadcast::getInterfaceId() const
{
    return interfaceId;
}

void LanBroadcast::setInterfaceId(const quint16 value)
{
    interfaceId = value;
}
