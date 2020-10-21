#!/bin/bash
#grep 'a[0-9]' /etc/passwd | head -n 10 > ficheiroextraido.txt

while IFS="" read -r p || [ -n "$p" ]
do
   linha=$p
   numero=$(echo $linha | cut -d ":" -f1)
   nome=$(echo $linha | cut -d ":" -f5)
   email=("a$numero@iscte-iul.pt")
   echo "$numero;$nome;;;$email;100" 
   echo "$numero;$nome;;;$email;100" >> pacientes.txt 
done < ficheiroextraido.txt

#rm -f ficheiroextraido.txt

