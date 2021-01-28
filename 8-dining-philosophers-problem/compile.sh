gcc optimal/main.c -o optimal/main
gcc deadlock/main.c -o deadlock/main
gcc starvation/main.c -o starvation/main

gcc optimal/philosopher.c -o optimal/philosopher
gcc deadlock/philosopher.c -o deadlock/philosopher
gcc starvation/philosopher.c -o starvation/philosopher

echo "Compiled"