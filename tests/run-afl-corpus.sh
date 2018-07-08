rm -f afl-corpus.log

export LANG=C.UTF-8
unset LC_COLLATE
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
