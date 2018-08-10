
set directories src/
file bin/Strix

set args -t 10.0.0.10 -a 10.0.0.1

#General config
set logging on

#Breakpoints
b main.c:main
#b executor.c:38
#b executor.c:18
#b injector.c:95

#Start
run

