CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -g
CPPFLAGS = -Iinclude
COMMON = src/clientes.c src/estoque.c src/sistema.c
.PHONY: all test experimento sanitize clean
all: build/autopecas
build:
	mkdir -p build
build/autopecas: src/main.c $(COMMON) $(wildcard include/*.h) | build
	$(CC) $(CPPFLAGS) $(CFLAGS) src/main.c $(COMMON) -o $@
build/teste: tests/teste.c $(COMMON) $(wildcard include/*.h) | build
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/teste.c $(COMMON) -o $@
build/experimento: tests/experimento.c $(COMMON) $(wildcard include/*.h) | build
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/experimento.c $(COMMON) -o $@
test: build/teste build/autopecas
	./build/teste
experimento: build/experimento
	./build/experimento
sanitize: | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -fsanitize=address,undefined -fno-omit-frame-pointer tests/teste.c $(COMMON) -o build/teste_sanitize
	ASAN_OPTIONS=detect_leaks=1 ./build/teste_sanitize
clean:
	rm -rf build
