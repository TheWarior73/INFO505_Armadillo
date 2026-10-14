CC = gcc
CFLAGS = -Wall -Wextra -Werror -I..
LDFLAGS =
LDLIBS = -L.. -lelement

TARGET = test_element
SRCS = test_element.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all test clean
