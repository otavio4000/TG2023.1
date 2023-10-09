ifeq ($(OS), Windows_NT)
CC = clang++
name = dijkstra.exe
else
CC = g++
name = dijkstra.out
endif

RUN_ARGS = -f exemple.txt -s

$(name): main.c
	$(CC) $< -o $(name)

run: $(name)
	./$(name) $(RUN_ARGS)

clean:
	del dijkstra.exe
	del dijkstra.out

pipe: $(name)
	./$(name) < exemple.txt
