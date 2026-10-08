
# run:
# cat freecad-stl2obj-dir.py | freecad -c

import Mesh
from freecad import module_io
import importers.importOBJ

App.newDocument()

path = './'

names = []

print('begin')

with os.scandir(path) as it:
    for entry in it:
        if not entry.name.startswith('.') and entry.is_file():
            if entry.name.endswith('stl'):
                name = entry.name[:-4]
                names.append(name)
                print(f'{entry.name} : {name}')
                module_io.OpenInsertObject("Mesh", entry.name, "insert", "Unnamed")

for name in names:
    objs = []
    objs.append(FreeCAD.getDocument("Unnamed").getObject(name))
    importers.importOBJ.export(objs, f"{name}.obj")
    del objs

