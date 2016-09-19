// enclosure for projector to camera adapter

function main() {
   
   // make our outer shell
   var block = cube({size: [22,40,12]});
   
   // hollow insize to teensy size
   block = block.subtract(cube({size: [18,36,12]}).translate([2,2,2]));
   
   block = block.subtract(cube({size: [10,8,2]}).rotateX(90).translate([6,2,4]));
   
   return block;
   
}
