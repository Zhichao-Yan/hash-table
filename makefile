CC := gcc
CFLAGS := -g
DIR := ./build
SRC := ./src

all: main
main: $(SRC)/main.c $(SRC)/hash_table.c
	$(CC) $(CFLAGS) -I$(SRC) $^ -o $@ -lm
	mv $@ $(DIR)/$@

clean:
	rm -f $(DIR)/*