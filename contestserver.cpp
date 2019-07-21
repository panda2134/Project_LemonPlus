#include "contestserver.h"
#include "contestserverworker.h"

#include <QDebug>

ContestServer::ContestServer(QObject *parent) : QTcpServer(parent)
{
    this->setMaxPendingConnections(300);
}

ContestServer::ContestServer(QObject *parent, Contest *contest) : QTcpServer(parent)
{
    this->contest = contest;
    this->setMaxPendingConnections(300);
    connect(this, SIGNAL(collectCodeSignal()), this, SLOT(collectCode()));
}

Contest *ContestServer::getContest() const
{
    return contest;
}

void ContestServer::setContest(Contest *value)
{
    contest = value;
}

void ContestServer::incomingConnection(qintptr socketDescriptor)
{
    ContestServerWorker *worker = new ContestServerWorker(nullptr, socketDescriptor);
    worker->setServer(this);
    worker->moveToThread(workerThread);

    connect(workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));

    connect(this, SIGNAL(sendCollectRequest()), worker, SLOT(doCollect()));
    connect(this, SIGNAL(stopAllClients()), worker, SLOT(doDisconnect()));
    connect(worker, SIGNAL(startCollecting(QString)), this, SLOT(startCollecting(QString)));
    connect(worker, SIGNAL(finishCollecting(QString)), this, SLOT(finishCollecting(QString)));
    connect(worker, SIGNAL(collectionFailed(QString)), this, SLOT(appendFailure(QString)));

    qDebug() << "incoming connection! starting...";
    worker->startWorker();
    qDebug() << "incoming connection! started.";
}

void ContestServer::startCollecting(const QString &name)
{
    emit showStartCollecting(name);
}

void ContestServer::finishCollecting(const QString &name)
{
    emit showFinishCollecting(name);
}

void ContestServer::appendFailure(const QString &name)
{
    failure.insert(name);
}

void ContestServer::appendOnline(const QString &name) {
    onlineContestant.insert(name);
}

void ContestServer::removeOnline(const QString &name)
{
    if(isContestantOnline(name))
        onlineContestant.erase(onlineContestant.find(name));
}

bool ContestServer::isContestantOnline(const QString &name) {
    return onlineContestant.find(name) != onlineContestant.end();
}

int ContestServer::onlineCount() const
{
    return onlineContestant.count();
}

void ContestServer::collectCode() {
    qDebug() << "collecting code...";
    failure.clear();
    emit sendCollectRequest();
}

void ContestServer::startServer()
{
    workerThread = new QThread();
    workerThread->start();
    this->listen(QHostAddress::Any, 53667);
}

void ContestServer::stopServer()
{
    if(this->onlineCount() != 0)
        emit stopAllClients();
    this->close();
    this->thread()->quit();
}
