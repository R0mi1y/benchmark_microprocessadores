import json
import os
import serial
import time

# Função para escrever dados em um arquivo JSON
def escrever_json(nome_arquivo, dados):
    with open(nome_arquivo, 'w') as arquivo:
        json.dump(dados, arquivo, indent=2)

# Função para ler dados de um arquivo JSON
def ler_json(nome_arquivo):
    with open(nome_arquivo, 'r') as arquivo:
        dados = json.load(arquivo)
    return dados

# Configuração da porta serial do Arduino
arduino_port = 'COM14'  # Substitua pelo caminho da porta serial do Arduino
baud_rate = 9600

# Inicialização da comunicação serial
ser = serial.Serial(arduino_port, baud_rate, timeout=1)
time.sleep(2)  # Aguarda 2 segundos para garantir que a comunicação esteja estabelecida
ser.write(b's')  # Envia o caractere 's' para iniciar o benchmark no Arduino

tempo = 0
cont = ler_json("./results.json")['cont']  # Obtém o contador de execuções do arquivo JSON

# Loop principal para monitorar a porta serial
while True:
    line = ser.readline().decode('UTF-8').strip()

    # Verifica se a mensagem indica o início do benchmark
    if "INICIADO" in line:
        tempo = time.time()  # Registra o tempo de início

    # Verifica se a mensagem indica o final do benchmark
    elif "FINALIZADO" in line:
        tempo = time.time() - tempo  # Calcula o tempo decorrido
        dados = ler_json("./results.json")
        dados[cont] = tempo
        cont += 1
        dados["cont"] = cont

        # Atualiza o arquivo JSON com os resultados
        escrever_json("./results.json", dados)

        # Exibe informações sobre o tempo de execução
        print(f"Tempo de execução: {tempo} milissegundos, {line.replace('FINALIZADO::', '')}")

    # Verifica se o Arduino está solicitando outra execução
    elif line == "Executar novamente? S/N:":
        ser.write(b's')  # Responde com 's' para indicar ao Arduino que deve executar novamente

    else:
        # Exibe mensagens não tratadas
        print(line)
