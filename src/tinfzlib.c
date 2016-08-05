/*
 * tinfzlib  -  tiny zlib decompressor
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

#include "tinf.h"

int tinf_zlib_parse_header(TINF_DATA *d)
{
   unsigned char cmf, flg;

   /* -- get header bytes -- */

   cmf = tinf_read_src_byte(d);
   flg = tinf_read_src_byte(d);

   /* -- check format -- */

   /* check checksum */
   if ((256*cmf + flg) % 31) return TINF_DATA_ERROR;

   /* check method is deflate */
   if ((cmf & 0x0f) != 8) return TINF_DATA_ERROR;

   /* check window size is valid */
   if ((cmf >> 4) > 7) return TINF_DATA_ERROR;

   /* check there is no preset dictionary */
   if (flg & 0x20) return TINF_DATA_ERROR;

   /* initialize for adler32 checksum */
   d->checksum = 1;

   return cmf >> 4;
}

int tinf_zlib_uncompress_dyn(TINF_DATA *d)
{
    int res;
    unsigned char *data = d->dest;

    res = tinf_uncompress_dyn(d);

    if (res < 0) return res;

    d->checksum = tinf_adler32(data, d->dest - data, d->checksum);

    if (res == TINF_DONE) {
        /* -- get adler32 checksum -- */
        unsigned int a32 = 0;
        int i;
        for (i = 4; i--;) {
            a32 = a32 << 8 | tinf_read_src_byte(d);
        }
        if (d->checksum != a32) {
            return TINF_DATA_ERROR;
        }
    }

    return res;
}
