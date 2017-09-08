#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

using namespace std;
struct OppPts {
	int x;
	int y;
	int newPoint;
	OppPts(int xIn) {
		x = xIn;
		y = -1;
		newPoint = 0;
	}
};

// A simple wrapper for store 3D vectors
struct Vector3 {
	float x;
	float y;
	float z;

	Vector3() :
			x(0.0), y(0.0), z(0.0) {
	}

	Vector3(float x, float y, float z) :
			x(x), y(y), z(z) {
	}

	Vector3(bool value) {
		z = 0;
		y = (rand() % 8 - 4) / 3;
		x = (rand() % 8 - 4) / 3;

	}

	Vector3(const Vector3 & v) :
			x(v.x), y(v.y), z(v.z) {
	}

	Vector3 operator+(const Vector3 & rhs) const {
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	Vector3 operator-(const Vector3 & rhs) const {
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	Vector3 operator*(float rhs) const {
		return Vector3(x * rhs, y * rhs, z * rhs);
	}
	Vector3 operator/(float rhs) const {
		return Vector3(x / rhs, y / rhs, z / rhs);
	}
	Vector3 operator+=(const Vector3 & rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	Vector3 operator-=(const Vector3 & rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	Vector3 operator*=(float rhs) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	Vector3 operator/=(float rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	float magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}
	void normalize() {
		*this /= magnitude();
	}
	Vector3 normalized() const {
		return *this / magnitude();
	}
	float dot(const Vector3 & rhs) const {
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	Vector3 cross(const Vector3 & rhs) const {
		return Vector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z,
				x * rhs.y - y * rhs.x);
	}
};

///////////////////////////////////////////////////////////////
////////////////   defining faces and vertices   //////////////
///////////////////////////////////////////////////////////////
struct vertice {
	int x;
	int y;
	int z;
	vertice(int xIn, int yIn, int zIn) {
		x = xIn;
		y = yIn;
		z = zIn;
	}
	vertice(const vertice & inV) {
		x = inV.x;
		y = inV.y;
		z = inV.z;
	}
	vertice& operator *(const vertice& inV) {
		x = x * inV.x;
		y = y * inV.y;
		z = z * inV.z;
		return *this;
	}
	vertice& operator *=(double multiplier) {
		x *= multiplier;
		y *= multiplier;
		z *= multiplier;
		return *this;
	}
	vertice& operator +=(const vertice& inV) {
		x += inV.x;
		y += inV.y;
		z += inV.z;
		return *this;
	}
};

struct face {
	int v1;
	int v2;
	int v3;
	float nx;
	float ny;
	float nz;
	double area;
	int magnitude;

	face(int xIn, int yIn, int zIn) {
		v1 = xIn;
		v2 = yIn;
		v3 = zIn;
		nx = 0;
		ny = 0;
		nz = 0;
		area = 0;
		magnitude = 0;
	}
	face(const face & inF) {
		v1 = inF.v1;
		v2 = inF.v2;
		v3 = inF.v3;
		nx = 0;
		ny = 0;
		nz = 0;
		area = 0;
		magnitude = 0;
	}
};

struct FaceAndDist {
	int face;
	int distance;
	FaceAndDist() {
		face = -1;
		distance = 100;
	}

};

void setMatrix(int array[][3], int array1[][1], vertice& a, vertice& b,
		vertice& c, int i, int j);
int determinent(int array[][3]);
int calculate(int array[][3], int a, int b, int c);
int copy(int array[][3], int array1[][1], int a);
void comp_copy(int array[][3], int array1[][3]);

bool cramersRule(vertice& a, vertice& b, vertice& c, int i, int j) {

	int matrix[3][3];
	int matrix1[3][1];
	int reserve[3][3];
	int det00, detr[3], sp1 = 0, cont = 0;
	setMatrix(matrix, matrix1, a, b, c, i, j);
	comp_copy(reserve, matrix);
	det00 = determinent(matrix);
	while (sp1 < 3) {
		detr[cont] = copy(matrix, matrix1, sp1);
		comp_copy(matrix, reserve);
		cont++;
		sp1++;
	}

	float r = 1.0 * detr[1] / det00;
	float s = 1.0 * detr[2] / det00;

	if (r >= 0.000001 && s >= 0.000001 && r + s <= 1) {

		return true;
	} else{
		return false;}

}
void setMatrix(int array[][3], int array1[][1], vertice& a, vertice& b,
		vertice& c, int i, int j) {

	int lightX = 100;
	int lightY = 100;
	int lightZ = -500;

	array[0][0] = lightX - i;
	array[0][1] = b.x - a.x;
	array[0][2] = c.x - a.x;
	array1[0][0] = lightX - a.x;

	array[1][0] = lightY - j;
	array[1][1] = b.y - a.y;
	array[1][2] = c.y - a.y;
	array1[1][0] = lightY - a.y;

	array[2][0] = lightZ - 800;
	array[2][1] = b.z - a.z;
	array[2][2] = c.z - a.z;
	array1[2][0] = lightZ - a.z;

}
int determinent(int array[][3]) {
	int rows = 1, col = 1;
	int z = 0;
	int temp = 0;
	int cont = 1;
	int x = 0;
	while (x < 3) {
		temp = temp + cont * (array[0][x] * calculate(array, rows, col, z));
		col = col * 0;
		z = z + cont;
		cont = cont * -1;
		x++;
	}
	return temp;
}
int calculate(int array[][3], int a, int b, int c) {
	int temp1;
	temp1 = (array[a][b] * array[a + 1][b + 1 + c])
			- (array[a + 1][b] * array[a][b + 1 + c]);
	return temp1;
}
int copy(int array[][3], int array1[][1], int a) {
	int col = 0;
	int temp;
	while (col < 3) {
		array[col][a] = array1[col][0];
		col++;
	}
	int i = 0, j = 0;
	while (i < 3) {
		j = 0;
		while (j < 3) {
			j++;
		}
		i++;
	}
	temp = determinent(array);
	return temp;
}
void comp_copy(int array[][3], int array1[][3]) {
	int rows = 0, col = 0;
	while (rows < 3) {
		col = 0;
		while (col < 3) {
			array[rows][col] = array1[rows][col];
			col++;
		}
		rows++;
	}
}

#endif
