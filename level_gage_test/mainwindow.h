/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include "serial/serial.h"
#include "settingsdialog.h"
#include "qmodbus/qrtumodbus.h"
#include <QTimer>
#include <QListView>
#include <QStringListModel>
#include <stdint.h>

#define GRAPH_LENGTH        1001
#define REQUEST_INTERVAL    125
#define MODBUS_REG_NUM      4

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();

    void on_getRtuRegister_clicked();

    void RegRequest();

private:
    void initActionsConnections();
    void addGraphPoint(uint16_t value);
    void clearGraph();

private:
    Ui::MainWindow *ui;
    Serial *serial;
    QRtuModbus *RtuModbus;
    SettingsDialog *settings;
    QTimer *RegRequestTimer;

    QStringList errorData;
    QStringListModel errorListModel;

    QVector<double> x, y;

    uint16_t level_meter_test_state_1,level_meter_test_state_2;

    uint16_t level_meter_value;
    uint16_t speed_manual;
    uint16_t speed_cycle;

    enum
    {
        LEVEL_METER_TEST_STOP=0,
        LEVEL_METER_TEST_MANUAL_UP,
        LEVEL_METER_TEST_MANUAL_DOWN,
        LEVEL_METER_TEST_CYCLE_UP,
        LEVEL_METER_TEST_CYCLE_DOWN,
        LEVEL_METER_TEST_CYCLE_PAUSE,
    };

    enum
    {
        REG_LEVEL=0,
        REG_SPEED_MANUAL,
        REG_SPEED_CYCLE,
        REG_STATE,
    };
};

#endif // MAINWINDOW_H
