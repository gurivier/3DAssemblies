#!/usr/bin/python3

for l in ('A', 'B', 'C', 'D'):
    for i in range(3):
        for j in range(3):
            filename = f'letter{l}_p{i}{j}.scad'
            with open(filename, 'w') as f:
                f.write('include<letters.scad>')
                f.write(f'cutter("{l}", {i}, {j});')

