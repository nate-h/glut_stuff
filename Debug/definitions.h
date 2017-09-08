
#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__
///////////////////////////////////////////////////////////////
////////////////   defining faces and vertices   //////////////
///////////////////////////////////////////////////////////////

struct vertice{
	int x;
	int y;
	int z;
	vertice(int xIn, int yIn, int zIn){
		x=xIn; y=yIn; z=zIn;
	}
	vertice(const vertice & inV){
		x=inV.x; y=inV.y; z=inV.z;
	}
	vertice& operator *(const vertice& inV)
	{
    	x= x*inV.x;
    	y= y*inV.y;
    	z = z*inV.z;
    	return *this;
	}
	vertice& operator *=(double multiplier)
	{
    	x*=multiplier;
    	y*=multiplier;
    	z*=multiplier;
    	return *this;
	}
	vertice& operator +=(const vertice& inV)
	{
    	x += inV.x;
    	y += inV.y;
    	z += inV.z;
    	return *this;
	}
};

struct face{
	int v1;
	int v2;
	int v3;
	int nx;
	int ny;
	int nz;
	double area;
	
	face(int xIn, int yIn, int zIn){
		v1=xIn; v2=yIn; v3=zIn;
		nx=0; ny=0; nz=0;
		area=0;
	}
	face(const face & inF){
		v1=inF.v1; v2=inF.v2; v3=inF.v3;
		nx=0; ny=0; nz=0;
		area=0;
	}
};

#endif
