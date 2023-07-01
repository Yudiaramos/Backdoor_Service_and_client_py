# Modulo Kernel que faz pequeno Backdoor

Para replicar o backdoor inicialmente clone o repositório:

```
git clone https://github.com/seu_usuario/seu_repositorio.git
```

então navegue até a pasta src, em que o Makefile está localizado

```
cd seu_repositorio/LinuxKernelModuleBackdoor/src
```

Compile o módulo do kernel dando o comando:

```
make
```

Logo após, insira o módulo no kernel:

```
sudo insmod backdoor.ko
```

##Receber os dados escritos

Para receber tudo que foi escrito no teclado, primeiro execute o server.py em um novo terminal para iniciar o servidor:

```
python3 server.py
```

Em outro terminal, execute o client.py para conectar-se ao servidor e receber os logs:

```
python3 client.py
```

A partir desse momento, o cliente estará conectado ao servidor e receberá as atualizações do log em tempo real.

Você também pode encerrar a conexão e remover o módulo do kernel quando desejar:

Para encerrar a conexão do cliente, basta pressionar Ctrl + C no terminal onde o client.py está sendo executado.

Para remover o módulo do kernel, execute o seguinte comando:

```
sudo rmmod backdoor.ko
```

Certifique-se de ter as permissões necessárias para inserir e remover módulos do kernel.

Lembrando que é importante adaptar os comandos conforme a estrutura do seu repositório e diretório de trabalho (se atente aos códigos que possuem tratam uma localidade de código, adaptando para seu próprio OS).
