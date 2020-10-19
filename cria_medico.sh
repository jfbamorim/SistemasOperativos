#!/bin/bash
if [ -z "$1" ]
then
  echo "Não inseriu dados"
  exit
else
  if grep -q $3 medicos.txt; then
     echo "Já existe um médico com o numero de cedula $3"
  else
     echo "$1 $2;$3;$4;$s5;0;0;0" >> medicos.txt
     cat medicos.txt
  fi
fi
