# SPDX-License-Identifier: Apache-2.0
if(CONFIG_UZLIB)
zephyr_include_directories(
  .
  ../src
)

zephyr_library_sources(
  ../src/crc32.c
)
zephyr_library_sources_ifdef(CONFIG_UZLIB_DECOMPRESS
  ../src/adler32.c
  ../src/tinflate.c
  ../src/tinfgzip.c
  ../src/tinfzlib.c
)
zephyr_library_sources_ifdef(CONFIG_UZLIB_COMPRESS
  ../src/genlz77.c
  ../src/defl_static.c
)
endif()