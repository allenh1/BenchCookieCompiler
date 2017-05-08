compiler := build/bcc
CFLAGS := -std=c11 -O0 -g
INCLUDE := -I include
source_files := $(wildcard src/*.c)
included_files := $(wildcard include/bcc/*.h)
object_files := $(patsubst src/%.c, \
	build/%.o, $(source_files))

.PHONY: all clean

$(compiler): $(object_files) $(main.c)
	@mkdir -p $(shell dirname $@)
	@gcc $(CFLAGS) $(INCLUDE) $(object_files) -o $(compiler)

build/%.o: src/%.c $(included_files)
	@mkdir -p $(shell dirname $@)
	@gcc $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@rm -r build
	@rm -f src/*~ include/bcc/*~ *~
