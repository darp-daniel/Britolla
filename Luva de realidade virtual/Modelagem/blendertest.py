import bpy

def create_cube(name, location, scale):
    bpy.ops.mesh.primitive_cube_add(size=1, location=location)
    obj = bpy.context.object
    obj.name = name
    obj.scale = scale
    return obj

def create_hand():
    # Limpar objetos existentes
    bpy.ops.object.select_all(action='DESELECT')
    bpy.ops.object.select_by_type(type='MESH')
    bpy.ops.object.delete()
    
    # Criar a palma da mão
    palm = create_cube("Palm", (0, 0, 0), (1, 1, 0.2))
    
    # Criar dedos
    finger_positions = [
        (0.8, 0, 0.15),  # Dedo indicador
        (0.8, 0, 0.3),   # Dedo médio
        (0.8, 0, 0.45),  # Dedo anelar
        (0.8, 0, 0.6),   # Dedo mínimo
        (0.4, 0, 0.35)   # Polegar
    ]
    
    for i, pos in enumerate(finger_positions):
        create_cube(f"Finger_{i+1}", pos, (0.2, 0.2, 0.6 if i < 4 else 0.4))  # Altura dos dedos

    # Ajustar a rotação dos dedos
    bpy.data.objects["Finger_1"].rotation_euler[1] = 0.2  # Dedo indicador
    bpy.data.objects["Finger_2"].rotation_euler[1] = 0.4  # Dedo médio
    bpy.data.objects["Finger_3"].rotation_euler[1] = 0.3  # Dedo anelar
    bpy.data.objects["Finger_4"].rotation_euler[1] = 0.2  # Dedo mínimo
    bpy.data.objects["Finger_5"].rotation_euler[0] = 0.5  # Polegar

create_hand()
