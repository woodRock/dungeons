import json
import struct
import os

def list_anims(path):
    if not os.path.exists(path):
        print(f"File not found: {path}")
        return
    with open(path, 'rb') as f:
        magic = f.read(4)
        if magic != b'glTF':
            print(f"Not a glTF file: {path}")
            return
        version = struct.unpack('<I', f.read(4))[0]
        length = struct.unpack('<I', f.read(4))[0]
        chunk_len = struct.unpack('<I', f.read(4))[0]
        chunk_type = struct.unpack('<I', f.read(4))[0]
        data = f.read(chunk_len)
        try:
            gltf = json.loads(data.decode('utf-8'))
            if 'animations' in gltf:
                print(f'Animations in {path}:')
                for anim in gltf['animations']:
                    print(f'  - {anim.get("name", "unnamed")}')
            else:
                print(f'No animations in {path}')
        except Exception as e:
            print(f"Error parsing {path}: {e}")

files = [
    'assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_General.glb',
    'assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_CombatMelee.glb',
    'assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_Simulation.glb',
    'assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_MovementBasic.glb',
    'assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_Special.glb',
    'assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_MovementAdvanced.glb'
]

for f in files:
    list_anims(f)
