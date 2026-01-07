#!/usr/bin/env python3
"""
Extract animation names from GLB files using pygltflib library.
"""

import sys
import json

try:
    import pygltflib
except ImportError:
    print("Installing pygltflib...")
    import subprocess
    subprocess.check_call([sys.executable, "-m", "pip", "install", "pygltflib"])
    import pygltflib

def extract_animations(glb_path):
    """Extract animation names from a GLB file."""
    try:
        gltf = pygltflib.GLTF2().load(glb_path)
        
        if gltf.animations is None or len(gltf.animations) == 0:
            print(f"No animations found in {glb_path}")
            return []
        
        animations = []
        for i, anim in enumerate(gltf.animations):
            anim_name = anim.name if anim.name else f"Animation_{i}"
            animations.append(anim_name)
            print(f"  [{i}] {anim_name}")
        
        return animations
    except Exception as e:
        print(f"Error loading {glb_path}: {e}")
        return []

def main():
    if len(sys.argv) < 2:
        print("Usage: python extract_animations.py <glb_file> [glb_file2] ...")
        print("\nExample:")
        print("  python extract_animations.py assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_General.glb")
        print("  python extract_animations.py assets/animations/Animations/gltf/Rig_Medium/Rig_Medium_Special.glb")
        return
    
    for glb_path in sys.argv[1:]:
        print(f"\nAnimations in {glb_path}:")
        animations = extract_animations(glb_path)
        if animations:
            print(f"Total: {len(animations)} animations")

if __name__ == "__main__":
    main()
