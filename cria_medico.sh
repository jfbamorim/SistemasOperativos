#!/bin/bash
FILE=medicos.txt
if [ -z "$1" ]
then
  echo "Não inseriu dados"
  exit
else
     if [ ! -e "$FILE" ]; then
	touch medicos.txt
     fi
     if grep -q $3 medicos.txt; then
        echo "Já existe um médico com o numero de cedula $3"
     else
        nomecompleto="$1 $2"
        echo "$nomecompleto;$3;$4;$s5;0;0;0" >> medicos.txt
        cat medicos.txt
     fi
fi
