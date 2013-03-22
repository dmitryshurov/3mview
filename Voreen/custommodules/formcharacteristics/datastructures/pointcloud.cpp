#include "pointcloud.h"
 
using namespace voreen;

void PointCloud :: SetOrientation(tgt::mat4 arg)
{
	double z;
     z = arg.elem[0]*(arg.elem[5]*arg.elem[10] - arg.elem[6]*arg.elem[9]) + arg.elem[1]*(arg.elem[6]*arg.elem[8] - arg.elem[4]*arg.elem[10]) + arg.elem[2]*(arg.elem[4]*arg.elem[9] - arg.elem[5]*arg.elem[8]);
	if (z>0)	orientation = 1;
	else      orientation = -1;
}

PointCloud :: PointCloud()
{
}

PointCloud :: ~PointCloud()
{
	delete[] points;
	delete[] values;
	delete[] moments;
}

void PointCloud :: VolumeFill(const Volume* vol)
{
	const VolumeRAM* volRam = vol->getRepresentation<VolumeRAM>();
	RealWorldMapping rwm    = vol->getRealWorldMapping();
	SetOrientation(vol->getVoxelToWorldMatrix());

	tgt::svec3 dims   = vol->getDimensions();
	tgt::dvec4 pworld;
	double     valNorm;
	double     valRW;

	weight 	  = 0;

	entries_num =  dims.x*dims.y*dims.z;
	points      = new tgt::vec3[entries_num];
     values      = new double[entries_num];

	for (int i=0; i<dims.x; ++i)
		for (int j=0; j<dims.y; ++j)
			for (int k=0; k<dims.z; ++k)
			{
				pworld = vol->getVoxelToWorldMatrix() * (tgt::dvec4(0.5+i, 0.5+j, 0.5+k, 1.0));

				points[i+j*dims.x+k*dims.x*dims.y].x = pworld.x;
				points[i+j*dims.x+k*dims.x*dims.y].y = pworld.y;
				points[i+j*dims.x+k*dims.x*dims.y].z = pworld.z;

				valNorm = volRam->getVoxelNormalizedLinear(tgt::dvec3(0.5+i, 0.5+j, 0.5+k));
				valRW = rwm.normalizedToRealWorld(valNorm);

				values[i+j*dims.x+k*dims.x*dims.y] = valRW;
				weight += valRW;
			}
}

void PointCloud :: MoleculeFill(const Molecule * mol)
{
	entries_num = (mol->getOBMol()).NumAtoms();

	points      = new tgt::vec3[entries_num];
     values      = new double[entries_num];

	weight 	  = 0;

	for (int i=0; i<entries_num; ++i)
	{	
		points[i].x   = (mol->getOBMol()).GetAtomById(i)->x();
		points[i].y   = (mol->getOBMol()).GetAtomById(i)->y();
		points[i].z   = (mol->getOBMol()).GetAtomById(i)->z();
		values[i]     = 1;

		weight += values[i];
	}
}


double PointCloud :: CalculateFourrier(int degX, int degY, int degZ)
{
	double res = 0;
	double temp;
	for (int i=0; i<entries_num; ++i)
	{
		temp = values[i];
		if (degX<0) temp *= cos(degX*points[i].x/scale/max_size*PI_2);
		if (degX>0) temp *= sin(degX*points[i].x/scale/max_size*PI_2);
		if (degY<0) temp *= cos(degY*points[i].y/scale/max_size*PI_2);
		if (degY>0) temp *= sin(degY*points[i].y/scale/max_size*PI_2);
		if (degZ<0) temp *= cos(degZ*points[i].z/scale/max_size*PI_2);
		if (degZ>0) temp *= sin(degZ*points[i].z/scale/max_size*PI_2);
		res += temp;
	}
	return res;
}

double PointCloud :: CalculateMoment(int degX, int degY, int degZ)
{
	double res = 0;
	double temp;
	for (int i=0; i<entries_num; ++i)
	{
		temp = values[i];
		for (int j = 0; j < degX; ++j)
		{
			temp *= points[i].x/scale;
		}
		
		for (int j = 0; j < degY; ++j)
		{
			temp *= points[i].y/scale;
		}
		
		for (int j = 0; j < degZ; ++j)
		{
			temp *= points[i].z/scale;
		}
		res += temp/weight;
	}
	return res;
}

double PointCloud :: PolynomVal(double x)
{
	return polynom[3]*x*x*x + polynom[2]*x*x + polynom[1]*x + polynom[0];
}

tgt::Matrix4d PointCloud :: GetShift()
{
     O[0] = 0;
	O[1] = 0;
	O[2] = 0;
     weight = 0;

	for (int i=0; i<entries_num; ++i)
	{	
		O[0] += points[i].x * values[i];
		O[1] += points[i].y * values[i];
		O[2] += points[i].z * values[i];
		weight += values[i];
    }
  
	O[0] /= weight;
	O[1] /= weight;
	O[2] /= weight;
	 
	tgt::Matrix4d out_data(1,     0,     0, -O[0],
					   0,     1,     0, -O[1],
			 		   0,     0,     1, -O[2],
					   0,     0,     0,    1);
	return out_data;
}

void PointCloud :: Centrify()
{
	for (int i=0; i<entries_num; ++i)
	{	
		points[i].x -= O[0];
		points[i].y -= O[1];
		points[i].z -= O[2];
	}
}

void PointCloud :: unCentrify()
{
	for (int i=0; i<entries_num; ++i)
	{	
		points[i].x += O[0];
		points[i].y += O[1];
		points[i].z += O[2];
	}
}

tgt::Matrix4d PointCloud :: GetAxes()
{
     Centrify();
	double disc;
	double I[3][3];
	double eigens[3];

	double V1[3];
	double V2[3];
	double a = -1;
	double b = 1;

	I[0][0] = CalculateMoment(2, 0, 0);
	I[1][1] = CalculateMoment(0, 2, 0);
	I[2][2] = CalculateMoment(0, 0, 2);
	I[0][1] = CalculateMoment(1, 1, 0);
	I[0][2] = CalculateMoment(1, 0, 1);
	I[1][2] = CalculateMoment(0, 1, 1);
	I[1][0] = I[0][1];
	I[2][0] = I[0][2];
	I[2][1] = I[1][2];


	polynom[0] =  I[0][0]*I[1][1]*I[2][2]+2*I[0][1]*I[0][2]*I[1][2]-I[0][2]*I[0][2]*I[1][1]-I[2][1]*I[2][1]*I[0][0]-I[0][1]*I[0][1]*I[2][2];
	polynom[1] = -(I[1][1]*I[2][2]+I[2][2]*I[0][0]+I[0][0]*I[1][1]-I[0][2]*I[0][2]-I[1][2]*I[1][2]-I[0][1]*I[0][1]);
	polynom[2] = I[0][0] + I[1][1] + I[2][2];
	polynom[3] = -1;

	while (PolynomVal(a)<0) {a-=1;}
	while (PolynomVal(b)>0) {b+=1;}
	for (int i=0; i<50; ++i)
	{
		if (PolynomVal((b+a)/2) > 0)
		{
			a = (b+a)/2;
		}
		else 
		{
			b = (b+a)/2;
		}
	}
	eigens[0] = a;
	
	polynom[0] = polynom[1]+a*polynom[2]+a*a*polynom[3];
	polynom[1] = polynom[2]+a*polynom[3];
	polynom[2] = polynom[3];
	polynom[3] = 0;
	

	disc = polynom[1]*polynom[1] - 4*polynom[0]*polynom[2];
	if (disc < 0) 
	{
		return tgt::Matrix4d::identity;
	}
	else
	{
		eigens[1] = -(polynom[1]-sqrt(disc))/(2*polynom[2]);
		eigens[2] = -(polynom[1]+sqrt(disc))/(2*polynom[2]);
	}
	
	for (int i=0; i<3; ++i)
	{
		for (int j=i; j<3; ++j)	
		{
			if (fabs(eigens[i]) > fabs(eigens[j]))
			{
				disc = eigens[i];
				eigens[i] = eigens[j];
				eigens[j] = disc;
			}
		}
	}	

	V1[0] = I[0][0] - eigens[0]; 
	V1[1] = I[0][1];
	V1[2] = I[0][2];

	V2[0] = I[1][0]; 
	V2[1] = I[1][1] - eigens[0];
	V2[2] = I[1][2];

	Ox[0] = V1[1]*V2[2] - V1[2]*V2[1];	
	Ox[1] = V1[2]*V2[0] - V1[0]*V2[2];	
	Ox[2] = V1[0]*V2[1] - V1[1]*V2[0];
	
	double len = sqrt(Ox[0]*Ox[0]+Ox[1]*Ox[1]+Ox[2]*Ox[2]);
	Ox[0] /= len;
	Ox[1] /= len;
	Ox[2] /= len;


	V1[0] = I[1][0];
	V1[1] = I[1][1] - eigens[1]; 
	V1[2] = I[1][2];

	V2[0] = I[2][0]; 
	V2[1] = I[2][1];
	V2[2] = I[2][2] - eigens[1];

	Oy[0] = V1[1]*V2[2] - V1[2]*V2[1];	
	Oy[1] = V1[2]*V2[0] - V1[0]*V2[2];	
	Oy[2] = V1[0]*V2[1] - V1[1]*V2[0];

	len = sqrt(Oy[0]*Oy[0]+Oy[1]*Oy[1]+Oy[2]*Oy[2]);
	Oy[0] /= len;
	Oy[1] /= len;
	Oy[2] /= len;

	V1[0] = I[0][0] - eigens[2]; 
	V1[1] = I[0][1];
	V1[2] = I[0][2];

	V2[0] = I[2][0]; 
	V2[1] = I[2][1];
	V2[2] = I[2][2] - eigens[2];

	Oz[0] = V1[1]*V2[2] - V1[2]*V2[1];	
	Oz[1] = V1[2]*V2[0] - V1[0]*V2[2];	
	Oz[2] = V1[0]*V2[1] - V1[1]*V2[0];

	len = sqrt(Oz[0]*Oz[0]+Oz[1]*Oz[1]+Oz[2]*Oz[2]);
	Oz[0] /= len;
	Oz[1] /= len;
	Oz[2] /= len;
	double mx = CalculateMoment(3, 0, 0);
	double my = CalculateMoment(0, 3, 0);

	disc =  Ox[0]*Oy[1]*Oz[2]+Ox[1]*Oy[2]*Oz[0]+Oy[0]*Oz[1]*Ox[2]-Ox[2]*Oz[0]*Oy[1]-Oz[1]*Oy[2]*Ox[0]-Ox[1]*Oy[0]*Oz[2];
	
	if (disc<0)
	{
		Oz[0] = -Oz[0];
		Oz[1] = -Oz[1];
		Oz[2] = -Oz[2];
	}
	
	if (mx<0)
	{
		Ox[0] = -Ox[0];
		Ox[1] = -Ox[1];
		Ox[2] = -Ox[2];

		Oy[0] = -Oy[0];
		Oy[1] = -Oy[1];
		Oy[2] = -Oy[2];
	}			
	if (my<0)
	{
		Oz[0] = -Oz[0];
		Oz[1] = -Oz[1];
		Oz[2] = -Oz[2];

		Oy[0] = -Oy[0];
		Oy[1] = -Oy[1];
		Oy[2] = -Oy[2];
	}

     unCentrify();

     tgt::Matrix4d out_data(Ox[0], Ox[1], Ox[2], 0,
					   Oy[0], Oy[1], Oy[2], 0,
 					   Oz[0], Oz[1], Oz[2], 0,
			     	   0,     0,     0,     1);
	return out_data;
}

void PointCloud :: GetMoments8()
{
	Centrify();
	mom_order = 8;
	moments = new double[240];

	int a, b, c;
	int l = 0;

	for (int i = -mom_order*mom_order*mom_order; 
              i <  mom_order*mom_order*mom_order; ++i) 
	{
		a = i / (mom_order * mom_order);
		b = (i - a*mom_order*mom_order) / mom_order;
		c = i % mom_order;
		if (sqrt(a*a) + sqrt(b*b) + sqrt(c*c) < mom_order)
		{
			moments[l] = CalculateFourrier(a, b, c);
			l++;
		}
	}	
	unCentrify();
}
