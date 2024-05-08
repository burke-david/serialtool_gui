#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <QQueue>

template <typename T>
class RingBuffer : public QQueue<T>
{
public:
    explicit RingBuffer(int size) : maxSize(size) {}

    void append(const T &value) {
        if (QQueue<T>::count() == maxSize) {
            QQueue<T>::dequeue(); // remove the oldest element if buffer is full
        }
        QQueue<T>::enqueue(value); // add the new element
    }

private:
    int maxSize;
};

#endif // RINGBUFFER_H