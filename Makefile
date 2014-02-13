
.PHONY: all clean src test

all: src

src:
	$(MAKE) -C src

#test:
#	$(MAKE) -C test

clean:
	$(MAKE) -C src clean
	#$(MAKE) -C test clean
