default:
	gcc -Wall -o portl portl.c
	gcc -Wall -o portt portt.c

release:
	gcc -Wall -O3 -o portl portl.c
	gcc -Wall -O3 -o portt portt.c

gnu99:
	gcc -Wall -std=gnu99 -o portl portl.c
	gcc -Wall -std=gnu99 -o portt portt.c

gnu99_release:
	gcc -Wall -O3 -std=gnu99 -o portl portl.c
	gcc -Wall -O3 -std=gnu99 -o portt portt.c	

gnu11:
	gcc -Wall -std=gnu11 -o portl portl.c
	gcc -Wall -std=gnu11 -o portt portt.c

gnu11_release:
	gcc -Wall -O3 -std=gnu11 -o portl portl.c
	gcc -Wall -O3 -std=gnu11 -o portt portt.c	
