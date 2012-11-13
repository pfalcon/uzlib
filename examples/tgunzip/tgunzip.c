/*
 * tgunzip  -  gzip decompressor example
 *
 * Copyright (c) 2003 by Joergen Ibsen / Jibz
 * All Rights Reserved
 *
 * http://www.ibsensoftware.com/
 *
 * This software is provided 'as-is', without any express
 * or implied warranty.  In no event will the authors be
 * held liable for any damages arising from the use of
 * this software.
 *
 * Permission is granted to anyone to use this software
 * for any purpose, including commercial applications,
 * and to alter it and redistribute it freely, subject to
 * the following restrictions:
 *
 * 1. The origin of this software must not be
 *    misrepresented; you must not claim that you
 *    wrote the original software. If you use this
 *    software in a product, an acknowledgment in
 *    the product documentation would be appreciated
 *    but is not required.
 *
 * 2. Altered source versions must be plainly marked
 *    as such, and must not be misrepresented as
 *    being the original software.
 *
 * 3. This notice may not be removed or altered from
 *    any source distribution.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tinf.h"

void exit_error(const char *what)
{
   printf("ERROR: %s\n", what);
   exit(1);
}

int main(int argc, char *argv[])
{
    FILE *fin, *fout;
    unsigned int len, dlen, outlen;
    unsigned char *source, *dest;
    int res;

    printf("tgunzip - example from the tiny inflate library (www.ibsensoftware.com)\n\n");

    if (argc < 3)
    {
       printf(
          "Syntax: tgunzip <source> <destination>\n\n"
          "Both input and output are kept in memory, so do not use this on huge files.\n");

       return 1;
    }

    tinf_init();

    /* -- open files -- */

    if ((fin = fopen(argv[1], "rb")) == NULL) exit_error("source file");

    if ((fout = fopen(argv[2], "wb")) == NULL) exit_error("destination file");

    /* -- read source -- */

    fseek(fin, 0, SEEK_END);

    len = ftell(fin);

    fseek(fin, 0, SEEK_SET);

    source = (unsigned char *)malloc(len);

    if (source == NULL) exit_error("memory");

    if (fread(source, 1, len, fin) != len) exit_error("read");

    fclose(fin);

    /* -- get decompressed length -- */

    dlen =            source[len - 1];
    dlen = 256*dlen + source[len - 2];
    dlen = 256*dlen + source[len - 3];
    dlen = 256*dlen + source[len - 4];

    dest = (unsigned char *)malloc(dlen);

    if (dest == NULL) exit_error("memory");

    /* -- decompress data -- */

    outlen = dlen;

    res = tinf_gzip_uncompress(dest, &outlen, source, len);

    if ((res != TINF_OK) || (outlen != dlen)) exit_error("inflate");

    printf("decompressed %u bytes\n", outlen);

    /* -- write output -- */

    fwrite(dest, 1, outlen, fout);

    fclose(fout);

    return 0;
}
