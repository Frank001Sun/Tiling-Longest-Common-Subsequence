CC = gcc
CFLAGS = -m64 -O1 -std=gnu11 -g -ggdb -Wall -Werror -Wno-unused-result -Wno-unused-value -Wno-unused-variable -fopenmp
LDFLAGS = -lpthread
# Get all .c files
SRCS = $(wildcard *.c)
# Generate the corresponding executable name list
EXECS = $(SRCS:.c=)

.PHONY: all clean

all: $(EXECS)

# Pattern rule: build an executable for each .c file
%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Remove all generated executables
clean:
	rm -f $(EXECS)
