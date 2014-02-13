CXXOPTIONS+=-std=c++0x
CXXOPTIONS+=-Wall -Wextra -Werror

LDOPTIONS+=-lboost_system -lboost_filesystem -lboost_program_options -lv8

.PHONY: all clean

all: stork

stork: stork.o \
	   template.o \
	   params_store.o \
	   utils.o
	$(CXX) $(LDOPTIONS) -o $@ $^

%.o: %.cc %.h
	$(CXX) $(CXXOPTIONS) -c -o $@ $<

clean:
	rm -f *.o stork
