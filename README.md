#portlt
A very basic pair of software to listen on a port and talk to a port reading \[from STDIN / to STDOUT\] (TCP/UDP)
#####Synopsis:
Run **`./portX -h`** or **`./portX --help`** [X = **'l'** / **'t'** respectively]
##Port Listen (*portl*):
Listen on specified port for incoming connections (TCP) / packets (UDP) and write to STDOUT.
##Port Tell (*portt*):
Send content of STDIN buffer in 1024 byte blocks to specified port on host via TCP / UDP.

