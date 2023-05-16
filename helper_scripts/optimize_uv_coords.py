# Small script to make sure UV coords aren't repeated if they are shared by a vertex
# Operates on OBJ files, tested on one of my blender outputs to optimize cube.obj
import sys

def optimize_uv():
    args = sys.argv
    file_found = False
    output_filename = "output.obj"

    for arg in args:
        if("-file=" in arg):
            file_found = True
            filename = arg.replace('-file=', '')

        if("-out=" in arg):
            output_filename = arg.replace('-out=', '')

    if not file_found:
        print(f"File {filename} Not Found")
        return

    obj_misc = []
    obj_verts = []
    obj_uvs = []
    obj_faces = []

    with open(filename) as f:
        for line in f:
            if("v " in line):
                obj_verts.append(line)
            elif("vt" in line):
                obj_uvs.append(line)
            elif("f" in line):
                obj_faces.append(line)
            else:
                obj_misc.append(line)

    obj_uvs_set = set()
    obj_uvs_list = []

    for uv in obj_uvs:
        uv_componenets = uv.rstrip('\n').split()
        uv_componenets.pop(0)
        uv_componenets = list(map(float, uv_componenets))
        obj_uvs_set.add(tuple(uv_componenets))
        obj_uvs_list.append(tuple(uv_componenets))

    uv_idx = 1
    obj_uv_map = dict()
    obj_uvs_set = list(obj_uvs_set)

    rebuilt_uv_lines = []
    for uv in obj_uvs_set:
        obj_uv_map[uv] = uv_idx
        uv_idx += 1
        rebuilt_uv_line = "vt " + ' '.join(list(map(str, list(uv)))) + '\n'
        rebuilt_uv_lines.append(rebuilt_uv_line)

    rebuilt_lines = ['']
    rebuilt_lines += obj_verts
    rebuilt_lines += rebuilt_uv_lines

    for face_line in obj_faces:
        parsed_face_line = face_line.rstrip('\n').split()
        parsed_face_line.pop(0)

        rebuilt_split_nums = []
        for face_vert in parsed_face_line:
            split_nums = list(map(int, face_vert.split('/')))
            split_nums[1] = obj_uv_map[obj_uvs_list[split_nums[1] - 1]]
            rebuilt_split_nums.append('/'.join(list(map(str, split_nums))))

        rebuilt_lines.append('f ' + ' '.join(rebuilt_split_nums) + '\n')

    with open(output_filename, 'w') as f:
        f.write(''.join(rebuilt_lines))

    print(f"File written to {output_filename}")
optimize_uv()