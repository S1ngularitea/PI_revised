#include <stdio.h>


typedef struct Position {
    double x,y;
} Position;

typedef struct Square {
    Position p1, p2;
} Square;

typedef struct Areas {
    double internal, external;
} Areas;

Square generateNewSquare(Position p1, Position p2) {
    Square returnSquare;
    returnSquare.p1 = p1;
    returnSquare.p2 = p2;

    return returnSquare;
}

Position newPoint(double x, double y) {
    Position returnPos;
    returnPos.x = x;
    returnPos.y = y;
    return returnPos;
}

void GenerateSmallerSquares(Square originalSquare, Square* SquareArray) {
    Position centralPoint = {
        (originalSquare.p1.x + originalSquare.p2.x)/2,
        (originalSquare.p1.y + originalSquare.p2.y)/2
    };

    // Bottom Left
    (SquareArray)[0] = generateNewSquare(originalSquare.p1, centralPoint);
    // Bottom Right
    (SquareArray)[1] = generateNewSquare(
        newPoint(centralPoint.x     , originalSquare.p1.y),
        newPoint(originalSquare.p2.x, centralPoint.y));
    // Top Left
    (SquareArray)[2] = generateNewSquare(
        newPoint(originalSquare.p1.x, centralPoint.y),
        newPoint(centralPoint.x     , originalSquare.p2.y)
    );
    // Top Right
    (SquareArray)[3] = generateNewSquare(centralPoint, originalSquare.p2);
}

double squareArea(Square square) {
    double xDif = square.p2.x-square.p1.x;
    double yDif = square.p2.y-square.p1.y;
    return xDif*yDif;
}

double psudoMagnitude(Position* vector)  {
    return vector->x * vector->x + vector->y * vector->y;
}

Areas CheckSquares(Square square, int depth, int maxDepth) {
    Areas areas = {0.0f, 0.0f};

    if (depth >= maxDepth) {
        areas.external += squareArea(square);
        return areas;
    }
    depth ++;
    Square smallerSquares[4];

    GenerateSmallerSquares(square, smallerSquares);


    /*for (int i = 0; i < 4; i++) {
        if (magnitude(&(smallerSquares[i].p2)) < 1) {
            areas.internal += squareArea(smallerSquares[i]);
            //printf("add %d\n", i);
        }
        if (magnitude(&(smallerSquares[i].p2)) > 1 && magnitude(&(smallerSquares[i].p1)) < 1) {
            //printf("recurse %d\n", i);
            Areas recusArea = CheckSquares(smallerSquares[i], depth, maxDepth);
            areas.internal += recusArea.internal;
            areas.external += recusArea.external;
        }
        if (magnitude(&(smallerSquares[i].p1)) > 1) {
            //printf("culled %d\n", i);
            areas.external += squareArea(smallerSquares[i]);
        }
    }*/

   // Code below is the same as the code above but optimised to reduce then number of redundant comparisons

    if(psudoMagnitude(&(smallerSquares[0].p2)) < 1)
        areas.internal += squareArea(smallerSquares[0]);
    else {
        Areas recurseArea = CheckSquares(smallerSquares[0], depth, maxDepth);
        areas.internal += recurseArea.internal;
        areas.external += recurseArea.external;
    }

    for (int i = 1; i < 3; i++) {


        // using psudo magnitude, if magnitude is < 1 then magnitude ^2 < 1, using pythagoras magnitude = sqrt(a^2 + b^2) the square and sqrt cancel out reducing the number of calculations
        if (psudoMagnitude(&(smallerSquares[i].p2)) < 1) {

            areas.internal += squareArea(smallerSquares[i]);
        }
        if (psudoMagnitude(&(smallerSquares[i].p2)) > 1 && psudoMagnitude(&(smallerSquares[i].p1)) < 1) {
            Areas recusArea = CheckSquares(smallerSquares[i], depth, maxDepth);
            areas.internal += recusArea.internal;
            areas.external += recusArea.external;
        }
        if (psudoMagnitude(&(smallerSquares[i].p1)) > 1) {
            areas.external += squareArea(smallerSquares[i]);
        }
    }

    if (psudoMagnitude(&(smallerSquares[3].p1)) > 1) {
        areas.external += squareArea(smallerSquares[3]);
    } else {
        Areas recusArea = CheckSquares(smallerSquares[3], depth, maxDepth);
        areas.internal += recusArea.internal;
        areas.external += recusArea.external;
    }



    return areas;
}

double PI(int maxDepth) {

    Square startSquare = generateNewSquare(newPoint(0, 0), newPoint(1, 1));

    Areas areas = CheckSquares(startSquare, 0, maxDepth);

    double total = areas.external+areas.internal;
    double Area = areas.internal/total;

    return Area * 4;
}

int main() {
    int max_depth = 30;

    double PiEstimate = PI(max_depth);
    printf("PI = %.10f\n", PiEstimate);
    
    return 0;
}