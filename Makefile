CERRFLAGS:=-pedantic -Wall -Wextra -Werror
CC?=clang
CFLAGS+=-fpic $(CERRFLAGS)
INSTALL_DIR?=/usr/local/
lib_dir:=/lib/
include_dir?=/include/
OBJS:=arena.o

libarena.so: $(OBJS)
	$(CC) -shared -o $@ $^

test: test.c libarena.so
	$(CC) -g $(CFLAGS) -o $@ $^

.PHONY: clean install
clean:
	rm -f $(OBJS) test libarena.so a.out

install: libarena.so
	cp $^ $(INSTALL_DIR)/$(lib_dir)
	cp arena.h $(INSTALL_DIR)/$(include_dir)
	

