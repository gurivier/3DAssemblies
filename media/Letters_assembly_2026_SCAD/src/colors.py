#!/usr/bin/python3

def print_newmtl(name, hexa, rgbu):
    print(f'#{name}')
    print(f'newmtl color_{hexa}')
    print(f'Kd {rgbu[0]}, {rgbu[1]}, {rgbu[2]}')
    print(f'Tr 0.0')
    print(f'd 1.0')
    
colors = [
    { 'hexa': 'fad82c', 'rgb8': [250, 216,  44], 'name': 'metal' },
    { 'hexa': 'ec4538', 'rgb8': [236,  69,  56], 'name': 'text' },
    { 'hexa': '0ac90a', 'rgb8': [ 10, 201,  10], 'name': 'part left' },
    { 'hexa': '008080', 'rgb8': [  0, 128, 128], 'name': 'broken' },
    { 'hexa': '0a0a64', 'rgb8': [ 10,  10, 100], 'name': 'part right upper' },
    { 'hexa': '0a0ac9', 'rgb8': [ 10,  10, 201], 'name': 'part right lower' }
]

for color in colors:
    rgbu = [ "%.17f" % (x / 255.0) for x in color['rgb8']]
    print_newmtl(color['name'], color['hexa'], rgbu)
