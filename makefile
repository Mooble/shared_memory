all: load print change query cleanup
load: load.c header.c header.h
	gcc -g load.c header.c -o load
print: print.c header.c header.h
	gcc -g print.c header.c -o print
change: change.c header.c header.h
	gcc -g change.c header.c -o change
query: query.c header.c header.h
	gcc -g query.c header.c -o query
cleanup: clean.c header.c header.h
	gcc -g clean.c header.c -o clean
.PHONY: clean
clean:
	rm -rf load print change query clean
