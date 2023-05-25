import bpy
import bmesh
import struct
import binascii
import string

import mathutils
import math

import sys
import os

argv = sys.argv
argv = argv[argv.index("--") + 1:]

filename = ""
code = '<'

vertexanimation = True
triangulate     = True  # Indices will not be exported when this is true

if (len(argv) >= 1):
    filename = argv[0]
    if (len(argv) == 2):
        code = argv[1]
else:
    error = True

# 3D file format, per-vertex animation support

objects = bpy.context.visible_objects

error = False

file   = []
images = []

images.append("{")

tokens = {
    "mesh": 0x00,
    "texture": 0x01,
    "triangle": 0x02,
    "uv": 0x03,
    "frame": 0x04,
    "vertex": 0x05,
    "normal": 0x06,
    "skin": 0x07,
    "joint": 0x08,
    "weight": 0x09
}

def triangulate_object(obj):
    me = obj.data
    # Get a BMesh representation
    bm = bmesh.new()
    bm.from_mesh(me)
    bmesh.ops.triangulate(bm, faces=bm.faces[:])
    # V2.79 : bmesh.ops.triangulate(bm, faces=bm.faces[:], quad_method=0, ngon_method=0)
    # Finish up, write the bmesh back to the mesh
    bm.to_mesh(me)
    bm.free()

for obj in objects:
          
    if (obj.type == "MESH"):
        
        bpy.data.scenes[0].frame_set(0) 
        bpy.ops.object.mode_set(mode = 'OBJECT') # Set Object mode (not EDIT)
        
        uvs = []
        
        triangulate_object(obj)
         
        # Write the attributes of object being written to file
        s = struct.Struct(code + 'I I I I')
        values = (tokens["mesh"], len(obj.data.vertices), len(obj.data.polygons), bpy.data.scenes[0].frame_end)
        print(code + " " + obj.name + " " + str(len(obj.data.vertices)) + " " + str(len(obj.data.polygons)) + " " + str(bpy.data.scenes[0].frame_end))
        file.append(s.pack(*values))
        
        #file.append("m %s %i %i\n" % (obj.name, len(obj.data.vertices), len(obj.data.polygons)))
        
        # Write texture paths
        for mat_slot in obj.material_slots:
            if mat_slot.material.node_tree.nodes:
                for texture in mat_slot.material.node_tree.nodes:
                    if texture.type == 'TEX_IMAGE':
                        file.append(struct.pack(code + 'I', tokens["texture"]))
                        filepath  = texture.image.filepath.replace('//','/')
                        directory = os.path.dirname(bpy.data.filepath[bpy.data.filepath.index("data") + len("data/"):])
                        json = "\"filename\": \"" + directory + filepath + "\""
                        if (json not in images):
                            images.append(json)
                        path = bytes(directory + filepath, 'utf-8')
                        file.append(struct.pack(code + "I%ds" % (len(path),), len(path), path))
        
        # Write all indices per triangle 
        for poly in obj.data.polygons:
            
            v = poly.vertices
            
            if len(v) != 3:
                self.report({'ERROR'}, "Polygons must consist of in total 3 (three) points each.")
                error = True
            
            s = struct.Struct(code + 'I I I I')
            values = (tokens["triangle"], v[0], v[1], v[2]) # poly.index
            file.append(s.pack(*values))
            
        depgraph = bpy.context.evaluated_depsgraph_get()
        
        # Find uvs
        bm = bmesh.new()
        bm.verts.ensure_lookup_table()
        
        bm.from_object(obj, depgraph)
        uv_layer = bm.loops.layers.uv.verify()
        
        for face in bm.faces:
            for loop in face.loops:
                uv = loop[uv_layer].uv
                uv_obj = {
                    "index": loop.vert.index,#v[loop % 3],
                    "uv": uv
                }
                uvs.append(uv_obj)
        
        # Write all UV co's per vertex
        list = []
        for uv in uvs:
            if uv["index"] not in list:
                s = struct.Struct(code + 'I I f f')
                values = (tokens["uv"], uv["index"], uv["uv"].x, uv["uv"].y)
                file.append(s.pack(*values))
        
        # Write all vertices with index and frame number
        for frame in range(bpy.data.scenes[0].frame_end + 1):
            # Record bones
            # Record mesh data (vertex animation records mesh data per frame)
            if (vertexanimation == True or frame == 1):
                bm = bmesh.new()
                bm.verts.ensure_lookup_table()
                
                bm.from_object(obj, depgraph)
                bmesh.ops.transform(bm, matrix=obj.matrix_world, verts=bm.verts)
                
                s = struct.Struct(code + 'I I')
                values = (tokens["frame"], bpy.data.scenes[0].frame_current)
                file.append(s.pack(*values))
                
                for i, v in enumerate(bm.verts):
                    s = struct.Struct(code + 'I f f f')
                    values = (tokens["normal"], v.normal.x, v.normal.y, v.normal.z)
                    file.append(s.pack(*values))

                    s = struct.Struct(code + 'I f f f')
                    values = (tokens["vertex"], v.co.x, v.co.y, v.co.z) # v.index
                    file.append(s.pack(*values))
                
                bpy.data.scenes[0].frame_set(frame + 1)

images.append("}")

if error == False:
    with open(filename, "wb") as f:
        for s in file:
            f.write(s)
    with open(bpy.data.filepath + "_textures.json", "w") as f:
        for s in images:
            f.write(s)
            f.write("\n")
