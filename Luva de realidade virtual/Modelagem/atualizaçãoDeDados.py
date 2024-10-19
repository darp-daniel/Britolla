import bpy
import time

def coleta_dados():
    #Coletagem de dados do sensor pelo serial
    #Retorna nova posição como objeto
    return {
        'dedo_indicador' : {'y' : None},
        'dedo_medio' : {'y' : None}
        # E assim por diante...
    }

def update_mao(data):
    # Atualizar os dedos
    dedo_indicador = bpy.data.objects.get("dedo_indicador")
    if dedo_indicador:
        dedo_indicador.location = data['dedo_indicador']['y']
    dedo_medio = bpy.data.objects.get("dedo_medio")
    if dedo_medio:
        dedo_medio.location.y = data['dedo_medio']['y']
    #E assim por diante...

def timer_func():
    # Obter os dados do sensor
    sensor_data = coleta_dados()
    
    # Atualizar a mão com os dados mais recentes
    update_mao(sensor_data)
    
    return 0.1  # Tempo em segundos para a próxima chamada

# Iniciar
bpy.app.timers.register(timer_func)
