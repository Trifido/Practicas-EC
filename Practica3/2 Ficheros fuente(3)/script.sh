#!/bin/bash

gcc -m32 suma_09_Casm.c -o suma_09_Casm -O1

for((i=0; i<11; i++)); do
echo $i; 
./suma_09_Casm

done | pr -11 -l 20 -w 120 >> eje1_1
