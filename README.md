#portlt
A very basic pair of software to listen on a port and talk to a port reading \[from STDIN / to STDOUT\] (TCP/UDP)

###Build & Install:
```
make && sudo make install
```

###Synopsis:
Run
```bash
./portl --help
```
and
```bash
./portt --help
```
respectively.
###Port Listen (*portl*):
Listen on specified port for incoming connections (TCP) / packets (UDP) and write to STDOUT.
###Port Tell (*portt*):
Send content of STDIN buffer in fixed size byte blocks to specified port on host via TCP / UDP.

