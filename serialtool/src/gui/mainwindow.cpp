#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../serial/serial.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_find_serialports_clicked()
{
    QStringList serialPorts = Serial::getInstance().getSerialPorts();
    ui->comboBox_serialports->clear();
    ui->comboBox_serialports->addItems(serialPorts);
}


void MainWindow::on_pushButton_connect_port_clicked()
{
    // If the button reads "Disconnect" when the button is pressed, then disconnect the serial port
    if (ui->pushButton_connect_port->text() == "Disconnect")
    {
        Serial::getInstance().disconnectSerialPort();
        ui->comboBox_serialports->setEnabled(true);
        ui->pushButton_connect_port->setText("Connect Device");
        ui->pushButton_find_serialports->setEnabled(true);
        ui->statusbar->showMessage("Disconnected");
        return;
    }
    else
    {
        // If the button reads "Connect Device", then the serial port should be connected (if possible)
        QString portName = ui->comboBox_serialports->currentText();
        if (Serial::getInstance().connectSerialPort(portName))
        {
            // place a message on the QStatusBar of the MainWindow indicating that the connection was successful
            ui->statusbar->showMessage("Connected to " + portName);

            // Don't allow changes to the comboBox while the serial port is connected
            ui->comboBox_serialports->setEnabled(false);
            ui->pushButton_connect_port->setText("Disconnect");
            ui->pushButton_find_serialports->setEnabled(false);

        }
        else
        {
            ui->statusbar->showMessage("Failed to connect to " + portName);
        }

    }

}


void MainWindow::on_pushButton_dump_clicked()
{
    Serial::getInstance().dump();
}



