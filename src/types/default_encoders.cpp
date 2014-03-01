#include "password.hpp"
#include <nettle/base16.h>

string
hex_encode(uint8_t *data, size_t data_len) {
    uint8_t *ascii_hash = new uint8_t[data_len * 2];
    base16_encode_update(
        ascii_hash,
        data_len,
        data
    );

    string hexed{(const char *)ascii_hash, data_len * 2};
    delete [] ascii_hash;

    return hexed;
}


string
ascii85_encode(uint8_t *data, size_t data_len) {
    size_t position = 0;
    uint8_t *ascii85  = new uint8_t[data_len + (data_len / 4)];

    for(size_t i = 0; i < data_len; i += 4) {
        /* Get 4 bytes from input data to be encoded as ascii85. */
        uint32_t piece =
            data[i + 0] << 24 |
            data[i + 1] << 16 |
            data[i + 2] << 8  |
            data[i + 3];

        /* Convert into 5 character, and move position 5 bytes along buffer for
         * the next round of character. */
        for(size_t j = 5; j > 0; --j) {
            ascii85[position + j - 1] = (piece % 85 + 33);
            piece /= 85;
        }

        position += 5;
    }

    string ascii85ed{(const char *)ascii85, data_len + (data_len / 4)};
    delete [] ascii85;

    return ascii85ed;
}
