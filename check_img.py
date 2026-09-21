import struct
with open('Recursos/img/Player.png', 'rb') as f:
    data = f.read(24)
    if data[:8] == b'\x89PNG\r\n\x1a\n' and data[12:16] == b'IHDR':
        w, h = struct.unpack('>LL', data[16:24])
        print(f"Width: {w}, Height: {h}")
    else:
        print("Not a valid PNG")
