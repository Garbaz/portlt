#TODO: Clean up, create variabels

builddir=build

default: gnu11_release

release:
	if [ ! -e "build" ];then mkdir build; elif [ ! -d "build" ];then printf "\"build\" isn't a directory. Make might fail...\n"; fi
	gcc -Wall -O2 -o build/portl src/portl.c
	gcc -Wall -O2 -o build/portt src/portt.c

gnu99:
	if [ ! -e "build" ];then mkdir build; elif [ ! -d "build" ];then printf "\"build\" isn't a directory. Make might fail...\n"; fi
	gcc -Wall -std=gnu99 -o build/portl src/portl.c
	gcc -Wall -std=gnu99 -o build/portt src/portt.c

gnu99_release:
	if [ ! -e "build" ];then mkdir build; elif [ ! -d "build" ];then printf "\"build\" isn't a directory. Make might fail...\n"; fi
	gcc -Wall -O2 -std=gnu99 -o build/portl src/portl.c
	gcc -Wall -O2 -std=gnu99 -o build/portt src/portt.c	

gnu11:
	if [ ! -e "build" ];then mkdir build; elif [ ! -d "build" ];then printf "\"build\" isn't a directory. Make might fail...\n"; fi
	gcc -Wall -std=gnu11 -o build/portl src/portl.c
	gcc -Wall -std=gnu11 -o build/portt src/portt.c

gnu11_release:
	if [ ! -e "build" ];then mkdir build; elif [ ! -d "build" ];then printf "\"build\" isn't a directory. Make might fail...\n"; fi
	gcc -Wall -O2 -std=gnu11 -o build/portl src/portl.c
	gcc -Wall -O2 -std=gnu11 -o build/portt src/portt.c

install:
	cp build/portt /usr/bin/
	cp build/portl /usr/bin/

uninstall: clean

clean:
	rm /usr/bin/portt
	rm /usr/bin/portl
	rm build/*


