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
 * generalsettings.h @Project Lemon+
 * Update 2018 Dust1404
 **/

#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <QtCore>
#include <QtGui>
#include <QWidget>

#include "settingsitem.h"

namespace Ui {
    class GeneralSettings;
}

class Settings;

class GeneralSettings : public QWidget, SettingsItem
{
    Q_OBJECT

public:
    explicit GeneralSettings(QWidget *parent = 0);
    ~GeneralSettings();
    void resetEditSettings(Settings*) override;
    bool checkValid() override;

private:
    Ui::GeneralSettings *ui;
    Settings *editSettings;

private slots:
    void defaultFullScoreChanged(const QString&);
    void defaultTimeLimitChanged(const QString&);
    void defaultMemoryLimitChanged(const QString&);
    void compileTimeLimitChanged(const QString&);
    void specialJudgeTimeLimitChanged(const QString&);
    void fileSizeLimitChanged(const QString&);
    void rejudgeTimesChanged(const QString&);
    void inputFileExtensionsChanged(const QString&);
    void outputFileExtensionsChanged(const QString&);
};

#endif // GENERALSETTINGS_H
