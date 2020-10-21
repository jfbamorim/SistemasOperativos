#!/bin/bash
while IFS="" read -r p || [ -n "$p" ]
do
   linha=$p
   ncons=$(echo $linha | cut -d ";" -f5)
   aval=$(echo $linha | cut -d ";" -f6)
   if [ $ncons -gt 6 ] && [ $aval -lt 5 ]; then
      echo $linha >> lista_negra_medicos.txt
   fi
done < medicos.txt 
