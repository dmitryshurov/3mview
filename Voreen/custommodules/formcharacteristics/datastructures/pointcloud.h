#ifndef VRN_POINTCLOUD_H
#define VRN_POINTCLOUD_H

#define PI_2 1.57079632679

#include "voreen/core/datastructures/volume/volume.h"
#include "tgt/matrix.h"
#include "tgt/vector.h"
#include "../../molecule/datastructures/molecule.h"
namespace voreen {

class PointCloud {
public:
	int       entries_num;

	tgt::vec3 *points;
	double    *values;
	double    scale;
	double    max_size;

	double    *moments;

	tgt::Matrix4d GetShift();
	tgt::Matrix4d GetAxes();
	
	void GetMoments8();

	double CalculateMoment(int, int, int);   	
	double CalculateFourrier(int, int, int); 

	void VolumeFill(const Volume* vol);

 	void MoleculeFill(const Molecule* mol);

	void SetOrientation(tgt::mat4 arg);

	PointCloud();
	~PointCloud();
private:

	float O[3]; 
	float Ox[3];
	float Oy[3];
	float Oz[3];

	double polynom[4];
	double weight;
	int    orientation;
	int    mom_order;
 	
	double PolynomVal(double);
	
	void Centrify();
	void unCentrify();
	void Reorientate();
};

}
#endif 