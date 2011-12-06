all: load print change
load: load.c header.c header.h
	gcc -g load.c header.c -o load
print: print.c header.c header.h
	gcc -g print.c header.c -o print
change: change.c header.c header.h
	gcc -g change.c header.c -o change
clean:
	rm -rf load print change
