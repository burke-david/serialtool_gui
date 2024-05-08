#include "mydata.h"
#include "../serial/serial.h"

#define MY_DATA_RX_BUFFER_SIZE 1024U

MyData::MyData()
{
    data_points = new RingBuffer<my_data_point_t>(MY_DATA_RX_BUFFER_SIZE);
}

MyData::~MyData()
{
    delete data_points;
}

typedef enum {
    E_LOOK_FOR_HEADER,
    E_LOOK_FOR_DATA,
    E_LOOK_FOR_FOOTER
} my_data_parsing_state_t;
static my_data_parsing_state_t parsing_state = E_LOOK_FOR_HEADER;

/**
 * This is an example of a function that parses data from the serial buffer.
 * 
 * In this case, the data packets to be parsed are in the following format:
 * 
 * byte 0: header (0x5A - DATA_TYPE_1, 0x7A - DATA_TYPE_2)
 * byte 1: data (MSB) for a uint32_t value
 * byte 2: data byte part of the uint32_t value 
 * byte 3: data byte part of the uint32_t value
 * byte 4: data (LSB) for a uint32_t value
 * byte 5: footer (0x00)
 * 
 * A better version of this would be to use an 8-bit CRC instead of 0x00 as the footer.
 * 
 * The function reads bytes from the serial buffer and parses them into my_data_point_t structures,
 * then stuffs them into a RingBuffer called data_points.
*/
void MyData::parse_data()
{
    static char data[6u];
    /* An index for keeping track of which byte of the 4-byte uint32_t data point we're reading */
    static int data_index = 1;
    static uint32_t type_1_sample_count = 0;
    static uint32_t type_2_sample_count = 0;
    char byte;


    while (Serial::getInstance().get_data(&byte)) {
        switch (parsing_state)
        {
            case E_LOOK_FOR_HEADER:
                // Look for the header
                // If we're here, we're looking for either 0x5A or 0x7A
                if (byte == 0x5A || byte == 0x7A)
                {
                    data[0] = byte;
                    parsing_state = E_LOOK_FOR_DATA;
                    data_index = 1;
                }
                else {
                    // If we're here, we're looking for the header
                    // If we don't find it, we're not interested in the data
                    // so we can just ignore it
                }


                break;
            case E_LOOK_FOR_DATA:
                // Look for the data
                // If we're here, we're looking for the data bytes
                // We need to read 4 bytes
                // The first byte is the MSB of the data
                // The second byte is the second byte of the data
                // The third byte is the third byte of the data
                // The fourth byte is the LSB of the data
                data[data_index] = byte;
                data_index++;
                if (data_index == 5)
                {
                    // We've read all the data bytes
                    // Now we need to read the footer
                    parsing_state = E_LOOK_FOR_FOOTER;
                }

                break;
            case E_LOOK_FOR_FOOTER:
                // Look for the footer
                // If we're here, we're looking for the footer
                // The footer is 0x00
                if (byte == 0x00)
                {
                    // We've found the footer
                    // Now we can create a data point and add it to the data_points buffer
                    my_data_point_t data_point;
                    data_point.data_type = (data[0] == 0x5A) ? DATA_TYPE_1 : DATA_TYPE_2;
                    data_point.sample_count = (data[0] == 0x5A) ? type_1_sample_count++ : type_2_sample_count++;
                    data_point.data = ((uint32_t)data[1] & 0xFF) << 24 | ((uint32_t)data[2] & 0xFF) << 16 | ((uint32_t)data[3] & 0xFF) << 8 | ((uint32_t)data[4] & 0xFF);
                    data_points->enqueue(data_point);
                    parsing_state = E_LOOK_FOR_HEADER;
                }
                else {
                    // If we're here, we're looking for the footer
                    // If we don't find it, we're not interested in the data
                    // so we can just ignore it
                    parsing_state = E_LOOK_FOR_HEADER;
                }
                break;
        }
    }


}

bool MyData::get_data_point(my_data_point_t *data_point)
{
    // Retrieve a data point
    if (data_points->size() == 0)
    {
        return false;
    }
    else {
        *data_point = data_points->dequeue();
    }
    return true;
}