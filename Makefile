TARGET: ileczekam czekamnaudp timer

CC	= cc
CFLAGS	= -Wall -O2
LFLAGS	= -Wall

timer: timer.o
	$(CC) $(LFLAGS) $^ -o $@

ileczekam: ileczekam.o err.o
	$(CC) $(LFLAGS) $^ -o $@

czekamnaudp: czekamnaudp.o err.o
	$(CC) $(LFLAGS) $^ -o $@

.PHONY: clean TARGET
clean:
	rm -f ileczekam czekamnaudp *.o *~ *.bak
