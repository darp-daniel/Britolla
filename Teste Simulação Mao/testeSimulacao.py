import pygame
import sys
import math

# Configurações do Pygame
pygame.init()
screen = pygame.display.set_mode((800, 600))
pygame.display.set_caption('Simulação de Mão com Potenciômetros')

#Configuração de cores
branco = (255,255,255)
pele = (139,69,19)

#Função para desenhar a palma
def desenhar_mao(x, y):
    pygame.draw.rect(screen, pele, (x,y,100,150))
# Função para desenhar os dedos
def draw_fingers(angles):
    finger_width = 20
    finger_length = 100
    for i, angle in enumerate(angles):
        # Calcula a posição do dedo com base no ângulo
        x = 100 + i * 60
        y = 200
        end_x = x + finger_length * math.cos(math.radians(angle))
        end_y = y - finger_length * math.sin(math.radians(angle))
        
        pygame.draw.line(screen, (255, 0, 0), (x, y), (end_x, end_y), finger_width)

# Loop principal
try:
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
        screen.fill(branco)
        desenhar_mao(350,300)
        draw_fingers([90,90,90,90,90])

        pygame.display.flip()
except KeyboardInterrupt:
    pygame.quit()