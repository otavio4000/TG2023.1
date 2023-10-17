ifeq ($(OS), Windows_NT)
CC = clang++
name = hamiltoneano.exe
else
CC = g++
name = hamiltoneano.out
endif

RUN_ARGS = -f exemple.txt -s

$(name): main.c
	$(CC) $< -o $(name)

run: $(name)
	./$(name) $(RUN_ARGS)

clean:
	del hamiltoneano.exe
	del hamiltoneano.out

pipe: $(name)
	./$(name) < exemple.txt
