TARGET = dz_shell
OBJ = main.c parser.c
CC = gcc

all:
	$(CC) -o $(TARGET) $(OBJ)
clean:
	rm -f *-o 
fclean: clean
	rm -f $(TARGET)
re: fclean all