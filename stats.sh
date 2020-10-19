#!/bin/bash
sed -n "/$1/P" pacientes.txt > tempfile.txt
nmed=$( wc -l tempfile.txt )
echo "O numero de medicos inscritos numa plataforma da localidade de $1 são $nmed"
rm -f tempfile.txt 
