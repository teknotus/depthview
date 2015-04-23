BINDIR=/usr/local/bin
CC=gcc
CFLAGS=-Wall
SRCS=realsense_map_controls.c
OBJS=$(SRCS:.c=.o)
TARGET=realsense-map-controls

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(TARGET)

install: $(TARGET)
	cp $(TARGET) $(BINDIR)
	cp 80-realsense.rules /etc/udev/rules.d/
