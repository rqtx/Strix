
set directories src/
file bin/Strix

set args -t 10.0.0.10 -a 10.0.0.1

#General config
set logging on

#Breakpoints
b main.c:main

#Start
run

