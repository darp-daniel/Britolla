import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit as cf
import serial
from matplotlib.animation import FuncAnimation
import time

# Definição das PORTS E BAUD_RATE
COM = 'COMn'  # Defina a porta correta
BAUD_RATE = 11200

# Conexão com o sensor
conn = serial.Serial(COM, BAUD_RATE)

# Array para armazenar dados
array_de_tuplas = []

def leituraSensor(array):
    # Espera a resposta do arduino
    if conn.in_waiting > 0:
        # Conversão para float
        linha = conn.readline().decode("utf-8").strip()
        valores = linha.split(', ')
        try:
            matriz = (float(valores[0]), float(valores[1]))
            array.append(matriz)
        except ValueError:
            print("Erro ao converter valores do sensor para float.")

def plotagem(frame):
    leituraSensor(array_de_tuplas)  # Atualiza os dados do sensor, se houver dados novos
    
    # Separando os dados em x e y
    if array_de_tuplas:
        xA, yA = zip(*array_de_tuplas)
        x = np.array(xA)
        y = np.array(yA)
        
        # Plotando os dados em tempo real
        ax.clear()
        ax.scatter(x, y, label='Dados', color="red")
        ax.plot(x, y, label='Relação MPU e Dedo')
        ax.set_xlabel('MPU')
        ax.set_ylabel('DEDO')
        ax.legend()

def acharFuncao(event):
    if len(array_de_tuplas) > 1:  # Verificar se há dados suficientes
        xA, yA = zip(*array_de_tuplas)
        x = np.array(xA)
        y = np.array(yA)
        
        # Função linear para ajuste
        def func(x, a, b):
            return a * x + b

        # Ajuste da curva
        params, _ = cf(func, x, y)
        a, b = params

        # Plotando os dados e a função ajustada
        plt.scatter(x, y, label="Dados", color="blue")
        plt.plot(x, func(x, a, b), label=f"Ajuste Linear: y = {a:.2f}x + {b:.2f}", color="red")
        plt.xlabel("x")
        plt.ylabel("y")
        plt.legend()
        plt.title("Ajuste Linear")
        plt.show()

# MAIN
fig, ax = plt.subplots()
fig.canvas.mpl_connect('close_event', acharFuncao)

ani = FuncAnimation(fig, plotagem, interval=100)
plt.show()
