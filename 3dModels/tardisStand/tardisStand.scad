
dx = 100;
dy = 100;
dz = 20;

tx = 78;
ty = 78;
tz = 6;


/**
translate([0, 0, 0]) {
  translate([37.9, -11.2, 0]) {
    %import("_baseSolid.stl");
  }
}
**/


difference() {
  cube([dx, dy, dz]);

  translate([(dx-tx)/2, (dy-ty)/2, dz-tz+1]) {
    cube([tx, ty, tz]);
  }
  translate([(dx-tx+4)/2, (dy-ty+4)/2, -50]) {
    cube([tx-4, ty-4, 100]);
  }
  translate([-dx/4,dy/4,-1]) {
    cube([tx*2, dy/2, 12]);
  }
  translate([dx/4,-dy/4,-1]) {
    cube([dx/2, ty*2, 12]);
  }

}









 