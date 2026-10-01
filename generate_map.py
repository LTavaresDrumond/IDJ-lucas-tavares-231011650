import random

width = 40
height = 40
layers = 2

# Valores para o map.txt (Offset +1 em relação ao código interno)
# Layer 0: Chao (1, 2, 3) -> na memoria: 0, 1, 2
# Layer 1: Cerca. 
# 0 no txt -> -1 na memoria (vazio)
# Horizontal fence: 18 (indice 17)
# Vertical fence: 11 (indice 10)

with open('Recursos/map/map.txt', 'w') as f:
    f.write(f"{width},{height},{layers},\n\n")
    
    # Layer 0: Chao de grama
    for y in range(height):
        row = []
        for x in range(width):
            r = random.random()
            if r < 0.75:
                row.append("1")   # tile 0: grama escura
            elif r < 0.92:
                row.append("2")   # tile 1: grama com flores
            else:
                row.append("3")   # tile 2: grama com objeto
        f.write(",".join(row) + ",\n")
    
    f.write("\n")
    
    # Layer 1: Cercas na borda
    for y in range(height):
        row = []
        for x in range(width):
            if y == 0 or y == height - 1:
                # Borda superior e inferior: Cerca horizontal
                row.append("18")
            elif x == 0 or x == width - 1:
                # Borda esquerda e direita: Cerca vertical
                row.append("11")
            else:
                row.append("0") # Vazio
        f.write(",".join(row) + ",\n")
    
print("Map generated! 2 layers. Layer 0: Grass. Layer 1: Fences on borders.")
