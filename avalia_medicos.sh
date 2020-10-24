#!/bin/bash
FILE="medicos.txt"
NEWFILE="lista_negra_medicos"
mincons=6
minaval=5

if [ ! -f "$NEWFILE" ]; then
  touch lista_negra_medicos.txt
fi
if [ -f "$FILE" ]; then
   while IFS="" read -r p || [ -n "$p" ]
   do
      linha=$p
      ncons=$(echo $linha | cut -d ";" -f5)
      aval=$(echo $linha | cut -d ";" -f6)
      if [[ "$ncons" -gt "$mincons" && "$aval" -lt "$minaval" ]]; then
         echo $linha >> lista_negra_medicos.txt
      fi
   done < medicos.txt
   cat lista_negra_medicos.txt
else
   echo "Ficheiro dos médicos não existe"
fi
