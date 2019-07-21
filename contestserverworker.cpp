#include "contestserverworker.h"
#include "settings.h"

#include <QDebug>

ContestServerWorker::ContestServerWorker(QObject *parent, int socketDescriptor)
    : QObject(parent), socketDescriptor(socketDescriptor)
{

}

ContestServerWorker::~ContestServerWorker()
{
    delete this->tcp;
}

void ContestServerWorker::startWorker()
{
    tcp = new QTcpSocket();
    if(!tcp->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "fail to set socket descriptor!";
        return;
    }
    readStream = new QDataStream(tcp);
    readStream->setVersion(QDataStream::Qt_5_7);

    connect(tcp, SIGNAL(readyRead()), this, SLOT(onReadyRead()), Qt::DirectConnection);
    connect(tcp, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onTcpError(QAbstractSocket::SocketError)));
}

void ContestServerWorker::doCollect()
{
    qDebug() << "sending collect msg...";
    emit startCollecting(name);
    writeCmd("COLLECT_CODE");
    qDebug() << "collect msg sent...";
}

void ContestServerWorker::setServer(ContestServer *value)
{
    server = value;
}

void ContestServerWorker::writeCmd(const QString &cmd)
{
    QDataStream writeStream(tcp);
    writeStream.setVersion(QDataStream::Qt_5_7);
    writeStream << uniqueId << cmd;
    tcp->flush();
}

void ContestServerWorker::doDisconnect()
{
    tcp->disconnectFromHost();
    if(tcp->state() != QTcpSocket::UnconnectedState)
        tcp->waitForDisconnected();
    if(name != "") {
        server->removeOnline(name);
        qDebug() << "disconnected from " << name;
    }
    deleteLater();
}

void ContestServerWorker::onReadyRead() {
    while(true) {
        readStream->startTransaction();
        this->readMessage();
        if(!readStream->commitTransaction())
            break;
    }
}

void ContestServerWorker::readMessage()
{
    idx = cmd = "";
    (*readStream) >> idx >> cmd;
    if(readStream->status() != QDataStream::Ok) return;
    qDebug() << "SERVER CMD : " << idx << cmd;
    if(idx != uniqueId) {
        qDebug() << idx << "Non-Lemon+ Client trying to connect!";
        emit collectionFailed(name); doDisconnect();
        return;
    }
    if(cmd == "REGISTER") {
        QString clientName;

        (*readStream) >> clientName;
        if(readStream->status() != QDataStream::Ok) return;
        qDebug() << "GOT CLIENT_NAME = " << clientName;
        qDebug() << "PROCEED.";

        qDebug() << clientName << "trying to connect, used = "
                 << server->isContestantOnline(clientName);
        if(server->isContestantOnline(clientName)) {
            qDebug() << "name used! bye...";
            writeCmd("ERR_NAME_USED");
            emit collectionFailed(name);
            doDisconnect();
            return;
        } else if(clientName.size() == 0) {
            writeCmd("ERR_INVALID_NAME");
            qDebug() << "Empty Name!";
            emit collectionFailed(name);
            doDisconnect();
            return;
        } else {
            writeCmd("OK_CONNECTION_ESTABLISHED");
            name = clientName;
            qDebug() <<(QString("ok. connection established for ") + name);
            QDir(Settings::sourcePath()).mkdir( name);
            server->appendOnline(name);
        }
    } else if(cmd == "CODE_PAYLOAD") {
        if(this->name.size() == 0) {
            writeCmd("ERR_NOT_REGISTERED");
            emit collectionFailed(name);
            doDisconnect();
            return;
        } else {
            qint64 size;
            QByteArray buf;
            qDebug() << "receiving file...";

            (*readStream) >> size >> buf;
            if(readStream->status() != QDataStream::Ok) return;

            // SIZE: >0 => total size; -1 => fragment
            if(size != -1) {
                totSize = size;
                tmpZip.setFileTemplate(QDir::toNativeSeparators(QDir::tempPath())
                                       + QDir::separator()
                                       + "LemonPlus_ContestantCode_XXXXXX.zip");
                tmpZip.open();
            }

            tmpZip.write(buf);

            if (tmpZip.size() == totSize) {
                tmpZip.seek(0);
                JlCompress::extractDir(&tmpZip, Settings::sourcePath() + this->name);
                qDebug() << "extracted.";
                writeCmd("OK_GOT_CODE");
                tmpZip.resize(0);
                tmpZip.close();
                emit finishCollecting(name);
            }
            return;
        }
    } else {
        writeCmd("ERR_INVALID_CMD");
        emit collectionFailed(name);
        doDisconnect();
        return;
    }
}

void ContestServerWorker::onTcpError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    emit collectionFailed(name);
    tmpZip.close();
    doDisconnect();
}
