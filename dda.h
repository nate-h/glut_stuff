#ifndef DDA_H_
#define DDA_H_


#include <iostream>
#include <math.h>
#include <algorithm>    // std::swap
#include <vector>
#include "point2d.h"
#include <fstream>
#include <sstream>
#include <utility> // header for pair
#include <map>
#include <set>
#include <string>
#include <sstream>
#include "definitions.h"
#include <time.h>
#include <string.h>
#include <windows.h>    /////////////////////////////////////////////////
#include<GL/gl.h>
#include "freeglut.h"


///////////////////////////////////////////////////////////////
///////////////   drawing pixel to screen   ///////////////////
///////////////////////////////////////////////////////////////

void renderPixel(int x, int y )
{
	glBegin( GL_POINTS );

	glVertex2f(x,y);

    	glEnd();
}

///////////////////////////////////////////////////////////////
/////////////////   drawing line with dda   ///////////////////
///////////////////////////////////////////////////////////////
void ddaAlgorithm(int x1, int y1, int x2, int y2){

    float dX,dY,iSteps;
    float xInc,yInc,iCount,x,y;

    dX = x1 - x2;
    dY = y1 - y2;

    if (fabs(dX) > fabs(dY))
        iSteps = (int)fabs(dX);
    else
        iSteps = (int)fabs(dY);

    xInc = dX/iSteps;
    yInc = dY/iSteps;
    x = x1;
    y = y1;

    for (iCount=1; iCount<=iSteps; iCount++)
    {
        renderPixel((int)x, (int)y);
        x -= xInc;
        y -= yInc;
    }

	glBegin(GL_POINTS);
	glVertex2f(x1,y1);     // A
	glVertex2f( x2,y2);    // B
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}


#endif /* DDA_H_ */
