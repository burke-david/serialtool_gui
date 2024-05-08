#include "serial.h"
#include <QDebug>

#include <QtSerialPort/QSerialPortInfo>

// #include "serial.moc"

// The size of the receive buffer. This is the maximum number of bytes that can be stored in the buffer.
#define RX_BUFFER_SIZE 1024U

// Initialize the static member variable
Serial* Serial::instance = nullptr;

Serial::Serial()
{
    // Initialize your Serial object here
    // buffer = new QByteArray();
    serialPort = new QSerialPort();
    rxBuffer = new RingBuffer<char>(RX_BUFFER_SIZE);
}

Serial::~Serial()
{
    // Clean up your Serial object here
    // delete buffer;
    delete serialPort;
    delete read_timer;
    delete rxBuffer;
}

Serial& Serial::getInstance()
{
    if (!instance)
    {
        instance = new Serial();
    }
    return *instance;
}

QStringList Serial::getSerialPorts()
{
    QStringList serialPorts;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        serialPorts.append(info.portName());
    }
    return serialPorts;
}

bool Serial::connectSerialPort(const QString& portName)
{
    // Set the port name
    serialPort->setPortName(portName);

    // Set the baud rate, data bits, parity, and stop bits
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);

    // Open the serial port in read/write mode
    if (!serialPort->open(QIODevice::ReadWrite)) {
        return false; // Failed to open the port
    }

    read_timer = new QTimer(this);
    connect(read_timer, &QTimer::timeout, this, &Serial::read_data);
    read_timer->start(10);

    return true; // Successfully opened the port
}

void Serial::read_data()
{
    if(serialPort->bytesAvailable())
    {
        QByteArray data = serialPort->readAll();
        for (int i = 0; i < data.size(); i++) {
            rxBuffer->append(data.at(i));
        }

        // buffer->append(serialPort->readAll());
        // qDebug() << *buffer;
        // buffer->clear();
    }

}

bool Serial::get_data(char *data)
{
    if (rxBuffer->size() > 0)
    {
        *data = rxBuffer->dequeue();
        return true;
    }
    return false;
}

void Serial::disconnectSerialPort()
{
    // Close the serial port
    serialPort->close();
    /* Cleanup the read_timer */
    if (read_timer != nullptr)
    {
        read_timer->stop();
        delete read_timer;
    }
}

/**
 * Dumps the contents of the buffer to the console.
 */
void Serial::dump()
{

}
