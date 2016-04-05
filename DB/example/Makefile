FLAGS=-Wall -Werror -I.
LIBS=-lpthread -ldl
all: sqlite3.o
	g++ $(FLAGS) sqlite3.o hello.cpp Backup.cpp Column.cpp Database.cpp Statement.cpp Transaction.cpp $(LIBS)  

sqlite3.o:
	gcc $(FLAGS) sqlite3.c -c
