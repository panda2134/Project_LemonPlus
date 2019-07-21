#ifndef LANBROADCAST_H
#define LANBROADCAST_H

#include <QObject>
#include <QTimer>
#include <QNetworkInterface>
#include <QUdpSocket>

class LanBroadcast : public QObject
{
    Q_OBJECT
public:
    explicit LanBroadcast(QObject *parent = nullptr);

    quint16 getInterfaceId() const;
    void setInterfaceId(const quint16 value);

signals:

public slots:
    void startBroadcastTimer();
    void doBroadcast();

private:
    const QHostAddress grpAddr4 = QHostAddress("224.0.36.166");
    const QHostAddress grpAddr6 = QHostAddress("ff18::3666");

    QTimer *timer;
    quint16 interfaceId;
    QUdpSocket *udp4, *udp6;

    QNetworkInterface getInterface();
};

#endif // LANBROADCAST_H
