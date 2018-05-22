.PHONY: build start

CC      := gcc
CFLAGS  := -O2 -std=c99 -Wall -g
LDFLAGS := -lm

OUT    := bin
EXEC   := $(OUT)/hw2
OBJS   := src/hw2.o src/frame.o

src/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(EXEC): $(OBJS)
	mkdir -p $(OUT)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

build: $(EXEC)

start: $(EXEC)
	@time ./$(EXEC) > result.log
	diff result.log expect.log

debug: $(EXEC)
	gdb -nh -x $$XDG_CONFIG_HOME/gdb/init $(EXEC)
