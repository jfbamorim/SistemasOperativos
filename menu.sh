#!/bin/bash
choice=-1
while [[ $choice != 0 ]]; do
  echo "1. Cria pacientes"
  echo "2. Cria médicos"
  echo "3. Stats"
  echo "4. Avalia médicos"
  echo "0. Sair"
  
  echo "Escolha uma das opções:"
  read choice

  case $choice in
    1)./cria_pacientes.sh ;;
    2)echo "Introduza o nome:" 
      read nome
      echo "Introduza o n º da cedula profissional:"
      read nced
      echo "Introduza a especialidade medica:"
      read espec
      echo "Introduza o email:"
      read email
      ./cria_medico.sh "$nome" $nced "$espec" $email;;
    3)echo "Introduza a localidade:"
      read local
      echo "Introduza o saldo mínimo:"
      read saldo
      ./stats.sh $local $saldo;;
    4)./avalia_medicos.sh;;
    0)exit;;
    *)echo "Opção não válida";;
  esac
done 
