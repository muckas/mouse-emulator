CC=gcc
FLAGS=
OUT=mouse-emulator

$(OUT): main.c
	$(CC) $(FLAGS) -o $(OUT) main.c

debug: main.c
	$(CC) -g $(FLAGS) -o $(OUT) main.c
	gdb $(OUT)

.PHONY: clean
clean:
	rm -rf $(OUT)
