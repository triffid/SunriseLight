
/* This file was automatically generated by nrfutil on 2018-07-09 (YY-MM-DD) at 17:21:48 */

#include "sdk_config.h"
#include "stdint.h"
#include "compiler_abstraction.h"

#if NRF_CRYPTO_BACKEND_OBERON_ENABLED
/* Oberon backend is changing endianness thus public key must be kept in RAM. */
#define _PK_CONST
#else
#define _PK_CONST const
#endif


/** @brief Public key used to verify DFU images */
__ALIGN(4) _PK_CONST uint8_t pk[64] =
{
    0x7b, 0xa6, 0xf7, 0x72, 0xb2, 0x20, 0xaf, 0xc0, 0xcf, 0xcb, 0xc3, 0x22, 0x21, 0xc2, 0x27, 0x04, 0xf5, 0xdd, 0x8a, 0xa4, 0x45, 0xc4, 0x26, 0x66, 0xf2, 0xf3, 0x95, 0x27, 0x3b, 0xf5, 0x67, 0xda, 
    0x51, 0x92, 0xef, 0xe9, 0x4b, 0xcd, 0x70, 0x9f, 0x77, 0x19, 0xe1, 0xc3, 0x81, 0xe6, 0x22, 0x2f, 0x0d, 0x2f, 0xec, 0xa3, 0x68, 0x7a, 0x6e, 0xd3, 0x35, 0x1e, 0xd4, 0xec, 0x0c, 0x3b, 0xb3, 0xcf
};
