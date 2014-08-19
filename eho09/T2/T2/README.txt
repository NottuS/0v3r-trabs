Tabelas(colunas):
	-addresses(street, number)
	-people(first_name, last_name, cpf)
	-cities(name)
	-phones(number)

Relacionamentos:
	-people tem relação muitos para muitos com addresses
	-people tem relação um para muitos com phones
	-addresses tem relação um para um com cities 

Modo de execução:
	ruby T2.rb operation[insert|remove|show|update|help] table_name[addresses|people|cities|phones] [-d|--data]

A operação show exibe os registros da tabela solicitada e também todos os registros de outras tabelas associadas com aquele registro.

Executando a operação sem a opção -d, o programa informa qual campo você deve inserir. Com a opção -d, há a possibilidade de passar os dados necessários pelas operações via linha de comando.


Modo de uso das operações com o -d:
-remove:
	ruby T2.rb remove Nome_Tabela -d id_do_registro
	Ex:
	  ruby T2.rb remove people -d 3	
-update:
	ruby T2.rb update Nome_tabela -d id_do_registro 
	Ex:
          ruby T2.rb update addresses -d 1 number 890

-insert:
	O insert possui uma ordem de parâmetros estabelecidos para cada tabela:
	-people:
		ruby insert people -d Primeiro_Nome Ultimo_nome cpf Quantidade_de_Endereços rua1 numero1 cidade1 ... ruaN numeroN cidadeN Quantidade_de_telefones telefone1 ... telefoneN

	Ex:
	   ruby T2.rb insert people -d Luiz "da Costa" 80980890 2 "Salto do itararé" 56 Curitiba "São Francisco" 789 "Foz do Iguaçu" 2 8098-9890 7987-7890
	

	-addresses:
		ruby T2.rb insert addresses -d rua numero cidade Quantidade_de_Pessoas Primeiro_Nome1 Ultimo_nome1 cpf1 Quantidade_de_telefones telefone1 ... telefoneN ... Primeiro_NomeN Ultimo_nomeN cpfN Quantidade_de_telefonesN telefone1 ... telefoneN
	Ex:
	   ruby T2.rb insert addresses -d "Salto do itararé" 666 Curitiba 2 Lucia Miranda 8098098 1 8909-5757 Lucas Costa 6798798 2 7987-7899 7987-8099


	-phones:
		ruby T2.rb insert phones -d telefone
	Ex:
	   ruby T2.rb insert phones -d 8979-9878
	
	
	-cities:
		ruby T2.rb insert cities -d cidade 
	Ex:
	  ruby T2.rb insert cities -d "São Paulo"

