all:
	gcc -Wall multistereo.c main.c -I /usr/local/include -L /usr/local/lib -lportaudio -lfftw3f -lsndfile -o multistereo

exec:
	gcc -Wall -o multistereo -I /usr/local/include main.c
	./testExec