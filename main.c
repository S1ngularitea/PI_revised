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

    // check is max depth has been reached
    if (depth >= maxDepth) {
        areas.external += squareArea(square);
        return areas;
    }
    depth ++;

    // create an array to store each subsquare
    Square smallerSquares[4];

    // Divides current square into quarters
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

    // checks bottom left square, it is known that the bottom left point lies within the circle so it doesn't have to be checked
    if(psudoMagnitude(&(smallerSquares[0].p2)) < 1)
        areas.internal += squareArea(smallerSquares[0]);
    else {
        Areas recurseArea = CheckSquares(smallerSquares[0], depth, maxDepth);
        areas.internal += recurseArea.internal;
        areas.external += recurseArea.external;
    }

    // checks bottom right and top left square, both points of these squares could be either inside or outside the quarter circle so have to be fully checked
    for (int i = 1; i < 3; i++) {
        double p1_psudo_mag = psudoMagnitude(&(smallerSquares[i].p1));
        double p2_psudo_mag = psudoMagnitude(&(smallerSquares[i].p2));

        // using psudo magnitude, if magnitude is < 1 then magnitude ^2 < 1, using pythagoras magnitude = sqrt(a^2 + b^2) the square and sqrt cancel out reducing the number of calculations
        if (p2_psudo_mag < 1) {

            areas.internal += squareArea(smallerSquares[i]);
        }
        if (p2_psudo_mag > 1 && p1_psudo_mag < 1) {
            Areas recusArea = CheckSquares(smallerSquares[i], depth, maxDepth);
            areas.internal += recusArea.internal;
            areas.external += recusArea.external;
        }
        if (p1_psudo_mag > 1) {
            areas.external += squareArea(smallerSquares[i]);
        }
    }

    // check is top right square lies within the quarter circle, it is known that the top point lies ouside of the circle so it doesn't have to be checked
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

    // create start 1x1 square
    Square startSquare = generateNewSquare(newPoint(0, 0), newPoint(1, 1));

    // calculate area within quarter circle and outside of quarter circle
    Areas areas = CheckSquares(startSquare, 0, maxDepth);

    double total = areas.external+areas.internal;
    double Area = areas.internal/total;

    return Area * 4;
}

int main() {
    int max_depth = 26;

    double PiEstimate = PI(max_depth);
    printf("PI = %.10f\n", PiEstimate);
    
    return 0;
}