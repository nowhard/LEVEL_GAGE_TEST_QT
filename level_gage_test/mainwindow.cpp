#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"

#include <QMessageBox>
#include <QDebug>
#include <QThread>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings = new SettingsDialog;

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    RtuModbus=new QRtuModbus();
    RegRequestTimer=new QTimer;

    connect(RegRequestTimer,SIGNAL(timeout()),this,SLOT(RegRequest()));

//    QThread *RtuModbusThread=new QThread(this);

//    RtuModbus->moveToThread(RtuModbusThread);
//    connect(RtuModbusThread, SIGNAL(started()), RtuModbus, SLOT(processRtuModbus()));
//    connect(RtuModbusThread, SIGNAL(finished()), RtuModbusThread, SLOT(deleteLater()));

//    RtuModbusThread->start();

    initActionsConnections();

    errorData<<"2111"<<"2222"<<"2333";
    errorListModel.setStringList(errorData);
    ui->listError->setModel(&errorListModel);

    ui->plot->addGraph();

    ui->plot->xAxis->setLabel("отсчеты");
    ui->plot->yAxis->setLabel("значение");
    ui->plot->xAxis->setRange(0, 200);
    ui->plot->yAxis->setRange(0, 4095);

    errorData<<"lolo";
    errorListModel.setStringList(errorData);
//    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
//            SLOT(handleError(QSerialPort::SerialPortError)));

}


MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}


void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();

    if (RtuModbus->open(p.name,p.baudRate,p.dataBits,p.stopBits,p.parity,p.flowControl))
    {
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);
            ui->statusBar->showMessage(tr("Подключено к %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    }
    else
    {
        QMessageBox::critical(this, tr("Ошибка"),"Произошла ошибка открытия порта");

        ui->statusBar->showMessage(tr("Ошибка открытия порта"));
    }
}


void MainWindow::closeSerialPort()
{
    RtuModbus->close();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("Отключено"));
}


void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
}

void MainWindow::on_getRtuRegister_clicked()
{
    if(RtuModbus->isOpen())
    {
        if(RegRequestTimer->isActive())
        {
            RegRequestTimer->stop();
            ui->getRtuRegister->setText(tr("Начать опрос"));
        }
        else
        {
            RegRequestTimer->setInterval(500);
            RegRequestTimer->start();
            ui->getRtuRegister->setText(tr("Завершить опрос"));
        }
    }
}


 void MainWindow::RegRequest()
 {
     QList<quint16> response;

     if(RtuModbus->isOpen())
     {
         response=RtuModbus->readInputRegisters(0xA,0x0,3,NULL);

         if(response.length()==3)
         {
             this->ui->pot_1->setText(QString::number(response[0]));
             this->ui->pot_2->setText(QString::number(response[1]));
             this->ui->sensor->setText(QString::number(response[2]));

             if(y.length()<GRAPH_LENGTH)
             {
                   y.append((double)response[2]);
                   x.append(x.length());
             }
             else
             {
                 y.pop_front();
                 y.append((double)response[2]);
             }

             ui->plot->graph(0)->setData(x,y);
             ui->plot->replot();

         }
         else
         {
            ui->statusBar->showMessage(tr("Ошибка протокола"));
         }

     }
 }
