.PHONY: build start debug test plot


CC      := gcc
CFLAGS  := -std=c99 -Wall -g3 -Iinclude
LDFLAGS := -lm

OUT    := bin
EXEC   := $(OUT)/hw2
OBJS   := src/hw2.o \
		  src/frame.o \
		  src/search.o \
		  src/algorithms/no_search.o \
		  src/algorithms/three_step_search.o \
		  src/algorithms/orthogonal_search.o \
		  src/algorithms/gradient_descent_search.o \
		  src/algorithms/cross_search.o

src/%.o: src/%.c include/%.h
	$(CC) $(CFLAGS) -o $@ -c $<

src/algorithms/%.o: src/algorithms/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(EXEC): $(OBJS)
	mkdir -p $(OUT)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

all: $(EXEC)


INPUT  := data/Foreman.CIF
FORMAT := CIF
SEARCH := cross

build: $(EXEC)

start: $(EXEC)
	./$(EXEC) $(INPUT) -f $(FORMAT) -s $(SEARCH)

debug: $(EXEC)
	gdb -nh -x $$XDG_CONFIG_HOME/gdb/init $(EXEC)

test: $(EXEC)
	@time ./$(EXEC) $(INPUT) -f $(FORMAT) -s $(SEARCH) > result.log

plot: $(EXEC) doc/plot/gnuplot.gpi
	time fish doc/plot/plot.fish
