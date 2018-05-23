.PHONY: build start debug test

CC      := gcc
CFLAGS  := -std=c99 -Wall -g3 -Iinclude
LDFLAGS := -lm

OUT    := bin
EXEC   := $(OUT)/hw2
OBJS   := src/hw2.o\
		  src/frame.o\
		  src/search.o\
		  src/algorithms/three_step_search.o

src/%.o: src/%.c include/%.h
	$(CC) $(CFLAGS) -o $@ -c $<

src/algorithms/%.o: src/algorithms/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(EXEC): $(OBJS)
	mkdir -p $(OUT)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

build: $(EXEC)

start: $(EXEC)
	./$(EXEC)

debug: $(EXEC)
	gdb -nh -x $$XDG_CONFIG_HOME/gdb/init $(EXEC)

test: $(EXEC)
	@time ./$(EXEC) > result.log
	# diff result.log expect.log
