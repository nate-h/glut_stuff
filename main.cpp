// Name: Nathanial Hapeman
// Quarter Spring, Year: 2013 
// Project 1
//
// This file is to be modified by the student.
// main.cpp
////////////////////////////////////////////////////////////
//#include <GL/glut.h>

#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
//#include "point2d.h"
#include <fstream>
#include <sstream>
#include <utility>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include "dda.h"
#include "definitions.h"
#include <time.h>
#include <string.h>
#include <windows.h>    /////////////////////////////////////////////////
#include<GL/gl.h>
#include "freeglut.h"

using namespace std;

//---------------------------------------------------globals
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

int buttonState = 4;
int mouseClickCount = 0;
int levelCount = 0;
int test = 0;
int lightX = 100;
int lightY = 100;
int lightZ = -500;
int xMin;
int xMax;
int yMin;
int yMax;
int zMin;
int zMax;

double center1X = 0;
double center1Y = 0;
double center1Z = 0;
double sin5 = .08715574274;
double cos5 = .99619469809;

typedef pair<int, int> Key;
typedef map<Key, OppPts> MapPairs;
typedef MapPairs::iterator It;
MapPairs oppMap;

string fileName;

bool pressedI = false;

vector<Vector3> sumVertices;
vector<string> stringOfStates;
vector<face> faces1;
vector<vertice> vertices1;
vector<face> faces2;
vector<vertice> vertices2;
vector<face> faces3;
vector<vertice> vertices3;
vector<face> *faces = &faces1;
vector<vertice> *vertices = &vertices1;
vector<vertice> updatedVertices;

// ------------------------------------- forward declarations

void ddaAlgorithm(int x1, int y1, int x2, int y2);
void refine();
void drawTriXZ();
void drawTriXY();
void drawTriZY();
void OnMouseClick(int button, int state, int x, int y);
void generateNewPoints();
void generateNewFaces();
void updateOldVertices();
void setNewVertices();
void translating(char movement);
void translating(char movement);
void rotate(char movement);
void renderImage();
void getNormals();
void getVertexNormals();

///////////////////////////////////////////////////////////////
/////////////////////////   key event   ///////////////////////
///////////////////////////////////////////////////////////////

void keyInput(unsigned char key, int x, int y) {

	//space pressed
	if (key == 32) {
		mouseClickCount++;
		glutPostRedisplay();
	}
	// (a) level pressed
	else if (key == 65 || key == 97) {
		if (buttonState == 1)
			translating('a');
		else if (buttonState == 0)
			rotate('a');
		glutPostRedisplay();
	}
	// (s) level pressed
	else if (key == 83 || key == 115) {
		if (buttonState == 1)
			translating('s');
		else if (buttonState == 0)
			rotate('s');
		glutPostRedisplay();
	}
	// (d) level pressed
	else if (key == 68 || key == 100) {
		if (buttonState == 1)
			translating('d');
		else if (buttonState == 0)
			rotate('d');
		glutPostRedisplay();
	}
	// (w) level pressed
	else if (key == 87 || key == 119) {
		if (buttonState == 1)
			translating('w');
		else if (buttonState == 0)
			rotate('w');
		glutPostRedisplay();
	}
	//escape pressed
	else if (key == 27) {
		glutDestroyWindow(test);
	}

	// (r) rotate pressed
	else if (key == 82 || key == 114) {
		buttonState = 0;
		glutPostRedisplay();
	}
	// (t) translate pressed
	else if (key == 84 || key == 116) {
		buttonState = 1;
		glutPostRedisplay();
	}
	// (i) image pressed
	else if (key == 73 || key == 105) {
		//add pong shading stuff here
		pressedI = true;
		glutPostRedisplay();
	}
	// (l) level pressed
	else if (key == 76 || key == 108) {
		++levelCount;
		int count = levelCount % 3;
		std::cout << "Changing sublevel: " << count << std::endl;
		switch (count) {
		case 0:
			faces = &faces1;
			vertices = &vertices1;
			break;
		case 1:
			faces = &faces2;
			vertices = &vertices2;
			break;
		case 2:
			faces = &faces3;
			vertices = &vertices3;
			break;
		default:
			cout << "wtf?????" << endl;
			break;

		}
		glutPostRedisplay();
	}

}

///////////////////////////////////////////////////////////////
///////////////   Printing String to Screen   /////////////////
///////////////////////////////////////////////////////////////

void printState() {

	string action = "Action: " + stringOfStates[buttonState];
	cout << "button state: " << buttonState << endl;
	const char*pString = action.c_str();

	//get the length of the string to display
	int len = (int) strlen(pString);
	glRasterPos2f(20, 770);
	//loop to display character by character
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, pString[i]);

	//--------------------------------------------------------

	std::ostringstream ostr;
	ostr << levelCount % 3 + 1;
	string countL = ostr.str();

	string level = "Level: " + countL;
	const char*p1String = level.c_str();

	//get the length of the string to display
	int len1 = (int) strlen(p1String);
	glRasterPos2f(20, 740);
	//loop to display character by character
	for (int i = 0; i < len1; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, p1String[i]);

	//------------------------------------------------------------
	int count = mouseClickCount % 3;
	string viewState;
	switch (count) {
	case 0:
		viewState = "XY";
		break;
	case 1:
		viewState = "ZY";
		break;
	case 2:
		viewState = "XZ";
		break;
	default:
		cout << "wtf?????" << endl;
	}
	string viewAngle = "View: " + viewState;
	const char*viewPrint = viewAngle.c_str();
	//get the length of the string to display
	int len2 = (int) strlen(viewPrint);
	glRasterPos2f(20, 50);
	//loop to display character by character
	for (int i = 0; i < len2; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, viewPrint[i]);

	//-----------------------------------------------------------

	string action4 = "FileName: " + fileName;
	const char*pString4 = action4.c_str();

	//get the length of the string to display
	int len4 = (int) strlen(pString4);
	glRasterPos2f(20, 20);
	//loop to display character by character
	for (int i = 0; i < len4; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, pString4[i]);

}

///////////////////////////////////////////////////////////////
//////////////////////   refining mesh   //////////////////////
///////////////////////////////////////////////////////////////
void refine() {
	oppMap.clear();

	for (unsigned i = 0; i < faces->size(); ++i) {

		int v1 = (*faces)[i].v1;
		int v2 = (*faces)[i].v2;
		int v3 = (*faces)[i].v3;
		int min = 0;
		int max = 0;
		It it;

		///////////////////////////////////////
		min = std::min(v1, v2);
		max = std::max(v1, v2);
		Key key1 = pair<int, int>(min, max);
		it = oppMap.find(key1);
		if (it == oppMap.end()) {
			OppPts oppPts1(v3);
			oppMap.insert(pair<Key, OppPts>(key1, oppPts1));

		} else {

			if (it->second.y == -1)
				it->second.y = v3;
		}
		///////////////////////////////////////
		min = std::min(v2, v3);
		max = std::max(v2, v3);
		Key key2 = pair<int, int>(min, max);
		it = oppMap.find(key2);
		if (it == oppMap.end()) {
			OppPts oppPts2(v1);
			oppMap.insert(pair<Key, OppPts>(key2, oppPts2));

		} else {

			if (it->second.y == -1)
				it->second.y = v1;
		}
		///////////////////////////////////////
		min = std::min(v1, v3);
		max = std::max(v1, v3);
		Key key3 = pair<int, int>(min, max);
		it = oppMap.find(key3);
		if (it == oppMap.end()) {
			OppPts oppPts3(v2);
			oppMap.insert(pair<Key, OppPts>(key3, oppPts3));

		} else {

			if (it->second.y == -1)
				it->second.y = v2;
		}

	}

}

///////////////////////////////////////////////////////////////
///////////////   Generate New Points in Graph   //////////////
///////////////////////////////////////////////////////////////

void generateNewPoints() {

	for (It it = oppMap.begin(); it != oppMap.end(); ++it) {

		int v1 = it->first.first;
		int v2 = it->first.second;
		int v3 = it->second.x;
		int v4 = it->second.y;
		int newX = (int) (3.0 / 8 * (*vertices)[v1].x
				+ 3.0 / 8 * (*vertices)[v2].x + 1.0 / 8 * (*vertices)[v3].x
				+ 1.0 / 8 * (*vertices)[v4].x);
		int newY = (int) (3.0 / 8 * (*vertices)[v1].y
				+ 3.0 / 8 * (*vertices)[v2].y + 1.0 / 8 * (*vertices)[v3].y
				+ 1.0 / 8 * (*vertices)[v4].y);
		int newZ = (int) (3.0 / 8 * (*vertices)[v1].z
				+ 3.0 / 8 * (*vertices)[v2].z + 1.0 / 8 * (*vertices)[v3].z
				+ 1.0 / 8 * (*vertices)[v4].z);
		vertice addMe(newX, newY, newZ);
		vertices->push_back(addMe);
		it->second.newPoint = vertices->size() - 1;

	}

}
///////////////////////////////////////////////////////////////
///////////////   Generate New Faces in Graph   ///////////////
///////////////////////////////////////////////////////////////
void generateNewFaces() {

	//vector<face> newFaces;
	int facesSize = faces->size();
	for (int i = 0; i < facesSize; ++i) {

		int v1 = (*faces)[i].v1;
		int v2 = (*faces)[i].v2;
		int v3 = (*faces)[i].v3;

		int min1 = min(v1, v2);
		int max1 = max(v1, v2);
		Key key1 = pair<int, int>(min1, max1);
		int v1v2 = oppMap.find(key1)->second.newPoint;

		int min2 = min(v1, v3);
		int max2 = max(v1, v3);
		Key key2 = pair<int, int>(min2, max2);
		int v1v3 = oppMap.find(key2)->second.newPoint;

		int min3 = min(v2, v3);
		int max3 = max(v2, v3);
		Key key3 = pair<int, int>(min3, max3);
		int v2v3 = oppMap.find(key3)->second.newPoint;

		face addMe1(v1v2, v2v3, v1v3);
		faces->push_back(addMe1);
		face addMe2(v1, v1v2, v1v3);
		faces->push_back(addMe2);
		face addMe3(v2, v1v2, v2v3);
		faces->push_back(addMe3);
		face addMe4(v3, v1v3, v2v3);
		faces->push_back(addMe4);

	}

	faces->erase(faces->begin(), faces->begin() + facesSize);
}

///////////////////////////////////////////////////////////////
/////////////////   Updating Old Vertices   ///////////////////
///////////////////////////////////////////////////////////////

void updateOldVertices() {

	vector<set<int> > gettingValues;
	for (unsigned i = 0; i < vertices->size(); ++i) {
		set<int> addMe;
		gettingValues.push_back(addMe);
	}

	for (unsigned i = 0; i < faces->size(); ++i) {
		int v1 = (*faces)[i].v1;
		int v2 = (*faces)[i].v2;
		int v3 = (*faces)[i].v3;
		gettingValues[v1].insert(v2);
		gettingValues[v1].insert(v3);
		gettingValues[v2].insert(v1);
		gettingValues[v2].insert(v3);
		gettingValues[v3].insert(v2);
		gettingValues[v3].insert(v1);

	}
	for (unsigned i = 0; i < gettingValues.size(); ++i) {
		vertice addMe((*vertices)[i]);
		int number = gettingValues[i].size();
		double beta;
		if (number == 3)
			beta = 3.0 / 16;
		else
			beta = 3.0 / (8 * number);
		addMe *= (1.0 - beta * number);
		set<int>::iterator it = gettingValues[i].begin();
		for (; it != gettingValues[i].end(); ++it) {
			vertice newVert = (*vertices)[*it];
			newVert *= beta;
			addMe += newVert;
		}
		updatedVertices.push_back(addMe);
	}

}
///////////////////////////////////////////////////////////////
///////////////   SETIING THE NEW VERTICES   //////////////////
///////////////////////////////////////////////////////////////
void setNewVertices() {

	for (unsigned i = 0; i < updatedVertices.size(); ++i) {
		(*vertices)[i].x = updatedVertices[i].x;
		(*vertices)[i].y = updatedVertices[i].y;
		(*vertices)[i].z = updatedVertices[i].z;

	}
	updatedVertices.clear();
}

///////////////////////////////////////////////////////////////
///////////////   drawing floor to screen   ///////////////////
///////////////////////////////////////////////////////////////
void drawFloor() {

	ddaAlgorithm(100, 100, 700, 100);
	ddaAlgorithm(100, 100, 100, 700);
	ddaAlgorithm(700, 100, 700, 700);
	ddaAlgorithm(100, 700, 700, 700);

}
///////////////////////////////////////////////////////////////
////////////   TRANSLATING GRID / MOVING OBJECT   /////////////
///////////////////////////////////////////////////////////////

void translating(char movement) {

	//left right
	//up down
	int count = mouseClickCount % 3;
	switch (count) {
	case 0:
		cout << "xy: " << 1 << endl;
		if (movement == 'a') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				vertices1[i].x -= 20;
			for (unsigned i = 0; i < vertices2.size(); ++i)
				vertices2[i].x -= 20;
			for (unsigned i = 0; i < vertices3.size(); ++i)
				vertices3[i].x -= 20;
			center1X -= 20;
		} else if (movement == 'd') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				vertices1[i].x += 20;
			for (unsigned i = 0; i < vertices2.size(); ++i)
				vertices2[i].x += 20;
			for (unsigned i = 0; i < vertices3.size(); ++i)
				vertices3[i].x += 20;
			center1X += 20;
		} else if (movement == 's') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				vertices1[i].y -= 20;
			for (unsigned i = 0; i < vertices2.size(); ++i)
				vertices2[i].y -= 20;
			for (unsigned i = 0; i < vertices3.size(); ++i)
				vertices3[i].y -= 20;
			center1Y -= 20;
		} else {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				vertices1[i].y += 20;
			for (unsigned i = 0; i < vertices2.size(); ++i)
				vertices2[i].y += 20;
			for (unsigned i = 0; i < vertices3.size(); ++i)
				vertices3[i].y += 20;
			center1Y += 20;
		}

		break;
	case 1:
		cout << "zy: " << 2 << endl;
		if (movement == 'a') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				vertices1[i].z -= 20;
			for (unsigned i = 0; i < vertices2.size(); ++i)
				vertices2[i].z -= 20;
			for (unsigned i = 0; i < vertices3.size(); ++i)
				vertices3[i].z -= 20;
			center1Z -= 20;
		} else if (movement == 'd') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				vertices1[i].z += 20;
			for (unsigned i = 0; i < vertices2.size(); ++i)
				vertices2[i].z += 20;
			for (unsigned i = 0; i < vertices3.size(); ++i)
				vertices3[i].z += 20;
			center1Z += 20;
		} else if (movement == 's') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				vertices1[i].y -= 20;
			for (unsigned i = 0; i < vertices2.size(); ++i)
				vertices2[i].y -= 20;
			for (unsigned i = 0; i < vertices3.size(); ++i)
				vertices3[i].y -= 20;
			center1Y -= 20;
		} else {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				vertices1[i].y += 20;
			for (unsigned i = 0; i < vertices2.size(); ++i)
				vertices2[i].y += 20;
			for (unsigned i = 0; i < vertices3.size(); ++i)
				vertices3[i].y += 20;
			center1Y += 20;
		}

		break;
	case 2:
		cout << "xz: " << 3 << endl;
		if (movement == 'a') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				vertices1[i].x -= 20;
			for (unsigned i = 0; i < vertices2.size(); ++i)
				vertices2[i].x -= 20;
			for (unsigned i = 0; i < vertices3.size(); ++i)
				vertices3[i].x -= 20;
			center1X -= 20;
		} else if (movement == 'd') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				vertices1[i].x += 20;
			for (unsigned i = 0; i < vertices2.size(); ++i)
				vertices2[i].x += 20;
			for (unsigned i = 0; i < vertices3.size(); ++i)
				vertices3[i].x += 20;
			center1X += 20;
		} else if (movement == 's') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				vertices1[i].z -= 20;
			for (unsigned i = 0; i < vertices2.size(); ++i)
				vertices2[i].z -= 20;
			for (unsigned i = 0; i < vertices3.size(); ++i)
				vertices3[i].z -= 20;
			center1Z -= 20;
		} else {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				vertices1[i].z += 20;
			for (unsigned i = 0; i < vertices2.size(); ++i)
				vertices2[i].z += 20;
			for (unsigned i = 0; i < vertices3.size(); ++i)
				vertices3[i].z += 20;
			center1Z += 20;
		}

		break;
	default:
		cout << "wtf?????" << endl;

	}

}

///////////////////////////////////////////////////////////////
///////////////   ROTATE GRID / MOVING OBJECT   ///////////////
///////////////////////////////////////////////////////////////
void rotateX(int&y, int& z, bool add) {

	double sin5Temp;
	if (add == 1)
		sin5Temp = sin5;
	else
		sin5Temp = -sin5;

	y -= int(center1Y);
	z -= int(center1Z);
	int yTemp = 0;
	int zTemp = 0;
	yTemp = int(y * cos5 - z * sin5Temp);
	zTemp = int(y * sin5Temp + z * cos5);
	y = yTemp;
	z = zTemp;
	y += int(center1Y);
	z += int(center1Z);

}
void rotateY(int& x, int& z, bool add) {

	double sin5Temp;
	if (add == 1)
		sin5Temp = sin5;
	else
		sin5Temp = -sin5;

	x -= int(center1X);
	z -= int(center1Z);

	int xTemp = 0;
	int zTemp = 0;
	xTemp = int(x * cos5 + z * sin5Temp);
	zTemp = int(-x * sin5Temp + z * cos5);
	x = xTemp;
	z = zTemp;

	x += int(center1X);
	z += int(center1Z);

}
void rotateZ(int& x, int&y, bool add) {

	double sin5Temp;
	if (add == 1)
		sin5Temp = sin5;
	else
		sin5Temp = -sin5;

	x -= int(center1X);
	y -= int(center1Y);

	int xTemp = 0;
	int yTemp = 0;
	xTemp = int(x * cos5 - y * sin5Temp);
	yTemp = int(x * sin5Temp + y * cos5);
	x = xTemp;
	y = yTemp;

	x += int(center1X);
	y += int(center1Y);

}

void rotate(char movement) {
	cout << "now rotating" << endl;
	int count = mouseClickCount % 3;
	switch (count) {
	case 0:
		cout << "xy: " << 1 << endl;
		if (movement == 'a') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				rotateY(vertices1[i].x, vertices1[i].z, true);
			for (unsigned i = 0; i < vertices2.size(); ++i)
				rotateY(vertices2[i].x, vertices2[i].z, true);
			for (unsigned i = 0; i < vertices3.size(); ++i)
				rotateY(vertices3[i].x, vertices3[i].z, true);
		} else if (movement == 'd') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				rotateY(vertices1[i].x, vertices1[i].z, false);
			for (unsigned i = 0; i < vertices2.size(); ++i)
				rotateY(vertices2[i].x, vertices2[i].z, false);
			for (unsigned i = 0; i < vertices3.size(); ++i)
				rotateY(vertices3[i].x, vertices3[i].z, false);
		} else if (movement == 's') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				rotateX(vertices1[i].y, vertices1[i].z, false);
			for (unsigned i = 0; i < vertices2.size(); ++i)
				rotateX(vertices2[i].y, vertices2[i].z, false);
			for (unsigned i = 0; i < vertices3.size(); ++i)
				rotateX(vertices3[i].y, vertices3[i].z, false);
		} else {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				rotateX(vertices1[i].y, vertices1[i].z, true);
			for (unsigned i = 0; i < vertices2.size(); ++i)
				rotateX(vertices2[i].y, vertices2[i].z, true);
			for (unsigned i = 0; i < vertices3.size(); ++i)
				rotateX(vertices3[i].y, vertices3[i].z, true);
		}

		break;
	case 1:
		cout << "zy: " << 2 << endl;
		if (movement == 'a') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				rotateY(vertices1[i].x, vertices1[i].z, true);
			for (unsigned i = 0; i < vertices2.size(); ++i)
				rotateY(vertices2[i].x, vertices2[i].z, true);
			for (unsigned i = 0; i < vertices3.size(); ++i)
				rotateY(vertices3[i].x, vertices3[i].z, true);
		} else if (movement == 'd') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				rotateY(vertices1[i].x, vertices1[i].z, false);
			for (unsigned i = 0; i < vertices2.size(); ++i)
				rotateY(vertices2[i].x, vertices2[i].z, false);
			for (unsigned i = 0; i < vertices3.size(); ++i)
				rotateY(vertices3[i].x, vertices3[i].z, false);
		} else if (movement == 's') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				rotateZ(vertices1[i].x, vertices1[i].y, false);
			for (unsigned i = 0; i < vertices2.size(); ++i)
				rotateZ(vertices2[i].x, vertices2[i].y, false);
			for (unsigned i = 0; i < vertices3.size(); ++i)
				rotateZ(vertices3[i].x, vertices3[i].y, false);
		} else {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				rotateZ(vertices1[i].x, vertices1[i].y, true);
			for (unsigned i = 0; i < vertices2.size(); ++i)
				rotateZ(vertices2[i].x, vertices2[i].y, true);
			for (unsigned i = 0; i < vertices3.size(); ++i)
				rotateZ(vertices3[i].x, vertices3[i].y, true);
		}

		break;
	case 2:
		cout << "xz: " << 3 << endl;
		if (movement == 'a') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				rotateZ(vertices1[i].x, vertices1[i].y, true);
			for (unsigned i = 0; i < vertices2.size(); ++i)
				rotateZ(vertices2[i].x, vertices2[i].y, true);
			for (unsigned i = 0; i < vertices3.size(); ++i)
				rotateZ(vertices3[i].x, vertices3[i].y, true);
		} else if (movement == 'd') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				rotateZ(vertices1[i].x, vertices1[i].y, false);
			for (unsigned i = 0; i < vertices2.size(); ++i)
				rotateZ(vertices2[i].x, vertices2[i].y, false);
			for (unsigned i = 0; i < vertices3.size(); ++i)
				rotateZ(vertices3[i].x, vertices3[i].y, false);
		} else if (movement == 's') {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				rotateX(vertices1[i].y, vertices1[i].z, false);
			for (unsigned i = 0; i < vertices2.size(); ++i)
				rotateX(vertices2[i].y, vertices2[i].z, false);
			for (unsigned i = 0; i < vertices3.size(); ++i)
				rotateX(vertices3[i].y, vertices3[i].z, false);
		} else {
			for (unsigned i = 0; i < vertices1.size(); ++i)
				rotateX(vertices1[i].y, vertices1[i].z, true);
			for (unsigned i = 0; i < vertices2.size(); ++i)
				rotateX(vertices2[i].y, vertices2[i].z, true);
			for (unsigned i = 0; i < vertices3.size(); ++i)
				rotateX(vertices3[i].y, vertices3[i].z, true);
		}

		break;
	default:
		cout << "wtf?????" << endl;

	}

}

///////////////////////////////////////////////////////////////
//////////////////  rendering to screen   /////////////////////
///////////////////////////////////////////////////////////////
void GL_render() {

	//clears buffers to present buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// drawing floor
	drawFloor();

	if (pressedI == true) {
		time_t timer;
		time(&timer);
		renderImage();

		time_t endTime;
		time(&endTime);
		pressedI = false;
		double seconds = difftime(timer, endTime);
		cout << "Time it took to render: " << -seconds << endl;
		printState();
		// swaps buffers of current window
		glutSwapBuffers();

		cout<<"stop here"<< endl;
		return;
	}

	int count = mouseClickCount % 3;
	switch (count) {
	case 0:
		cout << "blah: " << 1 << endl;
		drawTriXY();
		break;
	case 1:
		cout << "blah: " << 2 << endl;
		drawTriZY();
		break;
	case 2:
		cout << "blah: " << 3 << endl;
		drawTriXZ();
		break;
	default:
		cout << "wtf?????" << endl;

	}

	printState();
	// swaps buffers of current window
	glutSwapBuffers();

}

///////////////////////////////////////////////////////////////
////////////  getting intersection distance   /////////////////
///////////////////////////////////////////////////////////////
FaceAndDist whichFaceToRender(int rx, int ry) {

	int min = 800;
	FaceAndDist returnMe;

	for (unsigned i = 0; i < faces->size(); ++i) {
		int v1 = (*faces)[i].v1;
		int v2 = (*faces)[i].v2;
		int v3 = (*faces)[i].v3;
		//getting a cord
		int ax = (*vertices)[v1].x;
		int ay = (*vertices)[v1].y;
		int az = (*vertices)[v1].z;
		//getting b cord
		int bx = (*vertices)[v2].x;
		int by = (*vertices)[v2].y;
		int bz = (*vertices)[v2].z;
		//getting c cord
		int cx = (*vertices)[v3].x;
		int cy = (*vertices)[v3].y;
		int cz = (*vertices)[v3].z;
		int determinate = (bx - ax) * (cy - ay) - (cx - ax) * (by - ay);

		double r = 1.0 * ((rx - ax) * (cy - ay) - (cx - ax) * (ry - ay))
				/ determinate;
		double s = 1.0 * ((bx - ax) * (ry - ay) - (rx - ax) * (by - ay))
				/ determinate;

		if (r >= 0 && s >= 0 && r + s <= 1) {
			int t = (int) (az + (bz - az) * r + (cz - az) * s);
			if (t < min) {
				min = t;
				returnMe.face = i;
				returnMe.distance = t;
			}
		}

	}

	return returnMe;

}
///////////////////////////////////////////////////////////////
///////////////////   Finding Boundary Box   //////////////////
///////////////////////////////////////////////////////////////
void boundingBox() {

	xMin = (*vertices)[0].x;
	xMax = (*vertices)[0].x;
	yMin = (*vertices)[0].y;
	xMax = (*vertices)[0].y;
	for (unsigned i = 0; i < (*vertices).size(); ++i) {
		if ((*vertices)[i].x > xMax)
			xMax = (*vertices)[i].x;
		if ((*vertices)[i].x < xMin)
			xMin = (*vertices)[i].x;
		if ((*vertices)[i].y > yMax)
			yMax = (*vertices)[i].y;
		if ((*vertices)[i].y < yMin)
			yMin = (*vertices)[i].y;
	}

	cout << "yMin: " << yMin << endl;
	cout << "yMax: " << yMax << endl;
	cout << "xMax: " << xMax << endl;
	cout << "xMin: " << xMin << endl;

}
///////////////////////////////////////////////////////////////
///////////////   drawing pixel to screen   ///////////////////
///////////////////////////////////////////////////////////////

void renderColoredPixel(int x, int y, int color) {
	switch (color) {
	case -1:
		return;    // because no intersection
		break;
	case 0:
		glColor3f(0.0f, 0.0f, 0.2f);
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
		glColor3f(1.0f, 1.0f, 1.0f);
		break;
	default:
		cout << "wtf?????" << endl;
	}
}
///////////////////////////////////////////////////////////////
////////////////   CALCULATE LOCAL NORMAL    //////////////////
///////////////////////////////////////////////////////////////
int countf = 0;
void localNorm(int x, int y, int z, int faceNum, Vector3& localN) {

	int v1 = (*faces)[faceNum].v1;
	int v2 = (*faces)[faceNum].v2;
	int v3 = (*faces)[faceNum].v3;
	//getting a cord
	int ax = (*vertices)[v1].x;
	int ay = (*vertices)[v1].y;
	//int az=(*vertices)[v1].z;
	//getting b cord
	int bx = (*vertices)[v2].x;
	int by = (*vertices)[v2].y;
	//int bz=(*vertices)[v2].z;
	//getting c cord
	int cx = (*vertices)[v3].x;
	int cy = (*vertices)[v3].y;
	//int cz=(*vertices)[v3].z;
	int determinate = (bx - ax) * (cy - ay) - (cx - ax) * (by - ay);

	double r = 1.0 * ((x - ax) * (cy - ay) - (cx - ax) * (y - ay))
			/ determinate;
	double s = 1.0 * ((bx - ax) * (y - ay) - (x - ax) * (by - ay))
			/ determinate;

	if (r >= 0 && s >= 0 && r + s <= 1) {
		localN += sumVertices[v1] * (1 - r - s) + sumVertices[v2] * r
				+ sumVertices[v3] * s;
		return;
	}
	cout << "huh???" << endl;

}

///////////////////////////////////////////////////////////////
///////////////////   DIFFUSE LIGHTING   //////////////////////
///////////////////////////////////////////////////////////////
void diffuseAndSpecular(int x, int y, FaceAndDist fd, float& diffLight,
		float& specular) {
	int z = fd.distance;

	Vector3 localN(0, 0, 0);
	localNorm(x, y, z, fd.face, localN);

	float nx = localN.x;
	float ny = localN.y;
	float nz = localN.z;

	int mag = sqrt(
			(lightX - x) * (lightX - x) + (lightY - y) * (lightY - y)
					+ (lightZ - z) * (lightZ - z));

	diffLight = 1.0f * (lightX - x) / mag * nx + 1.0f * (lightY - y) / mag * ny
			+ 1.0f * (lightZ - z) / mag * nz;

	diffLight *= .5;

	// How I calculated reflected angle:  R=E-2(e.n)n
	float en = 1.0f * (lightX - x) / mag * nx + 1.0f * (lightY - y) / mag * ny
			+ 1.0f * (lightZ - z) / mag * nz;
	en *= 2;

	float nsx = en * nx;
	float nsy = en * ny;
	float nsz = en * nz;
	float ez = 1.0f * (lightZ - z) / mag - nsz;

	specular = 1.0f * ez;
	if (specular < 0 || specular > 100)
		specular = 0;

	specular = pow(specular, 4);

	//cout<<"after specular: "<<specular<<endl;

}
///////////////////////////////////////////////////////////////
//////////////  Making colorized triangles   //////////////////
///////////////////////////////////////////////////////////////
void allThree() {

	glBegin(GL_POINTS);
	for (int i = xMin; i < xMax; ++i) {
		for (int j = yMin; j < yMax; ++j) {
			FaceAndDist getFace = whichFaceToRender(i, j);
			if (getFace.face != -1) {
				float colorChange = .45f;
				float diffLight = 0.0f;
				float specular = 0.0f;
				diffuseAndSpecular(i, j, getFace, diffLight, specular);
				colorChange += diffLight;
				glColor3f(specular, specular, colorChange + specular);
				glVertex2f(i, j);

				//count++;
				//if(count%100 == 0)
				//cout<<"specu: "<<specular<<endl;
			}
		}
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();

}
///////////////////////////////////////////////////////////////
/////////////////////   drawing shadow   //////////////////////
///////////////////////////////////////////////////////////////

void shadow() {

	glBegin(GL_POINTS);
	glColor3f(0.6f, 0.6f, .6f);

	bool isIn;
	for (int i = 101; i < 700; ++i) {
		for (int j = 101; j < 700; ++j) {
			isIn = false;

			for (unsigned f = 0; f < (*faces).size(); ++f) {
				if (cramersRule((*vertices)[(*faces)[f].v1],
						(*vertices)[(*faces)[f].v2],
						(*vertices)[(*faces)[f].v3], i, j) == true) {
					isIn = true;
					break;
				}
			}

			if (isIn == false) {
				glVertex2f(i, j);
			}
		}
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();

}

///////////////////////////////////////////////////////////////
//////////////////   rendering everything   ///////////////////
///////////////////////////////////////////////////////////////

void renderImage() {

	getNormals();
	getVertexNormals();
	shadow();
	boundingBox();
	allThree();
	cout << "Done Rendering Image!\n" << endl;

}

///////////////////////////////////////////////////////////////
/////////////////   initialize attributes   ///////////////////
///////////////////////////////////////////////////////////////
void GLInit(int* argc, char** argv) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	test = glutCreateWindow("CS 130 - Project 2");

	glMatrixMode(GL_PROJECTION_MATRIX);
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
	glutDisplayFunc(GL_render);

}

///////////////////////////////////////////////////////////////
////////////////   OTHER SUBDIVISION LEVELS   /////////////////
///////////////////////////////////////////////////////////////
void gettingOtherLevels() {

	std::cout << "Faces:" << faces->size() << std::endl;
	//second level
	for (unsigned i = 0; i < vertices1.size(); ++i) {
		vertice tempVert(vertices1[i]);
		vertices2.push_back(tempVert);
	}
	for (unsigned i = 0; i < faces1.size(); ++i) {
		face tempFace(faces1[i]);
		faces2.push_back(tempFace);
	}
	//settting pointer to second sublevel
	faces = &faces2;
	vertices = &vertices2;

	refine();
	updateOldVertices();
	generateNewPoints();
	generateNewFaces();
	setNewVertices();

	//third level
	for (unsigned i = 0; i < vertices2.size(); ++i) {
		vertice tempVert(vertices2[i]);
		vertices3.push_back(tempVert);
	}
	for (unsigned i = 0; i < faces2.size(); ++i) {
		face tempFace(faces2[i]);
		faces3.push_back(tempFace);
	}
	//settting pointer to thing sublevel
	faces = &faces3;
	vertices = &vertices3;
	refine();
	updateOldVertices();
	generateNewPoints();
	generateNewFaces();
	setNewVertices();

	//settting pointer to first sublevel
	faces = &faces1;
	vertices = &vertices1;
}

///////////////////////////////////////////////////////////////
////////   CALCULATING NORMALS FOR A THREE VECTROS   //////////
///////////////////////////////////////////////////////////////
void getNormals() {

	float nx, ny, nz;
	for (unsigned i = 0; i < (*faces).size(); ++i) {
		//for x
		int x1 = (*vertices)[(*faces)[i].v1].x;
		int x2 = (*vertices)[(*faces)[i].v2].x;
		int x3 = (*vertices)[(*faces)[i].v3].x;
		int xab = x1 - x2;
		int xac = x1 - x3;

		//for y
		int y1 = (*vertices)[(*faces)[i].v1].y;
		int y2 = (*vertices)[(*faces)[i].v2].y;
		int y3 = (*vertices)[(*faces)[i].v3].y;
		int yab = y1 - y2;
		int yac = y1 - y3;

		//for z
		int z1 = (*vertices)[(*faces)[i].v1].z;
		int z2 = (*vertices)[(*faces)[i].v2].z;
		int z3 = (*vertices)[(*faces)[i].v3].z;
		int zab = z1 - z2;
		int zac = z1 - z3;

		nx = yab * zac - zab * yac;
		ny = zab * xac - xab * zac;
		nz = xab * yac - yab * xac;

		(*faces)[i].area = 100
				* sqrt(
						nx / 100 * nx / 100 + ny / 100 * ny / 100
								+ nz / 100 * nz / 100);

		double nMag = (*faces)[i].area;
		// normalizing these
		nx /= nMag;
		ny /= nMag;
		nz /= nMag;

		float nx2 = -1.0f * nx;
		float ny2 = -1.0f * ny;
		float nz2 = -1.0f * nz;

		int x = (*vertices)[(*faces)[i].v1].x;
		int y = (*vertices)[(*faces)[i].v1].y;
		int z = (*vertices)[(*faces)[i].v1].z;
		float dist1 = 100.0f
				* sqrt(
						(100 * nx + x - center1X) / 100
								* (100 * nx + x - center1X) / 100
								+ (100 * ny + y - center1Y) / 100
										* (100 * ny + y - center1Y) / 100
								+ (100 * nz + z - center1Z) / 100
										* (100 * nz + z - center1Z) / 100);
		float dist2 = 100.0f
				* sqrt(
						(100 * nx2 + x - center1X) / 100
								* (100 * nx2 + x - center1X) / 100
								+ (100 * ny2 + y - center1Y) / 100
										* (100 * ny2 + y - center1Y) / 100
								+ (100 * nz2 + z - center1Z) / 100
										* (100 * nz2 + z - center1Z) / 100);
		if (dist1 < dist2) {
			nx *= -1;
			ny *= -1;
			nz *= -1;
		}

		(*faces)[i].nx = nx;
		(*faces)[i].ny = ny;
		(*faces)[i].nz = nz;

	}

}

///////////////////////////////////////////////////////////////
///////////   CALCULATING NORMALS FOR A VERTICES   ////////////
///////////////////////////////////////////////////////////////

void getVertexNormals() {

	int counter[(*vertices).size()];

	for (unsigned i = 0; i < (*vertices).size(); ++i) {
		Vector3 addMe(0, 0, 0);
		sumVertices.push_back(addMe);
		counter[i] = 0;
	}

	for (unsigned i = 0; i < (*faces).size(); ++i) {
		float nx = (*faces)[i].nx;
		float ny = (*faces)[i].ny;
		float nz = (*faces)[i].nz;
		Vector3 addMe(nx, ny, nz);

		int a = (*faces)[i].v1;
		int b = (*faces)[i].v2;
		int c = (*faces)[i].v3;

		sumVertices[a] += addMe;
		sumVertices[b] += addMe;
		sumVertices[c] += addMe;

		counter[a] += 1;
		counter[b] += 1;
		counter[c] += 1;
	}

	for (unsigned i = 0; i < (*vertices).size(); ++i) {
		sumVertices[i] /= counter[i];
		sumVertices[i].normalize();
	}

}

///////////////////////////////////////////////////////////////
////////////////////   GET CENTER OF MASS  ////////////////////
///////////////////////////////////////////////////////////////
void getCenterOfMass() {

	double totalArea = 0;
	for (unsigned i = 0; i < (*faces).size(); ++i) {
		center1X += (*faces)[i].area
				* ((*vertices)[(*faces)[i].v1].x + (*vertices)[(*faces)[i].v2].x
						+ (*vertices)[(*faces)[i].v3].x) / 3;
		center1Y += (*faces)[i].area
				* ((*vertices)[(*faces)[i].v1].y + (*vertices)[(*faces)[i].v2].y
						+ (*vertices)[(*faces)[i].v3].y) / 3;
		center1Z += (*faces)[i].area
				* ((*vertices)[(*faces)[i].v1].z + (*vertices)[(*faces)[i].v2].z
						+ (*vertices)[(*faces)[i].v3].z) / 3;
		totalArea += (*faces)[i].area;
	}
	center1X /= totalArea;
	center1Y /= totalArea;
	center1Z /= totalArea;

	cout << "averages: " << center1X << "    " << center1Y << "  " << center1Z
			<< endl;
	cout << "totalarea: " << totalArea << endl;

}

///////////////////////////////////////////////////////////////
///////////////////////////   main  ///////////////////////////
///////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

	//////////////////////////////////////  initializing opengl
	GLInit(&argc, argv);
	glutMouseFunc(OnMouseClick);
	glutKeyboardFunc(keyInput);

	//pushing on the state  the user can push
	stringOfStates.push_back("Rotate");
	stringOfStates.push_back("Translate");
	stringOfStates.push_back("Image");
	stringOfStates.push_back("Level");
	stringOfStates.push_back("Not Determined");

	if (argc != 2) {
		cout << "Error: File not specified" << endl;
		return 0;
	}
	string line = argv[1];
	fileName = argv[1];
	cout << "line: " << line.c_str() << endl;
	ifstream myfile(line.c_str());
	if (myfile.is_open()) {

		getline(myfile, line);
		cout << line << endl;

		stringstream ss(line);
		int numVertices = 0;
		int numFaces;
		ss >> numVertices;
		ss >> numFaces;

		cout << "Here is numVertices: " << numVertices << endl;
		cout << "Here is numFaces: " << numFaces << endl;
		cout << "Printing vertices" << endl;

		//getting vertices and printing them
		for (int i = 0; i < numVertices; ++i) {
			int x, y, z;
			getline(myfile, line);
			stringstream ssVertices(line);

			// reading from string stream into vars
			ssVertices >> x;
			ssVertices >> y;
			ssVertices >> z;
			//pushing onto vector
			vertice addMe(8 * x, 8 * y, 8 * z);
			vertices1.push_back(addMe);
		}

		//getting vertices and printing them
		cout << "Printing faces" << endl;
		for (int i = 0; i < numFaces; ++i) {
			int x, y, z;
			getline(myfile, line);
			stringstream ssVertices(line);
			// reading from string stream into vars
			ssVertices >> x;
			ssVertices >> y;
			ssVertices >> z;
			//pushing onto vector
			face addMe(x, y, z);
			faces1.push_back(addMe);
		}

		myfile.close();
	} else
		cout << "Unable to open file";

	std::cout << "faveeeeeee:" << faces->size() << std::endl;

	/////////////////////////////////////// populating other sublevels
	gettingOtherLevels();

	/////////////////////////////////////// calculating the normals
	getNormals();
	//////////////////////////////////// calculate center of mass
	getCenterOfMass();

	glutMainLoop();

	return 0;
}

///////////////////////////////////////////////////////////////
///////////////////   drawing TRIANGLES   /////////////////////
//////////////////////////////////////////////////////////////

//draws front view of triangle or xy cord
void drawTriXY() {

	cout << " Doing rectangles" << endl;

	for (unsigned i = 0; i < faces->size(); ++i) {

		int v1 = (*faces)[i].v1;
		int v2 = (*faces)[i].v2;
		int v3 = (*faces)[i].v3;

		ddaAlgorithm((*vertices)[v1].x, (*vertices)[v1].y, (*vertices)[v2].x,
				(*vertices)[v2].y);
		ddaAlgorithm((*vertices)[v2].x, (*vertices)[v2].y, (*vertices)[v3].x,
				(*vertices)[v3].y);
		ddaAlgorithm((*vertices)[v3].x, (*vertices)[v3].y, (*vertices)[v1].x,
				(*vertices)[v1].y);
	}

}

//draws front view of triangle or xy cord
void drawTriZY() {

	for (unsigned i = 0; i < faces->size(); ++i) {

		int v1 = (*faces)[i].v1;
		int v2 = (*faces)[i].v2;
		int v3 = (*faces)[i].v3;

		ddaAlgorithm((*vertices)[v1].z, (*vertices)[v1].y, (*vertices)[v2].z,
				(*vertices)[v2].y);
		ddaAlgorithm((*vertices)[v2].z, (*vertices)[v2].y, (*vertices)[v3].z,
				(*vertices)[v3].y);
		ddaAlgorithm((*vertices)[v3].z, (*vertices)[v3].y, (*vertices)[v1].z,
				(*vertices)[v1].y);
	}

}

//draws front view of triangle or xy cord
void drawTriXZ() {

	cout << " Doing rectangles" << endl;

	for (unsigned i = 0; i < faces->size(); ++i) {

		int v1 = (*faces)[i].v1;
		int v2 = (*faces)[i].v2;
		int v3 = (*faces)[i].v3;

		ddaAlgorithm((*vertices)[v1].x, (*vertices)[v1].z, (*vertices)[v2].x,
				(*vertices)[v2].z);
		ddaAlgorithm((*vertices)[v2].x, (*vertices)[v2].z, (*vertices)[v3].x,
				(*vertices)[v3].z);
		ddaAlgorithm((*vertices)[v3].x, (*vertices)[v3].z, (*vertices)[v1].x,
				(*vertices)[v1].z);
	}

}

///////////////////////////////////////////////////////////////
////////////////////////   mouse event   //////////////////////
///////////////////////////////////////////////////////////////
void OnMouseClick(int button, int state, int x, int y) {

}

