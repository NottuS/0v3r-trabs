commit:
	# Mostrar IP do client e servidor no log
	cp -f ~/repos/ns-3-allinone/ns-3.16/src/applications/model/udp-echo-client.cc ~/0v3r-trabs/mpt06/src/applications/model/
	cp -f ~/repos/ns-3-allinone/ns-3.16/src/applications/model/udp-echo-server.cc ~/0v3r-trabs/mpt06/src/applications/model/
	
	# Criar nó como sendo routeador ou não
	cp -f ~/repos/ns-3-allinone/ns-3.16/src/network/model/node.cc ~/0v3r-trabs/mpt06/src/network/model/
	cp -f ~/repos/ns-3-allinone/ns-3.16/src/network/model/node.h ~/0v3r-trabs/mpt06/src/network/model/
	
	# Criar função para definir um roteador IPv4
	cp -f ~/repos/ns-3-allinone/ns-3.16/src/internet/helper/ipv4-interface-container.cc ~/0v3r-trabs/mpt06/src/internet/helper/
	cp -f ~/repos/ns-3-allinone/ns-3.16/src/internet/helper/ipv4-interface-container.h ~/0v3r-trabs/mpt06/src/internet/helper/
	
	# Adicionar globalRouter somente para os nós que forem roteadores
	cp -f ~/repos/ns-3-allinone/ns-3.16/src/internet/helper/ipv4-global-routing-helper.cc ~/0v3r-trabs/mpt06/src/internet/helper/
	
	# Exemplos
	cp -f ~/repos/ns-3-allinone/ns-3.16/scratch/router.cc ~/0v3r-trabs/mpt06/scratch/
	cp -f ~/repos/ns-3-allinone/ns-3.16/scratch/routers.cc ~/0v3r-trabs/mpt06/scratch/
	cp -f ~/repos/ns-3-allinone/ns-3.16/scratch/ospfv1.cc ~/0v3r-trabs/mpt06/scratch/
	cp -f ~/repos/ns-3-allinone/ns-3.16/scratch/ospfv2.cc ~/0v3r-trabs/mpt06/scratch/
	cp -f ~/repos/ns-3-allinone/ns-3.16/scratch/ospfv3.cc ~/0v3r-trabs/mpt06/scratch/

work:
	cp -f ~/0v3r-trabs/mpt06/src/applications/model/* ~/repos/ns-3-allinone/ns-3.16/src/applications/model/
	
	cp -f ~/0v3r-trabs/mpt06/src/network/model/* ~/repos/ns-3-allinone/ns-3.16/src/network/model/
	
	cp -f ~/0v3r-trabs/mpt06/src/internet/helper/* ~/repos/ns-3-allinone/ns-3.16/src/internet/helper/
	
	cp -f ~/0v3r-trabs/mpt06/scratch/ ~/repos/ns-3-allinone/ns-3.16/scratch/

clean:
	rm ~/repos/ns-3-allinone/ns-3.16/*.pcap
	rm ~/repos/ns-3-allinone/ns-3.16/*.tr
