#ifndef CONTESTSERVER_H
#define CONTESTSERVER_H

#include "contest.h"

#include <QObject>
#include <QTcpServer>
#include <QSet>
#include <QString>
#include <QSignalMapper>

class ContestServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ContestServer(QObject *parent = nullptr);
    explicit ContestServer(QObject *parent = nullptr, Contest *contest = nullptr);

    QSet<QString> failure;

    Contest *getContest() const;
    void setContest(Contest *value);
    bool isContestantOnline(const QString &name);
    void appendOnline(const QString &);
    void removeOnline(const QString &);
    int onlineCount() const;

    QSet<QString>* getFailure() const;

signals:
    void collectCodeSignal();
    void sendCollectRequest();
    void showStartCollecting(const QString &);
    void showFinishCollecting(const QString &);
    void stopAllClients();
public slots:
    void startServer();
    void stopServer();
protected:
    void incomingConnection(qintptr socketDescriptor) override;
private:
    QThread *workerThread;
    Contest *contest = nullptr;
    QSet<QString> onlineContestant;
private slots:
    void collectCode();
    void startCollecting(const QString &);
    void finishCollecting(const QString &);
    void appendFailure(const QString &);
};

#endif // CONTESTSERVER_H
