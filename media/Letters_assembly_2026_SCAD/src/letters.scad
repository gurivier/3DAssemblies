
module lettre(l){
     color ([1, 0, 0])
          linear_extrude(4)
          text(l, font="Liberation Sans:style=Bold", size=14);
}

module cutter(l, i, j)
{
     intersection() {
          if (i == 0) {
               translate([0, -1, -1]) cube([5, 16, 6]);
          }
          else if (i == 1) {
               translate([5, -1, -1]) cube([5, 16, 6]);
          }
          else {
               translate([10, -1, -1]) cube([5, 16, 6]);
          }

          if (j == 0) {
               translate([0, -1, -1]) cube([16, 6, 6]);
          }
          else if (j == 1) {
               translate([0, 5, -1]) cube([16, 5, 6]);
          }
          else {
               translate([0, 10, -1]) cube([16, 5, 6]);
          }
          
          lettre(l);
     }
}
