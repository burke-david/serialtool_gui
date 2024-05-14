#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../serial/serial.h"
#include <QTimer>
#include <QtCharts/QValueAxis>
#include <QRandomGenerator>

#define PLOT1_DISPLAY_DATASET_SIZE 100
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

    my_data->parse_data();

    // Do something with the data
    // Dump it to the terminal
    while (my_data->get_data_point(&data_point))
    {
        // Update the plots with the new data point
        update_plots(&data_point);
        displayDataPoint(data_point);
        // qDebug() << "Data point: " << data_point.data_type << " " << data_point.data;
    }

    // For debugging purposes while there is no data coming in
    // for create a sine wave with an amplitude of 50 and a frequency of 2 Hz that updates every 100 ms
    // while this periodic function is called, creates a data_point with the sine wave value and 
    // calls the update_plots method to pass data to the DATA_TYPE_1 plot.
    static int sample_count = 0;
    data_point.sample_count = sample_count++;
    data_point.data_type = DATA_TYPE_1;
    data_point.data = 50 * sin(2 * 3.14159 * sample_count / 20);
    update_plots(&data_point);
    displayDataPoint(data_point);

}


void MainWindow::init_plots()
{
    // Initialize the data structures for the plots
    series1 = new QLineSeries();
    series2 = new QLineSeries();

    // Create the charts
    chart1 = new QChart();
    chart2 = new QChart();

    // Add 1000 points to series 1 as a sine wave with amplitude 25 and a vertical offset of 25 such that
    // the lowest value on the y-axis is 0 and the highest value is 50. The frequency of the sine wave is 2 Hz
    // and each sample represents 25ms of time.
    // for (int i = 0; i < PLOT1_DISPLAY_DATASET_SIZE; i++)
    // {
    //     series1->append(i, 25 * sin(2 * 3.14159 * i / 50) + 25);
    // }

    // Add the series to the charts
    chart1->addSeries(series1);
    chart2->addSeries(series2);

    chart1->legend()->hide();
    chart2->legend()->hide();

    // Create the axes
    QValueAxis *axisX1 = new QValueAxis();
    QValueAxis *axisY1 = new QValueAxis();
    QValueAxis *axisX2 = new QValueAxis();
    QValueAxis *axisY2 = new QValueAxis();

    // Set the axis ranges
    axisX1->setRange(0, PLOT1_DISPLAY_DATASET_SIZE);
    axisY1->setRange(0, 100);
    axisX2->setRange(0, PLOT2_DISPLAY_DATASET_SIZE);
    axisY2->setRange(0, 100);

    // Add the axes to the chart
    chart1->addAxis(axisX1, Qt::AlignBottom);
    chart1->addAxis(axisY1, Qt::AlignLeft);
    chart2->addAxis(axisX2, Qt::AlignBottom);
    chart2->addAxis(axisY2, Qt::AlignLeft);

    // Attach the axes to the series
    series1->attachAxis(axisX1);
    series1->attachAxis(axisY1);
    series2->attachAxis(axisX2);
    series2->attachAxis(axisY2);

    // Create the chart views
    chartView1 = new QChartView(chart1);
    chartView2 = new QChartView(chart2);

    chartView1->setRenderHint(QPainter::Antialiasing);
    chartView2->setRenderHint(QPainter::Antialiasing);

    // Create layouts
    QVBoxLayout *layout1 = new QVBoxLayout(ui->frame_data1);
    QVBoxLayout *layout2 = new QVBoxLayout(ui->frame_data2);

    // Add the chart views to the layouts
    layout1->addWidget(chartView1);
    layout2->addWidget(chartView2);

    chart1->setTitle("Data Type 1");
    chart2->setTitle("Data Type 2");

    // // Create the plots
    // plot1 = new QCustomPlot();
    // plot2 = new QCustomPlot();

    // // Create layouts
    // QVBoxLayout *layout1 = new QVBoxLayout(ui->frame_data1);
    // QVBoxLayout *layout2 = new QVBoxLayout(ui->frame_data2);

    // // Add the plots to the layouts
    // layout1->addWidget(plot1);
    // layout2->addWidget(plot2);

    // // Set up the plot properties
    // plot1->addGraph();
    // plot1->xAxis->setLabel("Samples");
    // plot1->yAxis->setLabel("Value");
    // plot1->xAxis->setRange(0, PLOT1_DISPLAY_DATASET_SIZE);
    // plot1->yAxis->setRange(0, 100);

    // plot2->addGraph();
    // plot2->xAxis->setLabel("Samples");
    // plot2->yAxis->setLabel("Value");
    // plot2->xAxis->setRange(0, PLOT2_DISPLAY_DATASET_SIZE);
    // plot2->yAxis->setRange(0, 100);


    // plot1->xAxis->setRangeLower(0);
    // plot2->xAxis->setRangeLower(0);

}

void MainWindow::update_plots(my_data_point_t *data_point)
{
    if (data_point->data_type == DATA_TYPE_1)
    {
        series1->append(data_point->sample_count, data_point->data);

        Extents range = {0, 0, 0, 0};
        if(calculateExtents(series1->pointsVector(), range))
        {
            if (range.maxX > PLOT1_DISPLAY_DATASET_SIZE)
            {
                series1->remove(0);
                chartView1->chart()->axisX()->setRange(range.maxX - PLOT1_DISPLAY_DATASET_SIZE, range.maxX);
            }
        }




        // auto axesListX = chart1->axes(Qt::Horizontal);
        // auto axesListY = chart1->axes(Qt::Vertical);
        // if (!axesListX.isEmpty()) {
        //     auto axisX = qobject_cast<QValueAxis *>(axesListX.first());
        //     if (axisX) {
        //         axisX->setMin(data_point->sample_count - PLOT1_DISPLAY_DATASET_SIZE);
        //         axisX->setMax(data_point->sample_count);
        //     }
        // }
        // if (!axesListY.isEmpty()) {
        //     auto axisY = qobject_cast<QValueAxis *>(axesListY.first());
        //     if (axisY) {
        //         axisY->setMin(0);
        //         axisY->setMax(100);
        //     }
        // }
    }
    else if (data_point->data_type == DATA_TYPE_2)
    {
        series2->append(data_point->sample_count, data_point->data);
        auto axesListX = chart2->axes(Qt::Horizontal);
        auto axesListY = chart2->axes(Qt::Vertical);
        if (!axesListX.isEmpty()) {
            auto axisX = qobject_cast<QValueAxis *>(axesListX.first());
            if (axisX) {
                axisX->setMin(data_point->sample_count - PLOT2_DISPLAY_DATASET_SIZE);
                axisX->setMax(data_point->sample_count);
            }
        }
        if (!axesListY.isEmpty()) {
            auto axisY = qobject_cast<QValueAxis *>(axesListY.first());
            if (axisY) {
                axisY->setMin(0);
                axisY->setMax(100);
            }
        }
    }


    // // Update the plots with the new data point
    // if (data_point->data_type == DATA_TYPE_1)
    // {
    //     plot1_xdata.append((double)data_point->sample_count);
    //     plot1_ydata.append((double)data_point->data);

    //     /* Keep the total dataset size limited to a fixed number of your choosing */
    //     if (plot1_xdata.size() > PLOT1_DISPLAY_DATASET_SIZE)
    //     {
    //         plot1_xdata.removeFirst();
    //         plot1_ydata.removeFirst();
    //     }

    //     plot1->graph(0)->setData(plot1_xdata, plot1_ydata, true);

    //     // Autoscale the vertical axis to fit the data
    //     plot1->graph(0)->rescaleValueAxis();
    //     plot1->yAxis->setRangeLower(qMax(50.0, plot1->yAxis->range().lower));

    //     // Adjust the x-axis range to only show the last X samples
    //     if (data_point->sample_count > PLOT1_DISPLAY_DATASET_SIZE) {
    //         plot1->xAxis->setRange(data_point->sample_count - PLOT1_DISPLAY_DATASET_SIZE + 1, data_point->sample_count);
    //     }
    //     else {
    //         plot1->xAxis->setRange(0, PLOT1_DISPLAY_DATASET_SIZE);
    //     }

    //     plot1->replot();

    // }
    // else if (data_point->data_type == DATA_TYPE_2)
    // {
    //     plot2_xdata.append((double)data_point->sample_count);
    //     plot2_ydata.append((double)data_point->data);

    //     /* Keep the total dataset size limited to a fixed number of your choosing */
    //     if (plot2_xdata.size() > PLOT2_DISPLAY_DATASET_SIZE)
    //     {
    //         plot2_xdata.removeFirst();
    //         plot2_ydata.removeFirst();
    //     }

    //     plot2->graph(0)->setData(plot2_xdata, plot2_ydata, true);

    //     // Autoscale the vertical axis to fit the data
    //     plot2->graph(0)->rescaleValueAxis();
    //     plot2->yAxis->setRangeLower(qMax(50.0, plot2->yAxis->range().lower));

    //     // Adjust the x-axis range to only show the last X samples
    //     if (data_point->sample_count > PLOT2_DISPLAY_DATASET_SIZE) {
    //         plot2->xAxis->setRange(data_point->sample_count - PLOT2_DISPLAY_DATASET_SIZE + 1, data_point->sample_count);
    //     }
    //     else {
    //         plot2->xAxis->setRange(0, PLOT2_DISPLAY_DATASET_SIZE);
    //     }

    //     plot2->replot();
    // }
}

void MainWindow::displayDataPoint(const my_data_point_t &dataPoint)
{
    QString message = QString("TYPE<%1> Data<%2> Sample<%3>")
                      .arg(dataPoint.data_type)
                      .arg(dataPoint.data)
                      .arg(dataPoint.sample_count);

    ui->textEdit->appendPlainText(message);
    
}

bool MainWindow::calculateExtents(const QList<QPointF> &points, Extents &extents)
{
    if (points.isEmpty()) {
        // Return false if the input list is empty
        return false;
    }

    extents.minX = extents.maxX = points.first().x();
    extents.minY = extents.maxY = points.first().y();

    foreach (const QPointF& point, points) {
        extents.minX = qMin(extents.minX, point.x());
        extents.maxX = qMax(extents.maxX, point.x());
        extents.minY = qMin(extents.minY, point.y());
        extents.maxY = qMax(extents.maxY, point.y());
    }

    // Return true to indicate successful calculation
    return true;

}
