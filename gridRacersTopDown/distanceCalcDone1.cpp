computeDistanceValues(RaceCourse& course)
{
// course.distances = integer matrix of same height & width as course;
// initialize(distances);
//* int d = 0;
// while (no more values can be assigned)
  /*+*///* bool noMoreCanBeDone = false;
//* while (!noMoreCanBeDone)
//* {
//*  noMoreCanBeDone = true; 
/*-*///   assign some distance values
//*  Assign d+1 to all squares adjacent to a square of value d;
//** for (x = 0; x < width of course; ++x)
//**    for (y = 0; y < height of course; ++y)
//**      {
//**        if (distances[x,y] == d)
/*+*///*            noMoreCanBeDone = false;
/*-*///**           assign d+1 to all adjacent squares to (x,y) that do
//**              not yet have a value;
//***          for (dx = -1; dx <= 1; ++dx)
//***             for (dy = -1; dy <= 1; ++dy)
//***               {
//***                 newx = x + dx; newy = y + dy;
//***                 if (isIn(course, newx,newy) 
//***                     and hasNoValue(distances, newx,newy))
//***                    assign d+1 to distance[newx,newy];
//***               }
//**      }
//*  ++d;
// }
}
