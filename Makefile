CC=gcc
FLAGS=
OUT=mouse-emulator

$(OUT): main.c
	$(CC) $(FLAGS) -o $(OUT) main.c

debug: main.c
	$(CC) -g $(FLAGS) -o $(OUT) main.c

install:
	@echo installing to ${DESTDIR}${PREFIX}/bin
	@cp -f $(OUT) ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/multicopy

uninstall:
	@echo removing ${DESTDIR}${PREFIX}/bin/$(OUT)
	@rm -f ${DESTDIR}${PREFIX}/bin/$(OUT)

.PHONY: clean
clean:
	rm -rf $(OUT)
