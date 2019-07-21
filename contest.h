/***************************************************************************
    This file is part of Project Lemon
    Copyright (C) 2011 Zhipeng Jia

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/
/**
 * contest.h @Project Lemon+
 * Update 2018 Dust1404
 **/

#ifndef CONTEST_H
#define CONTEST_H

#include <QtCore>
#include <QObject>
#include <QThread>
#include "globaltype.h"
#define MagicNumber 0x20111127

class Task;
class Settings;
class Contestant;
class ContestServer;

class Contest : public QObject
{
    Q_OBJECT
public:
    explicit Contest(QObject *parent = 0);
    ContestServer *server;
    QThread *serverThread;
    void setSettings(Settings*);
    void setContestTitle(const QString&);
    const QString& getContestTitle() const;
    Task* getTask(int) const;
    const QList<Task*>& getTaskList() const;
    Contestant* getContestant(const QString&) const;
    QList<Contestant*> getContestantList() const;
    int getTotalTimeLimit() const;
    void addTask(Task*);
    void deleteTask(int);
    void refreshContestantList();
    void deleteContestant(const QString&);
    void writeToStream(QDataStream&);
    void readFromStream(QDataStream&);

private:
    QString contestTitle;
    Settings *settings;
    QList<Task*> taskList;
    QMap<QString, Contestant*> contestantList;
    bool stopJudging;
    void judge(Contestant*);
    void judge(Contestant*, int);
    void clearPath(const QString&);

public slots:
    void judge(const QString&);
    void judge(const QString&, int);
    void judgeAll();
    void judgeSingleTask(int);
    void stopJudgingSlot();

signals:
    void taskAddedForContestant();
    void taskDeletedForContestant(int);
    void taskAddedForViewer();
    void taskDeletedForViewer(int);
    void problemTitleChanged();
    void singleCaseFinished(int, int, int, int);
    void singleSubtaskDependenceFinished(int, int, double);
    void taskJudgingStarted(QString);
    void taskJudgingFinished();
    void contestantJudgingStart(QString);
    void contestantJudgingFinished();
    void compileError(int, int);
    void stopJudgingSignal();
    void stopServerSignal();
};

#endif // CONTEST_H
