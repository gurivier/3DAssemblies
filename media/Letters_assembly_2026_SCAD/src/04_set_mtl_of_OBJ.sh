#!/bin/sh

for f in `ls *.obj`;
do
    name=`basename $f .obj`

    pattern1='mtllib '$name'.mtl'
    replace1='mtllib colors.mtl'

    pattern2='o '$name
    replace2='o '$name"\n"'usemtl color_fad82c'
    
    cat $f | sed "s/$pattern1/$replace1/g" | sed "s/$pattern2/$replace2/g" > ../$f
done
