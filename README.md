# autoroute #

Este sistema contém uma série de programas que em conjunto fornecem um meio de comunicaçào balanceada utilizando os meios de rede disponiveis, incluindo conexão Móvel.

### autoppp ###

Este programa executa identifica a operadora do device conectado e efetua a configuração e auto conexão com a mesma. Seus artefatos são:

* Udev: Regras para identificação de adição/remoção de modems. Após a detecção do mesmo o script deverá executar o programa autoppp;
* Programa autoppp: Responsável por identificar a operadora, gerar os arquivos de configuração e efetuar a conexão;

### autoroute ###

Programa que identifica interfaces de rede, calcula a disponibilidade de cada uma delas e efetua o retemaneto automático entre elas.

### atinout ###

Programa que executas comandos AT em dispositivos.
