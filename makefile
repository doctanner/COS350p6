# AUTHOR: James Tanner
# COURSE: COS 350 - Systems Programming
# PURPOSE: Program 6

COMP=gcc
FLAGS=-g

# Everything:
all: lc1 lc2 lc3

# Version 1:
lc1: lc1.o countlines.o
	$(COMP) $(FLAGS) lc1.o countlines.o -o lc1

lc1.o: lc1.c countlines.h
	gcc -c $(FLAGS) lc1.c


# Version 2:
lc2: lc2.o countlines.o
	$(COMP) $(FLAGS) lc2.o countlines.o -o lc2

lc2.o: lc2.c countlines.h
	gcc -c $(FLAGS) lc2.c


# Version 3:
lc3: lc3.o countlines.o
	$(COMP) $(FLAGS) -pthread lc3.o countlines.o -o lc3

lc3.o: lc3.c countlines.h
	gcc -c $(FLAGS) lc3.c


# Supporting files:
countlines.o: countlines.c countlines.h
	gcc -c $(FLAGS) countlines.c

clean:
	rm -f *.o
