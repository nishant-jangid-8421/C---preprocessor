CC = gcc 
CFLAGS = -Wall -Wextra

TARGET = mypreprocessor

SOURCES = main.c comment_removal.c file_inclusion.c macro_handler.c

all: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o temp1.c temp2.c *.i
