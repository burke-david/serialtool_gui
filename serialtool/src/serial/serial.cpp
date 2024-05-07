#include "serial.h"
#include <QDebug>
#include <QTimer>
#include <QtSerialPort/QSerialPortInfo>



class SerialReader : public QObject
{
    Q_OBJECT

public slots:
    void read_data()
    {
        while(serialPort->isOpen())
        {
            if(serialPort->bytesAvailable())
            {
                if(mutex->tryLock())
                {
                    buffer->append(serialPort->readAll());
                    mutex->unlock();
                }
            }

            // if (serialPort->waitForReadyRead(1000))
            // {
            //     if(mutex->tryLock())
            //     {
            //         buffer->append(serialPort->readAll());
            //         mutex->unlock();
            //     }
            // }
        }
    }
public:
    SerialReader(QSerialPort* serialPort, QByteArray* buffer, QMutex* mutex)
        : serialPort(serialPort), buffer(buffer), mutex(mutex) {}

private:
    QSerialPort* serialPort;
    QByteArray* buffer;
    QMutex* mutex;
};

#include "serial.moc"

// Initialize the static member variable
Serial* Serial::instance = nullptr;

SerialReader *reader = nullptr;


Serial::Serial()
{
    // Initialize your Serial object here
    mutex = new QMutex();
    buffer = new QByteArray();
    serialPort = new QSerialPort();
}

Serial::~Serial()
{
    // Clean up your Serial object here
    readerThread.quit();
    readerThread.wait();
    delete mutex;
    delete buffer;
    delete serialPort;
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


    // reader = new SerialReader(serialPort, buffer, mutex);
    // reader->moveToThread(&readerThread);
    // connect(&readerThread, &QThread::finished, reader, &QObject::deleteLater);
    // connect(this, &Serial::start_reading, reader, &SerialReader::read_data);
    // readerThread.start();

    // emit start_reading();


    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Serial::read_data);
    timer->start(10);

    return true; // Successfully opened the port
}

void Serial::read_data()
{
    if(serialPort->bytesAvailable())
    {
        buffer->append(serialPort->readAll());

        qDebug() << *buffer;
        buffer->clear();
    }

}

void Serial::disconnectSerialPort()
{
    // Close the serial port
    serialPort->close();
}

/**
 * Dumps the contents of the buffer to the console.
 */
void Serial::dump()
{

    // if(buffer->isNull())
    // {
    //     qDebug() << "Buffer is null";
    //     return;
    // }

    // if (mutex->tryLock()) {
    //     /* read data out of the buffer, copy it to another buffer, then dump that to the terminal */
    //     if (buffer->size() > 0){
            
    //         QByteArray copyBuffer = *buffer;
    //         buffer->clear();
    //         qDebug() << copyBuffer;
    //     }
    //     mutex->unlock();
    // }

    if (buffer->size() > 0){
        qDebug() << *buffer;
        buffer->clear();
    }

}
