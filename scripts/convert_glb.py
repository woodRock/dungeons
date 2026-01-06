import os
import json
import struct
import sys

def convert_glb_to_obj(glb_path, obj_path):
    with open(glb_path, 'rb') as f:
        # Read Header
        magic = f.read(4)
        if magic != b'glTF':
            return False
        version = struct.unpack('<I', f.read(4))[0]
        length = struct.unpack('<I', f.read(4))[0]

        # Read JSON chunk
        chunk_length = struct.unpack('<I', f.read(4))[0]
        chunk_type = struct.unpack('<I', f.read(4))[0]
        if chunk_type != 0x4E4F534A: # JSON
            return False
        json_data = json.loads(f.read(chunk_length).decode('utf-8'))

        # Read BIN chunk
        chunk_length = struct.unpack('<I', f.read(4))[0]
        chunk_type = struct.unpack('<I', f.read(4))[0]
        if chunk_type != 0x004E4942: # BIN
            return False
        bin_data = f.read(chunk_length)

    # Simplified: Extract all meshes into one OBJ
    vertices = []
    normals = []
    uvs = []
    indices = []
    
    v_offset = 1
    
    with open(obj_path, 'w') as out:
        out.write(f"# Converted from {glb_path}\n")
        
        for mesh in json_data.get('meshes', []):
            for prim in mesh.get('primitives', []):
                attrs = prim.get('attributes', {})
                pos_acc_idx = attrs.get('POSITION')
                norm_acc_idx = attrs.get('NORMAL')
                uv_acc_idx = attrs.get('TEXCOORD_0')
                ind_acc_idx = prim.get('indices')
                
                if pos_acc_idx is None: continue
                
                # Helper to get data from accessor
                def get_data(acc_idx):
                    acc = json_data['accessors'][acc_idx]
                    bv = json_data['bufferViews'][acc['bufferView']]
                    offset = bv.get('byteOffset', 0) + acc.get('byteOffset', 0)
                    count = acc['count']
                    
                    # Component Types
                    # 5120: BYTE, 5121: UBYTE, 5122: SHORT, 5123: USHORT, 5125: UINT, 5126: FLOAT
                    comp_type = acc['componentType']
                    type_map = {5120: 'b', 5121: 'B', 5122: 'h', 5123: 'H', 5125: 'I', 5126: 'f'}
                    type_char = type_map[comp_type]
                    
                    # Element Types
                    type_counts = {'SCALAR': 1, 'VEC2': 2, 'VEC3': 3, 'VEC4': 4, 'MAT4': 16}
                    elem_count = type_counts[acc['type']]
                    
                    fmt = '<' + (type_char * elem_count)
                    stride = bv.get('byteStride', struct.calcsize(fmt))
                    
                    result = []
                    for i in range(count):
                        data = struct.unpack_from(fmt, bin_data, offset + i * stride)
                        result.append(data)
                    return result

                pos_data = get_data(pos_acc_idx)
                norm_data = get_data(norm_acc_idx) if norm_acc_idx is not None else []
                uv_data = get_data(uv_acc_idx) if uv_acc_idx is not None else []
                ind_data = get_data(ind_acc_idx) if ind_acc_idx is not None else []
                
                # Write to OBJ
                for v in pos_data:
                    out.write(f"v {v[0]} {v[1]} {v[2]}\n")
                for n in norm_data:
                    out.write(f"vn {n[0]} {n[1]} {n[2]}\n")
                for u in uv_data:
                    out.write(f"vt {u[0]} {u[1]}\n")
                
                # Indices (assuming triangles)
                curr_indices = [idx[0] for idx in ind_data]
                for i in range(0, len(curr_indices), 3):
                    # OBJ indices are 1-based. v/vt/vn
                    # We assume pos, norm, uv have same count and order for simplicity
                    idx1 = curr_indices[i] + v_offset
                    idx2 = curr_indices[i+1] + v_offset
                    idx3 = curr_indices[i+2] + v_offset
                    out.write(f"f {idx1}/{idx1}/{idx1} {idx2}/{idx2}/{idx2} {idx3}/{idx3}/{idx3}\n")
                
                v_offset += len(pos_data)
                
    return True

def main():
    paths = [
        ('assets/adventurers/Characters/gltf/', 'assets/adventurers/Characters/obj/'),
        ('assets/skeletons/characters/gltf/', 'assets/skeletons/characters/obj/')
    ]
    
    for src_dir, dst_dir in paths:
        if not os.path.exists(src_dir): continue
        if not os.path.exists(dst_dir): os.makedirs(dst_dir)
        
        for f in os.listdir(src_dir):
            if f.endswith('.glb'):
                src = os.path.join(src_dir, f)
                dst = os.path.join(dst_dir, f.replace('.glb', '.obj'))
                print(f"Converting {src} to {dst}...")
                convert_glb_to_obj(src, dst)

if __name__ == '__main__':
    main()
