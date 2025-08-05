INCLDIR = ./include
CC = gcc
CFLAGS = -O2 -include stdio.h
CFLAGS += -I$(INCLDIR)

INCLUDE_LIBRARY_PTHREAD = -lpthread

default: task4_threads

OBJDIR = obj

_DEPS = task4_threads.h dbj2.h tea.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = task4_threads.o dbj2.o tea.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

task4_threads: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) $(INCLUDE_LIBRARY_PTHREAD)

.PHONY: clean
clean: 
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~
