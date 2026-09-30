

//difference ()
//{
//        cube ([400, 400, 10], $fn=1000) ;
//        translate ([-10, -10, -5]) cube ([50, 50, 30], $fn=1000) ;
//}
//        
//difference ()
//{
//        cylinder (h = 40, r=10, center = true, $fn=100);
//        rotate ([90,0,0]) cylinder (h = 40, r=9, center = true, $fn=100);
//}

cube ([4, 4, 20]) ;

//translate ([4, 0, 0]) cube ([6, 4, 4]) ;

translate ([4, 0, 8]) cube ([4, 4, 4]) ;

translate ([4, 0, 16]) cube ([6, 4, 4]) ;

//text("OpenSCAD");
