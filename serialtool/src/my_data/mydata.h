#ifndef MYDATA_H
#define MYDATA_H

#include "../buffer/ringbuffer.h"

/**
 * @brief The type of data that can be transmitted or received through an interface
 */
typedef enum {
    DATA_TYPE_1, /**< Data type 1 */
    DATA_TYPE_2  /**< Data type 2 */
} my_data_type_t;


/**
 * @brief Represents a data point
 * 
 * This struct contains the data value and the data type of a single data point
 * that can be transmitted or received through an interface.
 */
typedef struct {
    uint32_t data;
    uint32_t sample_count;
    my_data_type_t data_type;
} my_data_point_t;

class MyData
{

public:
    MyData();

    ~MyData();

    /**
     * Parses any available data from the serial buffer.
     *
     * valid packets added to buffer of data points. Call periodically to parse data.
     */
    void parse_data();


    /**
     * Retrieves a data point.
     *
     * This function retrieves a data point and stores it in the provided `data_point` parameter.
     *
     * @param data_point A pointer to a `my_data_point_t` structure where the retrieved data point will be stored.
     * @return `true` if the data point was successfully retrieved, `false` otherwise.
     */
    bool get_data_point(my_data_point_t *data_point);

private:

    RingBuffer<my_data_point_t> *data_points;

};

#endif // MYDATA_H
