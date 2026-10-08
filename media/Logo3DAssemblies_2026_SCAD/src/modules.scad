
c_right_block_lower = [10/255, 10/255, 200/255];
c_right_block_upper = [10/255, 10/255, 100/255];
c_text = [235/255, 69/255, 56/255];
c_left_block = [10/255, 200/255, 10/255];
c_fragment = [0.0, 0.5, 0.5];

module left_logo() {
     translate([-0.5, 0.5, 0])
          translate([-14.05, -13.85, -2])
          color (c_text)
          scale([1.2, 2, 1])
          linear_extrude(4)
          text("3D", font="Liberation Sans:style=Bold", size=14);
}

module left_block() {
     translate([-15, -30, -10]) cube ([30, 60, 40]);
     translate([5, -50, -10]) cube ([10, 100, 40]);

     difference() {
          translate([5, 30, -10]) cylinder(r=20, h=40, $fn=100);
          translate([15, -50, -11]) cube ([150, 100, 42]);
     }
     difference() {
          translate([5, -30, -10]) cylinder(r=20, h=40, $fn=100);
          translate([15, -50, -11]) cube ([150, 100, 42]);       
     }
}

module left_block_puzzle() {
     difference(){
          difference(){
               translate([-15, 0, 0])
               {
                    color (c_left_block)
                         union() {
                              left_block();
                              translate([25, 0, -10]) cylinder(r=20, h=40, $fn=100);
                         }
               }
               color(c_left_block)
                    translate([10, 0, 30]) 
                    left_logo();
          }
          color(c_left_block)
               translate([10, 0, -10]) 
               rotate([180, 0, 0])
               left_logo();
     }
}

module right_logo() {
     translate([-0.58, 0.4, 0])
          translate([-62.715, -14.29, -2])
          color (c_text)
          scale([1.2, 2, 1])
          linear_extrude(4)
          text("Assemblies", font="Liberation Sans:style=Bold", size=14);
}

module right_block() {
     translate([-70, -30, -10]) cube ([170, 60, 20]);
     translate([-70, -50, -10]) cube ([150, 100, 20]);
     
     translate([80, -30, -10]) cylinder(r=20, h=20, $fn=100);
     translate([80, 30, -10]) cylinder(r=20, h=20, $fn=100);
}

module right_block_puzzle() {
     translate([-15, 0, 0])
     {
          difference(){
               difference(){
                    difference() {
                         right_block();
                         translate([-60, 0, -11])
                              cylinder(r=20, h=22, $fn=100);
                    }
                    left_anchor_spot();
               }
               right_anchor_spot();
          }
     }
}

module right_block_puzzle_lower() {
     color (c_right_block_lower)
          difference() {
          right_block_puzzle();
          color (c_right_block_lower)
               translate([9, 0, -10])
               rotate([180, 0, 0])
               right_logo();
     }
}

module right_block_puzzle_upper() {
     translate([-15, 0, 0])
     {
          difference(){
               color (c_right_block_upper)
                    difference(){
                    difference(){
                         difference() {
                              right_block();
                              translate([-60, 0, -11])
                                   cylinder(r=20, h=22, $fn=100);
                         }
                         translate([0, 0, -20])
                              left_anchor_spot();
                    }
                    translate([0, 0, -20])
                         right_anchor_spot();
               }
               color (c_right_block_upper)
                    translate([28, 0, 10.001])
                    right_logo();
          }
     }
}

module bolt(){
     translate([0, 0, -15]){
          rotate([0, 0, 30])
               cylinder(r=6, h=5, $fn=6);
          cylinder(r=4, h=30, $fn=100);
     }
}

module nut(){
     translate([0, 0, -2.5])
          difference(){
          cylinder(r=6, h=5, $fn=6);
          translate([0, 0, -1]) cylinder(r=4, h=7, $fn=100);
     }
}

module anchor_hanger(){
     difference(){
          union(){
               translate([-5, 0, 0]) cube ([15, 5, 5]);
               translate([2.5, 5, 0]) cylinder(r=7.5, h=5, $fn=100);
          }
          translate([2.5, 5, -1]) cylinder(r=4, h=7, $fn=100);
     }     
}

module anchor(){
     translate([0, -3.75, 0])
     {
          translate([-7.5, -5, -10]) cube ([15, 5, 20]);
          translate([-2.5, 0, -10]) anchor_hanger();
          translate([-2.5, 0, 5]) anchor_hanger();
     }
}

module left_anchor_spot(){
     translate([-60, 45, 0]) cube ([15, 6, 20]);
}

module right_anchor_spot(){
     translate([45, 45, 0]) cube ([15, 6, 20]);
}

module breaking(){

     translate([120.14, -60.2, -11])
          rotate([0, 0, 90])
          cube ([20, 120, 22]);

     translate([85, -125, -11])
          rotate([0, 0, 45])
          cube ([20, 120, 22]);
     
     translate([105, -125, -11])
          rotate([0, 0, 45])
          cube ([20, 120, 22]);
     
     translate([145, -125, -11])
          rotate([0, 0, 45])
          cube ([30, 120, 22]);

     translate([201.4, -38.93, -11])
          rotate([0, 0, 90])
          cube ([20, 120, 22]);
     
     translate([175, -105, -11])
          rotate([0, 0, 45])
          cube ([30, 120, 22]);
}

module broken_block() {
     intersection(){
          right_block();
          breaking();
     }
}

module broken_block_at_origin() {
     color(c_fragment)
          translate([-85/2, 34.46, 0])
          broken_block();
}

module extended_broken_block() {
     union(){
          broken_block();
          breaking();
     }
}
