#!/bin/sh

for filename in `ls letter*.scad`;
do
    outfilename=`basename $filename .scad`".stl"
    openscad -o $outfilename $filename 2> /dev/null
done
