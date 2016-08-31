#!/bin/sh

DATA_DIR=test_data

function test() {
	INFILE=$1
	GZIP_FILE=$INFILE.gz
	OUTFILE=$FILENAME.out


	#compress it
	gzip -k $INFILE
    SIZE=`stat -f%z $GZIP_FILE`
    echo "compressed to $SIZE bytes"

	#decompress it with our app
	./tgunzip $GZIP_FILE $OUTFILE


	#compare original to decompressed file
	diff $OUTFILE $INFILE

	if [ $? != 0 ]; then
		echo "FAILED!"
		exit 1
	else
		echo "PASSED!"
	fi

	#clean up
	rm $GZIP_FILE
	rm $OUTFILE	
}

FILES=$DATA_DIR/*
for f in $FILES
do
  SIZE=`stat -f%z $f`
  echo "Processing $f ($SIZE bytes)"
  test $f
done

echo "*** Test completed, all passed! ***"