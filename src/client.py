import socket

HOST = 'localhost'  # Endereço IP ou nome do host do servidor
PORT = 25577  # Porta do servidor

# Cria um socket TCP/IP
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Conecta-se ao servidor
s.connect((HOST, PORT))
print("Conexão estabelecida com o servidor")

# Recebe os dados do servidor e exibe no terminal do cliente
while True:
    data = s.recv(1024)
    if not data:
        break
    print("Log recebido:", data.decode())

# Encerra a conexão com o servidor
s.close()
print("Conexão encerrada")

