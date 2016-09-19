// mounting plate for DMD

function main() {
   
   // make our outer shell
   var plate = cube({size: [31.7,28.4,1.5]});
   
   plate = plate.subtract(cylinder({r: 1.255, h:1.5}).translate([2.35,6,0]));
   plate = plate.subtract(cylinder({r: 1.255, h:1.5}).translate([29.35,6,0]));
   return plate;
   
}