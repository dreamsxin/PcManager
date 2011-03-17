#ifndef GNUMERIC_EXCEL_RC4_H
#define GNUMERIC_EXCEL_RC4_H

/* rc4.h */
typedef struct {
    unsigned char state[256];
    unsigned char x;
    unsigned char y;
} RC4_KEY;
void prepare_key (unsigned char *key_data_ptr, int key_data_len, RC4_KEY *key);
void rc4 (unsigned char *buffer_ptr, unsigned buffer_len, RC4_KEY *key);

inline void
swap_byte (unsigned char *a, unsigned char *b)
{
    unsigned char swapByte;

    swapByte = *a;
    *a = *b;
    *b = swapByte;
}
inline
void
prepare_key (unsigned char *key_data_ptr, int key_data_len, RC4_KEY *key)
{
    unsigned char index1;
    unsigned char index2;
    unsigned char *state;
    unsigned int counter;

    state = &key->state[0];
    for (counter = 0; counter < 256; counter++)
        state[counter] = counter;
    key->x = 0;
    key->y = 0;
    index1 = 0;
    index2 = 0;
    for (counter = 0; counter < 256; counter++)
    {
        index2 = (key_data_ptr[index1] + state[counter] + index2) % 256;
        swap_byte (&state[counter], &state[index2]);

        index1 = (index1 + 1) % key_data_len;
    }
}
inline
void
rc4 (unsigned char *buffer_ptr, unsigned buffer_len, RC4_KEY *key)
{
    unsigned char x;
    unsigned char y;
    unsigned char *state;
    unsigned char xorIndex;
    unsigned int counter;

    x = key->x;
    y = key->y;

    state = key->state;
    for (counter = 0; counter < buffer_len; counter++)
    {
        x = (x + 1) % 256;
        y = (state[x] + y) % 256;
        swap_byte (&state[x], &state[y]);

        xorIndex = (state[x] + state[y]) % 256;

        buffer_ptr[counter] ^= state[xorIndex];
    }
    key->x = x;
    key->y = y;
}



#endif

