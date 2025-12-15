all:
	gcc -o collision collision.c particles.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
