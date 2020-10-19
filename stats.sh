#!/bin/bash
# recolha dos pacientes inscitos numa plataforma na localidade passada como primeiro arg
sed -n "/$1/P" pacientes.txt > tempfile.txt
npac=$( cat tempfile.txt | wc -l )
echo "O numero de medicos inscritos numa plataforma na localidade de $1 são $npac"
rm -f tempfile.txt 

# recolha dos medicos inscritos na Plataforma com saldo superior a um determinado valor
sed -n "/$1/P" medicos.txt > tempfile.txt

nmed=$(cat tempfile.txt | wc -l)

