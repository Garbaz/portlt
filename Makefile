default:
	gcc -Wall -o portl portl.c
	gcc -Wall -o portt portt.c
gnu99:
	gcc -Wall -std=gnu99 -o portl portl.c
	gcc -Wall -std=gnu99 -o portt portt.c
