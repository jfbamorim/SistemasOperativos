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
     if grep -q $2 medicos.txt; then
        echo "Já existe um médico com o numero de cedula $3"
     else
        nome="$1"
        ced=$2
        esp="$3"
        eml=$4
        echo "$nome;$ced;$esp;$eml;0;0;0" >> medicos.txt
        cat medicos.txt
     fi
fi
