all: main.c node.c key.c bptree.c bptree_ui.c
	gcc -g -o bptree.out main.c node.c key.c bptree.c bptree_ui.c
debug: main.c node.c key.c bptree.c bptree_ui.c
	gcc -DDEBUG -g -o bptree.out main.c node.c key.c bptree.c bptree_ui.c
clean: 
	rm -f bptree.out
