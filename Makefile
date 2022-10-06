CC=gcc
FLAGS=-lX11 -lXtst
OUT=mouse-emulator

$(OUT): main.c
	$(CC) $(FLAGS) -o $(OUT) main.c

.PHONY: clean
clean:
	rm -rf $(OUT)
