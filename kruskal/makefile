ifeq ($(OS), Windows_NT)
CC = clang++
name = kruskal.exe
else
CC = g++
name = kruskal.out
endif

RUN_ARGS = -f exemple.txt -s

$(name): main.c
	$(CC) $< -o $(name)

run: $(name)
	./$(name) $(RUN_ARGS)

clean:
	del kruskal.exe
	del kruskal.out

pipe: $(name)
	./$(name) < exemple.txt
