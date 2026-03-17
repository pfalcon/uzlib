/*
 * uzlib  -  tiny deflate/inflate library (deflate, gzip, zlib)
 *
 * Copyright (c) 2014-2018 by Paul Sokolovsky
 */
#ifndef UZLIB_CONF_H_INCLUDED
#define UZLIB_CONF_H_INCLUDED

#include <zephyr.h>

/* Debug logging level 0, 1, 2, etc. */
#define UZLIB_CONF_DEBUG_LOG CONFIG_UZLIB_DEBUG_LOG

/* Perform extra checks on the input stream, even if they aren't proven
   to be strictly required (== lack of them wasn't proven to lead to
   crashes). */
#define UZLIB_CONF_PARANOID_CHECKS IS_ENABLED(CONFIG_UZLIB_PARANOID_CHECKS)

/* Use memcpy() for copying data out of LZ window or uncompressed blocks,
   instead of doing this byte by byte. For well-compressed data, this
   may noticeably increase decompression speed. But for less compressed,
   it can actually deteriorate it (due to the fact that many memcpy()
   implementations are optimized for large blocks of data, and have
   too much overhead for short strings of just a few bytes). */
#define UZLIB_CONF_USE_MEMCPY IS_ENABLED(CONFIG_UZLIB_USE_MEMCPY)

#endif /* UZLIB_CONF_H_INCLUDED */
