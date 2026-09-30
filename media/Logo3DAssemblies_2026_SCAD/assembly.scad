
include<modules.scad>


//== LEFT LOGO

translate([-70, 0, 0.01])
left_block_puzzle();

//translate([-60, 0, 30])
//left_logo();

//== RIGHT LOGO

//translate([15, 0, 0])
//difference(){
//     right_block_puzzle_lower();
//     color (c_right_block_lower)
//     extended_broken_block();
//}

translate([28, 0, 30])
right_logo();

translate([15, 0, 20])
right_block_puzzle_upper();

//== BROCKEN BLOCK

//translate([60, -45, 0.1]) rotate([0, 0, 8])  broken_block_at_origin();

//== LEFT ANCHOR

//translate([-25, -30, 14.7]) rotate([0, 92.8, 0]) bolt();
//translate([-20, 35, 12.501]) rotate([0, 0, 0]) nut();
//translate([-52.5, 53.75, 10]) anchor();

//== RIGHT ANCHOR

//translate([25, 30, 14.7]) rotate([0, 92.8, 0]) bolt();
//translate([70, 30, 12.501]) rotate([0, 0, 30]) nut();
translate([52.50, 53.75, 10]) anchor();

