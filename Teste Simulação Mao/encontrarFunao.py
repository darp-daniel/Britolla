import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit as cf
import serial
from matplotlib.animation import FuncAnimation
import time

# Definição das PORTS E BAUD_RATE
COM = 'COM5'  # Defina a porta correta
BAUD_RATE = 9600

# Conexão com o sensor
conn = serial.Serial(COM, BAUD_RATE)

leitura = []

def leituraSensor(array):
    # Espera a resposta do arduino
    if conn.in_waiting > 0:
        # Conversão para float
        linha = conn.readline().decode("utf-8").strip()
        valores = linha.split(', ')
        try:
            matriz = (float(valores[0]), float(valores[1]))
            array.append(matriz)
            if matriz[0] != 90:
                leituraSensor(array)
        except ValueError:
            print("Erro ao converter valores do sensor para float.")

def plotagem(frame):
    leituraSensor(leitura)  # Atualiza os dados do sensor, se houver dados novos
    # Separando os dados em x e y
    if leitura:
        yA, xA = zip(*leitura)
        x = np.array(xA)
        y = np.array(yA)
        
        # Plotando os dados em tempo real
        ax.clear()
        ax.scatter(x, y, label='Dados', color="red")
        ax.plot(x, y, label='Relação Angulo e Bits')
        ax.set_xlabel('Bits')
        ax.set_ylabel('Angulo')
        ax.legend()

def acharFuncao(event):
    if len(leitura) > 1:  # Verificar se há dados suficientes
        xA, yA = zip(*leitura)
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

        # Imprimindo a função
        print(f"Função: y = {a:.2f}x + {b:.2f}")

# MAIN
fig, ax = plt.subplots()
fig.canvas.mpl_connect('close_event', acharFuncao)

ani = FuncAnimation(fig, plotagem, interval=100)
plt.show()
