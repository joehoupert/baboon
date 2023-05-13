CC=gcc
CFLAGS=-Wall

SRCS = bmp.c bab.c
OBJS = $(SRCS:.c=.o)

TARGET = bab

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC)  $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)


