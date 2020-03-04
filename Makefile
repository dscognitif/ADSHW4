CC     = gcc
CFLAGS = -g -std=c99 -pedantic -Wall

tree: scanner.c treeForm.c treeMain.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f tree
