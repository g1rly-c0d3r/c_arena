CC?=clang
CFLAGS+=-fpic
INSTALL_DIR?=/usr/local/
lib_dir:=/lib/
include_dir?=/include/
OBJS:=arena.o

libarena.so: $(OBJS)
	$(CC) -shared -o $@ $^

test_arena: test.c  ${OBJS}
	$(CC) -o $@ $^

.PHONY: clean install
clean:
	rm -f $(OBJS) test_arena libarena.so

install: libarena.so
	cp $^ $(INSTALL_DIR)/$(lib_dir)
	cp arena.h $(INSTALL_DIR)/$(include_dir)
	

