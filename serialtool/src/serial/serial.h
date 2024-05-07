#ifndef SERIAL_H
#define SERIAL_H

#include <QStringList>
#include <QMutex>
#include <QThread>
#include <QByteArray>
#include <QObject>
#include <QtSerialPort/QSerialPort>

class Serial : public QObject
{
    Q_OBJECT
    
public:
    // Delete the copy constructor and assignment operator
    Serial(const Serial&) = delete;
    Serial& operator=(const Serial&) = delete;

    /**
     * Returns the singleton instance of the Serial class.
     *
     * @return The singleton instance of the Serial class.
     */
    static Serial& getInstance();

    /**
     * Returns a QStringList containing the names of all available serial ports.
     *
     * @return A QStringList containing the names of all available serial ports.
     */
    QStringList getSerialPorts();

    /**
     * Connects to a serial port with the given name.
     *
     * @param portName The name of the serial port to connect to.
     * @return true if the connection was successful, false otherwise.
     */
    bool connectSerialPort(const QString& portName);

    /**
     * Disconnects the serial port.
     */
    void disconnectSerialPort();

    void dump();

private:
    // Make the constructor private
    Serial();
    ~Serial();

    void read_data();

    // Static member variable to hold the singleton instance
    static Serial* instance;

    QSerialPort *serialPort;

    // Buffer for incoming data
    QByteArray *buffer;

    // Mutex for thread safety
    QMutex *mutex;

    QThread readerThread;



signals:

    void start_reading();
};

#endif // SERIAL_H
