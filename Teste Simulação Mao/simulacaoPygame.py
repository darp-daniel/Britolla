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

# Configurações do Pygame
pygame.init()
screen = pygame.display.set_mode((800, 600))
pygame.display.set_caption('Simulação de Mão com Potenciômetros')

branco = (255, 255, 255)
pele = (69, 69, 69)

pos_dedos = {}

# Função para desenhar a palma
def desenhar_mao(x, y):
    pygame.draw.rect(screen, pele, (x, y, 100, 150))

# Função para desenhar os dedos
def draw_fingers(ran):
    dedos = ['indicador', 'medio', 'anelar', 'minimo', 'polegar']
    x = 358
    y = 300
    distancia_dedos = 26
    finger_width = 15
    finger_length = ran
    
    for i, dedo in enumerate(dedos[:-1]):  # Para os 4 dedos principais
        pos = pygame.draw.line(screen, pele, (x, y), (x, y - finger_length), finger_width)
        pos_dedos[dedo] = pos
        x += distancia_dedos
    
    # Desenhar o polegar
    p_x = 354
    p_y = 400
    polegar = pygame.draw.line(screen, pele, (p_x, p_y), (333, 350), finger_width)
    pos_dedos['polegar'] = polegar

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
        draw_fingers(random.randrange(0,100))
        clock.tick(5)

        pygame.display.flip()
except KeyboardInterrupt:
    pygame.quit()