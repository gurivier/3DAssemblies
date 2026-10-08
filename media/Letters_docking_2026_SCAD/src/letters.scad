
module wall()
{
     translate ([-20, -4, -20]) cube ([40, 8, 40]) ;
}

module letterF()
{
     translate([-5, -2, -10]) {
          cube ([4, 4, 20]) ;
          translate ([4, 0, 8]) cube ([4, 4, 4]) ;
          translate ([4, 0, 16]) cube ([6, 4, 4]) ;
     }
}
