#ifndef SERIAL_H
#define SERIAL_H

#include "../buffer/ringbuffer.h"
#include <QStringList>
#include <QByteArray>
#include <QObject>
#include <QTimer>
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

    /**
     * Returns the data in the buffer.
     *
     * @param data A pointer to a char array where the data will be stored.
     * @return true if the data was successfully retrieved, false otherwise.
     */
    bool get_data(char *data);


    /**
     * Sends data over the serial connection.
     *
     * @param data The data to be sent.
     * @param len The length of the data.
     * @return True if the data was sent successfully, false otherwise.
     */
    bool send_data(const char *data, int len);

    void dump();

private:
    // Make the constructor private
    Serial();
    ~Serial();

    void read_data();


    // Static member variable to hold the singleton instance
    static Serial* instance;

    QSerialPort *serialPort;

    // // Buffer for incoming data
    // QByteArray *buffer;

    QTimer *read_timer;

    RingBuffer<char> *rxBuffer;
};

#endif // SERIAL_H
