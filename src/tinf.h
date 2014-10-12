/*
 * tinf  -  tiny inflate library (inflate, gzip, zlib)
 *
 * version 1.00
 *
 * Copyright (c) 2003 by Joergen Ibsen / Jibz
 * All Rights Reserved
 *
 * http://www.ibsensoftware.com/
 */

#ifndef TINF_H_INCLUDED
#define TINF_H_INCLUDED

/* calling convention */
#ifndef TINFCC
 #ifdef __WATCOMC__
  #define TINFCC __cdecl
 #else
  #define TINFCC
 #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define TINF_OK             0
#define TINF_DATA_ERROR    (-3)

/* data structures */

typedef struct {
   unsigned short table[16];  /* table of code length counts */
   unsigned short trans[288]; /* code -> symbol translation table */
} TINF_TREE;

struct TINF_DATA;
typedef struct TINF_DATA {
   const unsigned char *source;
   unsigned int tag;
   unsigned int bitcount;

    /* Buffer start */
    unsigned char *destStart;
    /* Buffer total size */
    unsigned int destSize;
    /* Current pointer in buffer */
    unsigned char *dest;
    /* Remaining bytes in buffer */
    unsigned int destRemaining;
    /* Argument is the allocation size which didn't fit into buffer. Note that
       exact mimumum size to grow buffer by is lastAlloc - destRemaining. But
       growing by this exact size is ineficient, as the next allocation will
       fail again. */
    int (*destGrow)(struct TINF_DATA *data, unsigned int lastAlloc);

   TINF_TREE ltree; /* dynamic length/symbol tree */
   TINF_TREE dtree; /* dynamic distance tree */
} TINF_DATA;

/* function prototypes */

void TINFCC tinf_init();

int TINFCC tinf_uncompress(void *dest, unsigned int *destLen,
                           const void *source, unsigned int sourceLen);

int TINFCC tinf_gzip_uncompress(void *dest, unsigned int *destLen,
                                const void *source, unsigned int sourceLen);

int TINFCC tinf_zlib_uncompress(void *dest, unsigned int *destLen,
                                const void *source, unsigned int sourceLen);

unsigned int TINFCC tinf_adler32(const void *data, unsigned int length);

unsigned int TINFCC tinf_crc32(const void *data, unsigned int length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TINF_H_INCLUDED */
