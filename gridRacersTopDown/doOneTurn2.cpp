bool doOneTurn (int firstCar)
{
//*  print the grid;
//*  print car info;
//*  for (int i = 0; i < 3; ++i)
//*    {
//*      int carToMove = (firstCar + i) % 3;
//*      bool quit = obtain a move (carToMove, /*+*/firstCar,/*-*/ move);
//*      if (quit) return true;
//*      bool winner = attempt a move (carToMove, move);
//*      if (winner) return true;
//*    }  
//*  return false;
}

bool obtainAMove (int carNumber, /*+*/int firstToMove,/*-*/ Move& move)
{
  if (carNumber == 1)
     return obtainHumanMove(carNumber, /*+*/int firstToMove,/*-*/ move);
  else
     return obtainComputerMove(carNumber, move);
}

bool obtainHumanMove (int carNumber, /*+*/int firstToMove,/*-*/ Move& move)
{
}

