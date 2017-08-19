#ifndef CHIP8_DECODER_H
#define CHIP8_DECODER_H

/*
    Provides convienent access to different parts of the opcode
    the first nibble is always the instruction to be performed,
    but the other 3 nibbles can be divided the following ways:

    INNN
    IXKK
    IXYJ 
*/
typedef union {
    uint16_t bits : 16;
    struct
    {
        uint8_t I : 4;
        union {
            uint16_t NNN : 12;
            struct
            {
                uint8_t X : 4;
                union {
                    uint8_t KK : 8;
                    struct
                    {
                        uint8_t Y : 4;
                        uint8_t J : 4;
                    };
                };
            };
        };
    };
} opcode_t;

#endif