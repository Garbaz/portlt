default: gnu11

release:
	gcc -Wall -O2 -o bin/portl src/portl.c
	gcc -Wall -O2 -o bin/portt src/portt.c

gnu99:
	gcc -Wall -std=gnu99 -o bin/portl src/portl.c
	gcc -Wall -std=gnu99 -o bin/portt src/portt.c

gnu99_release:
	gcc -Wall -O2 -std=gnu99 -o bin/portl src/portl.c
	gcc -Wall -O2 -std=gnu99 -o bin/portt src/portt.c	

gnu11:
	gcc -Wall -std=gnu11 -o bin/portl src/portl.c
	gcc -Wall -std=gnu11 -o bin/portt src/portt.c

gnu11_release:
	gcc -Wall -O2 -std=gnu11 -o bin/portl src/portl.c
	gcc -Wall -O2 -std=gnu11 -o bin/portt src/portt.c

install:
	cp bin/portt /usr/bin/
	cp bin/portl /usr/bin/

uninstall: clean

clean:
	rm /usr/bin/portt
	rm /usr/bin/portl
