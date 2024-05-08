#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../serial/serial.h"
#include <QTimer>

#define PLOT1_DISPLAY_DATASET_SIZE 1000
#define PLOT2_DISPLAY_DATASET_SIZE 100

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Initialize the plots */
    init_plots();

    /* Create a MyData object for parsing incoming serial data */
    my_data = new MyData();

    /* Create a timer for periodic processing */
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(periodic_processing()));
    timer->start(100); // 100ms interval

}

MainWindow::~MainWindow()
{
    delete ui;
    delete my_data;
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

void MainWindow::periodic_processing()
{
    my_data_point_t data_point;
    static uint32_t noise_count = 0;

    my_data->parse_data();

    // Do something with the data
    // Dump it to the terminal
    while (my_data->get_data_point(&data_point))
    {
        // Update the plots with the new data point
        update_plots(&data_point);
        qDebug() << "Data point: " << data_point.data_type << " " << data_point.data;
    }
}

void MainWindow::init_plots()
{
    // Create the plots
    plot1 = new QCustomPlot();
    plot2 = new QCustomPlot();

    // Create layouts
    QVBoxLayout *layout1 = new QVBoxLayout(ui->frame_data1);
    QVBoxLayout *layout2 = new QVBoxLayout(ui->frame_data2);

    // Add the plots to the layouts
    layout1->addWidget(plot1);
    layout2->addWidget(plot2);

    // Set up the plot properties
    plot1->addGraph();
    plot1->xAxis->setLabel("Samples");
    plot1->yAxis->setLabel("Value");
    plot1->xAxis->setRange(0, PLOT1_DISPLAY_DATASET_SIZE);
    plot1->yAxis->setRange(0, 100);

    plot2->addGraph();
    plot2->xAxis->setLabel("Samples");
    plot2->yAxis->setLabel("Value");
    plot2->xAxis->setRange(0, PLOT2_DISPLAY_DATASET_SIZE);
    plot2->yAxis->setRange(0, 100);


    plot1->xAxis->setRangeLower(0);
    plot2->xAxis->setRangeLower(0);

}

void MainWindow::update_plots(my_data_point_t *data_point)
{
    // Update the plots with the new data point
    if (data_point->data_type == DATA_TYPE_1)
    {
        plot1_xdata.append((double)data_point->sample_count);
        plot1_ydata.append((double)data_point->data);

        /* Keep the total dataset size limited to a fixed number of your choosing */
        if (plot1_xdata.size() > PLOT1_DISPLAY_DATASET_SIZE)
        {
            plot1_xdata.removeFirst();
            plot1_ydata.removeFirst();
        }

        plot1->graph(0)->setData(plot1_xdata, plot1_ydata, true);

        // Autoscale the vertical axis to fit the data
        plot1->graph(0)->rescaleValueAxis();
        plot1->yAxis->setRangeLower(qMax(50.0, plot1->yAxis->range().lower));

        // Adjust the x-axis range to only show the last X samples
        if (data_point->sample_count > PLOT1_DISPLAY_DATASET_SIZE) {
            plot1->xAxis->setRange(data_point->sample_count - PLOT1_DISPLAY_DATASET_SIZE + 1, data_point->sample_count);
        }
        else {
            plot1->xAxis->setRange(0, PLOT1_DISPLAY_DATASET_SIZE);
        }

        plot1->replot();

    }
    else if (data_point->data_type == DATA_TYPE_2)
    {
        plot2_xdata.append((double)data_point->sample_count);
        plot2_ydata.append((double)data_point->data);

        /* Keep the total dataset size limited to a fixed number of your choosing */
        if (plot2_xdata.size() > PLOT2_DISPLAY_DATASET_SIZE)
        {
            plot2_xdata.removeFirst();
            plot2_ydata.removeFirst();
        }

        plot2->graph(0)->setData(plot2_xdata, plot2_ydata, true);

        // Autoscale the vertical axis to fit the data
        plot2->graph(0)->rescaleValueAxis();
        plot2->yAxis->setRangeLower(qMax(50.0, plot2->yAxis->range().lower));

        // Adjust the x-axis range to only show the last X samples
        if (data_point->sample_count > PLOT2_DISPLAY_DATASET_SIZE) {
            plot2->xAxis->setRange(data_point->sample_count - PLOT2_DISPLAY_DATASET_SIZE + 1, data_point->sample_count);
        }
        else {
            plot2->xAxis->setRange(0, PLOT2_DISPLAY_DATASET_SIZE);
        }

        plot2->replot();
    }
}



