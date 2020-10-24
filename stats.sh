#!/bin/bash
# recolha dos pacientes inscitos numa plataforma na localidade passada como primeiro arg
FILEP="pacientes.txt"
FILEM="medicos.txt"
minval=500

if [ -f "$FILEP" ]; then
   sed -n "/$1/P" pacientes.txt > tempfile.txt
   npac=$( cat tempfile.txt | wc -l )
   echo "O numero de pacientes inscritos numa plataforma na localidade de $1 são $npac" | xargs
   rm -f tempfile.txt 
else
   echo "Ficheiro dos pacientes não existe"
fi

if [ -f "$FILEM" ]; then  
# recolha dos medicos inscritos na Plataforma com saldo superior a um determinado valor
   touch countmeds.txt
   
   while IFS="" read -r p || [ -n "$p" ]
   do
      line=$p
      valor=$( echo $line | cut -d ";" -f7 )
      if [[ "$valor" -gt "$minval" ]]; then
        echo line >> countmeds.txt 
      fi
   done < medicos.txt

   nmed=$( cat countmeds.txt | wc -l )
   echo "O numero de medicos inscritos na Plataforma com saldo superior a $2 são $nmed" | xargs 
   rm -f countmeds.txt
else
   echo "Ficheiro dos médicos não existe"
fi
