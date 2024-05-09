#ifndef CRC_H
#define CRC_H

#include <stdint.h>

class crc
{
public:
    crc();


    /**
     * @brief Checks the CRC-8 checksum of the given data.
     *
     * This function calculates the CRC-8 checksum of the specified data and compares it with the provided checksum.
     *
     * @param data Pointer to the data buffer.
     * @param len Length of the data buffer.
     * @param checksum The expected CRC-8 checksum.
     * @return True if the calculated CRC-8 checksum matches the expected checksum, false otherwise.
     */
    static bool crc8_check(const uint8_t *data, uint8_t len, uint8_t checksum);


    /**
     * Calculates the CRC-8 checksum for the given data.
     *
     * @param data The input data for which the CRC-8 checksum needs to be calculated.
     * @param len The length of the input data.
     * @return The calculated CRC-8 checksum.
     */
    static uint8_t crc8(const uint8_t *data, uint8_t len);
};

#endif // CRC_H
