import pygame
import sys
import random
import time
import serial
import math
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import csv
import numpy as np

#COM
SERIAL_PORT = 'COM5'
BAUD_RATE = 9600

#CONEXAO

#controle = 0
conser = serial.Serial(SERIAL_PORT, BAUD_RATE)

def leiturasensor():
    linha = conser.readline().decode('utf-8').strip()
    valores = linha.split(', ')
    matriz = np.full(5, float(valores[1]))
    return matriz

# Configurações do Pygame
pygame.init()
screen = pygame.display.set_mode((800, 600))
pygame.display.set_caption('Simulação de Mão com Potenciômetros')

# Configuração de cores
branco = (255, 255, 255)
pele = (139, 69, 19)

# Dicionário com a configuração dos dedos
pos_dedos = {}

# Mudar tom
def mudar_cor(angles):
    tons_pele = [
        (173, 216, 230),  # Azul claro
        (0, 0, 139),      # Azul escuro
        (255, 99, 71),    # Vermelho claro
        (139, 0, 0),      # Vermelho escuro
        (128, 0, 128)     # Roxo
    ]
    return [tons_pele[min(int(angle // 18), len(tons_pele) - 1)] for angle in angles]

# Função para desenhar a palma
def desenhar_mao(x, y):
    pygame.draw.rect(screen, pele, (x, y, 100, 150))

# Função para desenhar os dedos
def draw_fingers(angles):
    dedos = ['indicador', 'medio', 'anelar', 'minimo', 'polegar']
    x = 358
    y = 300
    distancia_dedos = 26
    finger_width = 15
    finger_length = 100
    angulos = mudar_cor(angles)
    
    for i, dedo in enumerate(dedos[:-1]):  # Para os 4 dedos principais
        cor = angulos[i]
        pos = pygame.draw.line(screen, cor, (x, y), (x, y - finger_length), finger_width)
        pos_dedos[dedo] = pos
        x += distancia_dedos
    
    # Desenhar o polegar
    p_x = 354
    p_y = 400
    cor_p = angulos[4]
    polegar = pygame.draw.line(screen, cor_p, (p_x, p_y), (333, 350), finger_width)
    pos_dedos['polegar'] = polegar

# Loop principal
clock = pygame.time.Clock()
try:
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

        screen.fill(branco)
        desenhar_mao(350, 300)

        # Gerar novos ângulos e desenhar dedos
        angles = leiturasensor()
        draw_fingers(angles)
        print(mudar_cor(angles))
        clock.tick(60)

        pygame.display.flip()
except KeyboardInterrupt:
    pygame.quit()
