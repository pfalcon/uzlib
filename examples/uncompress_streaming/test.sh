#!/bin/sh

function test {
	FILENAME=random
	INFILE=$FILENAME.bin
	GZIP_FILE=$INFILE.gz
	OUTFILE=$FILENAME.out

	COUNT=$(($RANDOM*100 + $RANDOM))
	echo "Generating random file, $COUNT bytes"

	#generate a random bytestream
	dd if=/dev/urandom of=$INFILE bs=1 count=$COUNT

	#compress it
	gzip -k $INFILE

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
	rm $INFILE
	rm $GZIP_FILE
	rm $OUTFILE	
}

if [ $# -lt 1 ]; then
	echo "usage: test.sh <# of runs>"
	exit 1
fi

COUNTER=0
while [  $COUNTER -lt $"1" ]; do
    echo Run $COUNTER
    test
    let COUNTER=COUNTER+1 
done