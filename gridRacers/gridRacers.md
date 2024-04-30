title: Semester Project Description
toc: yes

%define {\spc} {} {<span style="visibility: hidden;">~</span>}

_Grid Racers_ is a game based upon an old pencil-and-graph-paper
game. In this project you will develop a program to play a game
against a human opponent.


# The Game 

Consider a race track laid out on a rectangular grid.

```
xxxxXxxxxXxxxxXxxxxXxxxxXxxxxX
xxxxxx      xxxxx      xxx  xx
xxxxx        xxx       xx    x
xx     xx     x        x     x
X      xx              x  x  x
x      xx                 x  x
x      xxxxxxxxxxxxxx     x  x
x     xxxxxxxxxxxxxxxxxxxxx  x
xFFFF x  xxxxxxxxxxxxxx      x
XFFFF x    xxxxxxxx          x
x     x      1               x
xxxxxxx      2        xxx    x
xxxxxx       3         x     x
x                            x
Xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

An `x` or `X` denotes a wall or barrier of some kind. The numbers `1`,
`2`, and `3` denote the positions of each of three different cars. The
`F` symbols represent the finish position of the race.

Each car has a current _position_ expressed as a pair of
integers $(x,y)$.  These represent coordinates in the race track,
assuming that $(0,0)$ denotes the lower-left corner.

Each car has a _velocity_, expressed as a pair of
integers $(dx,dy)$. The velocity composnents can be positive, zero, or
negative. On each turn, the driver of the car may keep its
velocity unchanged or may add or subtract 1 from either $dx$ or $dy$,
but not both. The car then moves (or, at least, attempts to move) to
position $(x+dx, y+dy)$. 

The _speed_ of the car is the sum of the absolute values of
its velocity components.  Each car has a maximum speed. At the start
of the race, this maximum speed is 5, but it can be reduced during the
course of the race due to accidents and collisions. 

A car may move from $(x_1,y_1)$ to $(x_2, y_2)$ only the destination
point lies within the bounds of the course and if all grid squares
whose interior is touched by the line between the center of those two
squares are clear of walls and other cars. Refer [here](#determiningifamoveisclear) for details.
If a car attempts a move
that fails this condition, it remains where it is and its velocity is
immediately set to (0,0). If the failure can be attributed to the
presence of a wall within that rectangle (regardless of the position
of other cars) or an attempt to leave the course entirely, the car is
`damaged` and its maximum speed is reduced by 1. The maximum speed
can, however, never be dropped below 1.

The winner is the first car to finish a turn on any of the `F` positions.


# Managing the Game 
     
Each game is a single race. There will be three cars in each race. The
computer drives cars 1 and 3. A human player will drive car 2.

The program will read a racecourse from
a file named as a command line parameter. Thereafter, it will take
input from the human driver from the standard input stream. All output
will be sent to the standard output stream. 

The entire program must implement 3 distinct phases: pre-game, game, 
and post-game.
   
  
## Pre-Game
     
In the pre-game phase, the program should read the race course from a
file named as a command line parameter. E.g.,

```
./gridRacer course23.dat
```

The course format is as shown earlier.
All lines in the input must be of the same length and must begin and
end with a wall (`X` or `x`). The first and last line must consist
entirely of wall characters. The end of the course input is signaled
by the end of the file.  Each line of text must be properly terminated.

During this phase, as an aid to the computer player, the program
should compute the distance of each square from the closest Finish
square and store these [distance values](#distancefromthefinish) for later use.


## Game
     
The game phase consists of multiple turns. On the first term, the cars
move in the order 1, 2, 3. On the next turn, they move in the order 2,
3, 1. On the third turn they move in the order 3, 1, 2. Subsequent
turns continue cycling through those three options.

Each turn consists of

  1. [Printing the grid] (#printingthegrid)
  2. [Printing the car information](#printingthecar)
  3. For each car, in the order explained above, [obtaining](#obtainingamove) and
     [attempting](#attemptingamove) a move.
  
These are described in more detail below. During the third step, if
any car lands on an `F` square, or if an attempt to obtain a move from
the human player fails due to an end-of-input condition, the program
moves to the post-game phase.


## Post-Game
     
In the post-game phase, the program prints the grid as described
above, then terminates.


# Detailed Operations

This section fills in the detail for a number of operations mentioned
in [above](#managingthegame).


## Determining if a Move is Clear 

A car may from one position to another only if the path is clear of
walls and other cars.

For the purpose of this program, that means that we draw a line
segment between the center of the starting and destination squares
(e.g., for a square at coordinates $(x,y)$ the center is at $(x+0.5,
y+0.5)$). Any squares whose _interior_ is touched by that line
must be free of walls and other cars.

\bPicOnRight(clearPath,40)

For example, for a car with velocity (4,1) we would want to check the
shaded squares indicated in the picture shown here.

There are several ways to determine which squares must be checked under
this rule. One would be to adapt the technique from an earlier
assignment and to note that a square needs to be checked only if any two
of its vertices lie on opposite sides of the line segment.

Another approach is to `walk` along the line using the parametric
form of the line segment equation. Suppose we have a line segment between
points $(x_1, y_1)$ and $(x_2, y_2)$. Then let $t$ be a number in the
range $0.0\ldots 1.0$ (inclusive). The point $(x,y)$ given by

\\[ x = x_1 + t (x_2 - x_1) \\]
\\[ y = y_1 + t (y_2 - y_1) \\]

is a point lying somewhere along that line segment. By advancing $t$ in
small increments (say, $0.01$) from $0.0$ to $1.0$, we ``walk` the
line segment from the first point to the second.

Ignore all $(x,y)$ points obtained in this manner that are very close
to a border.  For example, if $x$ or $y$ is within $0.01$ of an
integer value, ignore them. The remaining points will be in the
interior of the square $(\lfloor x \rfloor, \lfloor y
\rfloor)$. [^floorfunction]

[^floorfunction]: $\lfloor~\rfloor$ denotes the `floor` or
integer truncation of a floating point number.

## Distance from the Finish

Each wall square is assigned a value of 1,000,000. Each `F` square is
assigned a value of 0.

The remaining squares are assigned values indicating their
`distance` from an `F` square as follows:

  * Each square that does not yet have a value, but that is adjacent
    (horizontally, vertically, or diagonally) to a square valued at 0, is
    assigned a value of 1.
  * Then, each square that does not yet have a value, but that is adjacent
    (horizontally, vertically, or diagonally) to a square valued at 1, is
    assigned a value of 2.
  * Then, each square that does not yet have a value, but that is adjacent
    (horizontally, vertically, or diagonally) to a square valued at 2, is
    assigned a value of 3.

This process continues until no more values can be assigned.

For example, suppose that we started with the race course

```
XXXXXXXXXXX
X      XFFX
X      X  X
X     XX  X
X  XXXX   X
X1        X
X2        X
X3        X
XXXXXXXXXXX
```

The initial setup can be visualized as the matrix

|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| M | M | M | M | M | M | M | M | M | M | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 0 | 0 | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | \spc | \spc | M |
| M | \spc | \spc | \spc | \spc | \spc | M | M | \spc | \spc | M |
| M | \spc | \spc | M | M | M | M | \spc | \spc | \spc | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | M | M | M | M | M | M | M | M | M | M |


where `M` stands for 1,000,000, and an empty space denotes a position
that has not been assigned a value yet. (I would probably use -1 to
denote this when coding it.)

Then the successive steps of this procedure would be

|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| M | M | M | M | M | M | M | M | M | M | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 0 | 0 | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 1 | 1 | M |
| M | \spc | \spc | \spc | \spc | \spc | M | M | \spc | \spc | M |
| M | \spc | \spc | M | M | M | M | \spc | \spc | \spc | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | M | M | M | M | M | M | M | M | M | M |

~

|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| M | M | M | M | M | M | M | M | M | M | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 0 | 0 | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 1 | 1 | M |
| M | \spc | \spc | \spc | \spc | \spc | M | M | 2 | 2 | M |
| M | \spc | \spc | M | M | M | M | \spc | \spc | \spc | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | M | M | M | M | M | M | M | M | M | M |

~

|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| M | M | M | M | M | M | M | M | M | M | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 0 | 0 | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 1 | 1 | M |
| M | \spc | \spc | \spc | \spc | \spc | M | M | 2 | 2 | M |
| M | \spc | \spc | M | M | M | M | 3 | 3 | 3 | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | M | M | M | M | M | M | M | M | M | M |

~

|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| M | M | M | M | M | M | M | M | M | M | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 0 | 0 | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 1 | 1 | M |
| M | \spc | \spc | \spc | \spc | \spc | M | M | 2 | 2 | M |
| M | \spc | \spc | M | M | M | M | 3 | 3 | 3 | M |
| M | \spc | \spc | \spc | \spc | \spc | 4 | 4 | 4 | 4 | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | M | M | M | M | M | M | M | M | M | M |

~

|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| M | M | M | M | M | M | M | M | M | M | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 0 | 0 | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 1 | 1 | M |
| M | \spc | \spc | \spc | \spc | \spc | M | M | 2 | 2 | M |
| M | \spc | \spc | M | M | M | M | 3 | 3 | 3 | M |
| M | \spc | \spc | \spc | \spc | 5 | 4 | 4 | 4 | 4 | M |
| M | \spc | \spc | \spc | \spc | 5 | 5 | 5 | 5 | 5 | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | \spc | M |
| M | M | M | M | M | M | M | M | M | M | M |

~

|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| M | M | M | M | M | M | M | M | M | M | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 0 | 0 | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 1 | 1 | M |
| M | \spc | \spc | \spc | \spc | \spc | M | M | 2 | 2 | M |
| M | \spc | \spc | M | M | M | M | 3 | 3 | 3 | M |
| M | \spc | \spc | \spc | 6 | 5 | 4 | 4 | 4 | 4 | M |
| M | \spc | \spc | \spc | 6 | 5 | 5 | 5 | 5 | 5 | M |
| M | \spc | \spc | \spc | 6 | 6 | 6 | 6 | 6 | 6 | M |
| M | M | M | M | M | M | M | M | M | M | M |

~

|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| M | M | M | M | M | M | M | M | M | M | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 0 | 0 | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 1 | 1 | M |
| M | \spc | \spc | \spc | \spc | \spc | M | M | 2 | 2 | M |
| M | \spc | \spc | M | M | M | M | 3 | 3 | 3 | M |
| M | \spc | \spc | 7 | 6 | 5 | 4 | 4 | 4 | 4 | M |
| M | \spc | \spc | 7 | 6 | 5 | 5 | 5 | 5 | 5 | M |
| M | \spc | \spc | 7 | 6 | 6 | 6 | 6 | 6 | 6 | M |
| M | M | M | M | M | M | M | M | M | M | M |

~

|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| M | M | M | M | M | M | M | M | M | M | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 0 | 0 | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 1 | 1 | M |
| M | \spc | \spc | \spc | \spc | \spc | M | M | 2 | 2 | M |
| M | \spc | 8 | M | M | M | M | 3 | 3 | 3 | M |
| M | \spc | 8 | 7 | 6 | 5 | 4 | 4 | 4 | 4 | M |
| M | \spc | 8 | 7 | 6 | 5 | 5 | 5 | 5 | 5 | M |
| M | \spc | 8 | 7 | 6 | 6 | 6 | 6 | 6 | 6 | M |
| M | M | M | M | M | M | M | M | M | M | M |

~

|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| M | M | M | M | M | M | M | M | M | M | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 0 | 0 | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 1 | 1 | M |
| M | 9 | 9 | 9 | \spc | \spc | M | M | 2 | 2 | M |
| M | 9 | 8 | M | M | M | M | 3 | 3 | 3 | M |
| M | 9 | 8 | 7 | 6 | 5 | 4 | 4 | 4 | 4 | M |
| M | 9 | 8 | 7 | 6 | 5 | 5 | 5 | 5 | 5 | M |
| M | 9 | 8 | 7 | 6 | 6 | 6 | 6 | 6 | 6 | M |
| M | M | M | M | M | M | M | M | M | M | M |


~

|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| M | M | M | M | M | M | M | M | M | M | M |
| M | \spc | \spc | \spc | \spc | \spc | \spc | M | 0 | 0 | M |
| M | 10| 10| 10| 10| \spc | \spc | M | 1 | 1 | M |
| M | 9 | 9 | 9 | 10| \spc | M | M | 2 | 2 | M |
| M | 9 | 8 | M | M | M | M | 3 | 3 | 3 | M |
| M | 9 | 8 | 7 | 6 | 5 | 4 | 4 | 4 | 4 | M |
| M | 9 | 8 | 7 | 6 | 5 | 5 | 5 | 5 | 5 | M |
| M | 9 | 8 | 7 | 6 | 6 | 6 | 6 | 6 | 6 | M |
| M | M | M | M | M | M | M | M | M | M | M |

~

|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| M | M | M | M | M | M | M | M | M | M | M |
| M | 11| 11| 11| 11| 11| \spc | M | 0 | 0 | M |
| M | 10| 10| 10| 10| 11| \spc | M | 1 | 1 | M |
| M | 9 | 9 | 9 | 10| 11| M | M | 2 | 2 | M |
| M | 9 | 8 | M | M | M | M | 3 | 3 | 3 | M |
| M | 9 | 8 | 7 | 6 | 5 | 4 | 4 | 4 | 4 | M |
| M | 9 | 8 | 7 | 6 | 5 | 5 | 5 | 5 | 5 | M |
| M | 9 | 8 | 7 | 6 | 6 | 6 | 6 | 6 | 6 | M |
| M | M | M | M | M | M | M | M | M | M | M |

~

|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| M | M | M | M | M | M | M | M | M | M | M |
| M | 11| 11| 11| 11| 11| 12| M | 0 | 0 | M |
| M | 10| 10| 10| 10| 11| 12| M | 1 | 1 | M |
| M | 9 | 9 | 9 | 10| 11| M | M | 2 | 2 | M |
| M | 9 | 8 | M | M | M | M | 3 | 3 | 3 | M |
| M | 9 | 8 | 7 | 6 | 5 | 4 | 4 | 4 | 4 | M |
| M | 9 | 8 | 7 | 6 | 5 | 5 | 5 | 5 | 5 | M |
| M | 9 | 8 | 7 | 6 | 6 | 6 | 6 | 6 | 6 | M |
| M | M | M | M | M | M | M | M | M | M | M |


and so on until all squares have been assigned an integer value.


## Printing the Grid 
     
The grid is printed in the format shown [above](#thegame).  The
digits `1`, `2`, and `3` are displayed in the appropriate positions
reflecting the current positions of the cars. (Blanks are filled to
for the former positions. During debugging, however, you might find it
useful to suppress this so that you can easily see the entire path
taken by a car.)

## Printing the Car 

For each of the three cars, in order from 1 to 3, print a line in the
form

```
n: (x,y) (dx,dy) s
```

where $n$ is the car number, $(x,y)$ is the car`s position, $(dx,dy)$
is the car's current velocity, and $s$ is the car`s maximum speed.


## Obtaining a Move 

A `move` in this program consists of a suggested change to a car's
velocity.  Such a move can be expressed as a pair of integers $(\Delta
x, \Delta y)$, with each integer in the range $-1\ldots 1$ and with at
least one of these numbers being $0$.

The process for obtaining this depends on the car and on
whether the car is driven by a human or the computer.


## Obtaining a Human Driver's Move

If the human driver's car is not making the first move on this turn,
print the grid and the human's car info (in the format described earlier).

Print a prompt `Desired change in velocity:` followed by a single
blank (and no line terminator). Read a line from the standard input.
If that line consists of two integers (separated by one or more
blanks), with at least one being zero, and both in the range $-1\ldots
1$, then accept this as the human`s move.  Otherwise, print the
message

```
Not a legal change of velocity.
```

and automatically proceed with a move of $(0,0)$.


## Obtaining a Computer Driver`s Move

Depending on the car`s current and max speed values, there are
anywhere from 1 to 5 possible moves from among: $(-1,0)$, $(0, -1)$, $(0,0)$,
$(1,0)$, and $(0,1)$.

For each move that is permitted by the car's max speed, check to see
  
  * if the move is legal as described earlier.

  * If no moves are legal, then use the move $(0,0)$.

  * If exactly one move is legal, use it.

If more than one move is legal, examine the distance values of the
squares that would be reached by each move. Choose the square(s) with
the smallest possible distance values, discarding the other moves.  If
you have more than one move that takes you to this same smallest
distance value, then

  * Car 1 should take the move that maximizes its speed. If there is
    more than one such move, take the one that maximizes $|dx|$.

  * Car 3 should take the move that minimizes its speed. If there is
    more than one such move, take the one that minimizes $|dx|$.

This slight difference in `tie-breaking` rules is intended to help
separate cars 1 and 3 so that they aren`t continually bumping in to
one another. It will also give the two cars slightly different driving
styles, as car 1 favors raw speed but car 3 favors maneuverability.


## Attempting a Move

  * If a proposed move would accelerate a car past its maximum speed, print a
    message of the form

    ```
    Car n cannot go that fast.
    ```

    with appropriate replacement for the car number $n$.  Leave the car`s
    velocity unchanged and proceed as if the proposed move had been
    $(0,0)$.

  * Update the car`s velocity by adding the move value to it.

  * Check to see if the car can legally move from $(x,y)$ to
    $(x+dx,y+dy)$.

    * If that would cause the car to leave the course or to strike a
      wall (see earlier description of a legal move), print a message

        ```
        Car n has struck a wall.
        ```

        Reduce the car's max speed by 1 (but do not reduce it entirely to
        zero).  Set the car`s velocity to $(0,0)$ and leave the car at its
        current position.

    * If that would cause the car to bump another car (see earlier 
    description of a legal move), print a message

        ```
        Car n has bumped another car.
        ```

        Set the car`s velocity to $(0,0)$ and leave the car at its
        current position.

    * If the move is legal, update the car`s position. No message is printed.


       
    
# Example 
     
Here is a transcript of a possible game.
   
```
> ./gridRacers course0.dat
XXXXXXXXXXX
X      XFFX
X      X  X
X      X  X
X  XXXX   X
X1        X
X2        X
X3        X
XXXXXXXXXXX
1: (1,3) (0,0)
2: (1,2) (0,0)
3: (1,1) (0,0)
XXXXXXXXXXX
X      XFFX
X      X  X
X      X  X
X  XXXX   X
X 1       X
X2        X
X3        X
XXXXXXXXXXX
2: (1,2) (0,0)
Desired change in velocity: 1 0
XXXXXXXXXXX
X      XFFX
X      X  X
X      X  X
X  XXXX   X
X 1       X
X 2       X
X 3       X
XXXXXXXXXXX
1: (2,3) (1,0)
2: (2,2) (1,0)
3: (2,1) (1,0)
Desired change in velocity: 0 1
XXXXXXXXXXX
X      XFFX
X      X  X
X      X  X
X  XXXX   X
X  2      X
X  1      X
X   3     X
XXXXXXXXXXX
1: (3,2) (1,-1)
2: (3,3) (1,1)
3: (4,1) (2,0)
XXXXXXXXXXX
X      XFFX
X      X  X
X      X  X
X  XXXX   X
X  2      X
X     3   X
X    1    X
XXXXXXXXXXX
2: (3,3) (1,1)
Desired change in velocity: 0 -1
XXXXXXXXXXX
X      XFFX
X      X  X
X      X  X
X  XXXX   X
X   2     X
X     3   X
X    1    X
XXXXXXXXXXX
1: (5,1) (2,-1)
2: (4,3) (1,0)
3: (6,2) (2,1)
XXXXXXXXXXX
X      XFFX
X      X  X
X      X  X
X  XXXX   X
X   2     X
X     3   X
X      1  X
XXXXXXXXXXX
2: (4,3) (1,0)
Desired change in velocity: 1 0
XXXXXXXXXXX
X      XFFX
X      X  X
X      X  X
X  XXXX 3 X
X     2   X
X         X
X      1  X
XXXXXXXXXXX
1: (7,1) (2,0)
2: (6,3) (2,0)
3: (8,4) (2,2)
Desired change in velocity: 0 1
Car 2 has bumped another car.
XXXXXXXXXXX
X      XFFX
X      X 3X
X      X  X
X  XXXX   X
X     2   X
X        1X
X         X
XXXXXXXXXXX
1: (9,2) (2,1)
2: (6,3) (0,0)
3: (9,6) (1,2)
Car 3 has struck a wall.
Car 1 has struck a wall.
XXXXXXXXXXX
X      XFFX
X      X 3X
X      X  X
X  XXXX   X
X     2   X
X        1X
X         X
XXXXXXXXXXX
2: (6,3) (0,0)
Desired change in velocity: 1 0
XXXXXXXXXXX
X      XFFX
X      X 3X
X      X  X
X  XXXX   X
X      2  X
X        1X
X         X
XXXXXXXXXXX
1: (9,2) (0,0)
2: (7,3) (1,0)
3: (9,6) (0,0)
XXXXXXXXXXX
X      XFFX
X      X 3X
X      X  X
X  XXXX   X
X      2 1X
X         X
X         X
XXXXXXXXXXX
2: (7,3) (1,0)
Desired change in velocity: 0 1
XXXXXXXXXXX
X      XF3X
X      X  X
X      X  X
X  XXXX 2 X
X        1X
X         X
X         X
XXXXXXXXXXX
> 
```
