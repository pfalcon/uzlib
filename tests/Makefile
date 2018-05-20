# Very basic smoke test for decompressor
test:
	$(MAKE) -C ../src -f makefile.elf
	$(MAKE) -C ../examples/tgunzip -f makefile.elf
	../examples/tgunzip/tgunzip corpus.tar.gz corpus-out.tar
	md5sum -c corpus.md5sum
