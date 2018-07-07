rm -f afl-corpus.log

env | grep UBSAN

for f in afl-corpus/*/crashes/*; do
    echo "*" $f
    ../examples/tgunzip/tgunzip "$f" /dev/null
    echo $f $? >>afl-corpus.log
done

echo

if diff -u afl-corpus.ref afl-corpus.log; then
    echo "Test passed"
else
    echo "Test FAILED"
fi