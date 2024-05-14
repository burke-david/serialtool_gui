#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include "../my_data/mydata.h"
// #include "../plot/qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_find_serialports_clicked();

    void on_pushButton_connect_port_clicked();

    void on_pushButton_dump_clicked();

    void periodic_processing();


private:

    struct Extents {
        double minX;
        double maxX;
        double minY;
        double maxY;
    };
    
    Ui::MainWindow *ui;

    void init_test_frame();

    /**
     * @brief Initializes the plots in the main window.
     * 
     * This function is responsible for initializing the plots used in the main window of the GUI.
     * It sets up the necessary configurations and initializes the data structures required for plotting.
     * 
     * @note This function should be called before displaying the main window.
     */
    void init_plots();

    /**
     * @brief Updates the plots with the provided data point.
     * 
     * This function is responsible for updating the plots with the provided data point.
     * It adds the data point to the plot and updates the plot's display.
     * 
     * @param data_point The data point to be added to the plots.
     */
    void update_plots(my_data_point_t *data_point);

    void displayDataPoint(const my_data_point_t& dataPoint);

    bool calculateExtents(const QList<QPointF>& points, Extents& extents);

    /**
     * @brief The QCustomPlot object used for plotting data.
     */
    // QCustomPlot *plot1;

    /**
     * @brief The QCustomPlot object used for plotting data.
     */
    // QCustomPlot *plot2;

    QLineSeries *series1;
    QLineSeries *series2;

    QChart *chart1;
    QChart *chart2;

    QChartView *chartView1;
    QChartView *chartView2;

    QVector<double> plot1_xdata;
    QVector<double> plot1_ydata;

    QVector<double> plot2_xdata;
    QVector<double> plot2_ydata;

    /* An object that handles parsing incoming data from a serial port and storing it in the format I want */
    MyData *my_data;
};
#endif // MAINWINDOW_H
