/*
 * tgunzip -  gzip decompressor example
 *
 * Copyright (c) 2016 Currant, Inc.
 * All Rights Reserved
 * Modified to support a single byte output stream
 *  
 * 
 * Copyright (c) 2003 by Joergen Ibsen / Jibz
 * All Rights Reserved
 *
 * http://www.ibsensoftware.com/
 *
 * Copyright (c) 2014-2016 by Paul Sokolovsky
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
#include <string.h> 
#include <errno.h>

#include "tinf.h"


#define OUTPUT_BUFFER_SIZE (4)
int output_position;  //position in output_buffer
unsigned char output_buffer[OUTPUT_BUFFER_SIZE];

FILE *fin, *fout;

void exit_error(const char *what)
{
   printf("ERROR: %s\n", what);
   exit(1);
}

//readDest - read a byte from the decompressed destination file, at 'offset' from the current position.
//offset will be the negative offset back into the written output stream.
//note: this does not ever write to the output stream; it simply reads from it.
static unsigned int readDestByte(int offset, unsigned char *out)
{
  unsigned char data;

  //delta between our position in output_buffer, and the desired offset in the output stream
  int delta = output_position + offset;

  if (delta >= 0) {
    //we haven't written output_buffer to persistent storage yet; we need to read from output_buffer
    data = output_buffer[delta];
  } 

  else {
    //we need to read from persistent storage


    //save where we are in the file
    long last_pos = ftell(fout);

    //go back to the delta (desired offset from the current position)
    int retval = fseek(fout, delta, SEEK_CUR);
    if (retval == -1) {
      printf("errno=%s\n", strerror(errno));
      exit_error("fseek pre");
      return -1;
    }   

    if (fread(&data, 1, 1, fout) != 1 && feof(fout) == 0) {
      //error if we don't read a byte and it's not the end of file
      printf("errno=%s\n", strerror(errno));
      exit_error("read");
      return -1;
    } 

    //go back to where we were, so we can write the next word to the proper position in the stream
    retval = fseek(fout, last_pos, SEEK_SET);
    if (retval == -1) {
      printf("errno=%s\n", strerror(errno));
      exit_error("fseek post");
      return -1;
    }
  }

    *out = data;

    return 0;  
}

/*
 * readSourceByte - consume and return a byte from the source stream into the argument 'out'.
 *                  returns 0 on success, or -1 on error.
 */
static unsigned int readSourceByte(struct TINF_DATA *data, unsigned char *out)
{
  if (fread(out, 1, 1, fin) != 1) {
    exit_error("read");
    return -1;
  }
  return 0;
}

int main(int argc, char *argv[])
{
    unsigned int len, dlen, outlen;
    const unsigned char *source;
    int res;

    printf("tgunzip\n\n");

    if (argc < 3)
    {
       printf("Syntax: tgunzip <source> <destination>\n\n");
       return 1;
    }

    uzlib_init();

    /* -- open files -- */

    if ((fin = fopen(argv[1], "r")) == NULL) exit_error("source file");
    if ((fout = fopen(argv[2], "w+")) == NULL) exit_error("destination file");


    //unsigned char output_buffer[OUTPUT_BUFFER_SIZE];

    TINF_DATA d;
    outlen = 0;
    d.readSourceByte = readSourceByte;
    d.readDestByte = readDestByte;
    d.destSize = 1;

    res = uzlib_gzip_parse_header(&d);
    if (res != TINF_OK) {
        printf("Error parsing header: %d\n", res);
        exit(1);
    }

    uzlib_uncompress_init(&d, NULL, 0);

    /* decompress a single byte at a time */
    output_position = 0;

    do {
        d.dest = &output_buffer[output_position];
        res = uzlib_uncompress_chksum(&d);
        if (res != TINF_OK) break;
        output_position++;
        //if the destination has been written to, write it out to disk
        if (output_position == OUTPUT_BUFFER_SIZE) {
            fwrite(output_buffer, 1, OUTPUT_BUFFER_SIZE, fout);
            outlen += OUTPUT_BUFFER_SIZE;
            output_position = 0;
        }
        
        
    } while (res == TINF_OK);

    if (res != TINF_DONE) {
        printf("Error during decompression: %d\n", res);
    }

    //write remaining bytes
    fwrite(output_buffer, 1, output_position, fout);

    printf("decompressed %d bytes\n", outlen + output_position);

    fclose(fin);
    fclose(fout);

    return 0;
}
