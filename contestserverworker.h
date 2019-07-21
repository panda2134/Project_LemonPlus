#ifndef CONTESTSERVERWORKER_H
#define CONTESTSERVERWORKER_H

#include "contestserver.h"
#include "quazip/JlCompress.h"

#include <QtCore>
#include <QThread>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>

const qint64 BYTES_PER_PACKET = 1024;

class ContestServerWorker : public QObject
{
    Q_OBJECT

public:
    explicit ContestServerWorker(QObject *parent, int socketDescriptor);
    ~ContestServerWorker();
    void setServer(ContestServer *value);

signals:
    void startCollecting(const QString &);
    void finishCollecting(const QString &);
    void collectionFailed(const QString &);
public slots:
    void doCollect();
    void startWorker();
private:
    QString idx, cmd;
    qint64 totSize = 0;
    QByteArray zipContent;
    qintptr socketDescriptor;
    QTcpSocket *tcp;
    QDataStream *readStream;
    QString name;
    ContestServer *server;
    QTemporaryFile tmpZip;
    const QString uniqueId = "LEMONPLUS";
    void writeCmd(const QString &cmd);
    void readMessage();
private slots:
    void onReadyRead();
    void doDisconnect();
    void onTcpError(QAbstractSocket::SocketError);
};

#endif // CONTESTSERVERWORKER_H
