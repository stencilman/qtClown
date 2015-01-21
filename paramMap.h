#ifndef __PARAM_MAP__
#define __PARAM_MAP__

#include <map>
#include <xstring>
#include "CMatrix.h"
#include <nr3.h>
#include <svd.h>
#include <vector>

class paramMap
{
public:
	enum semanticParams {Height,Weight,BreastSize,WaistSize,HipSize,LegLength};

private:
	std::map<semanticParams, bool> stateSemanticParams;	int mActiveParams;
	std::map<semanticParams, std::string> nameSemanticParams;
	std::map<semanticParams, std::pair<int, int> > mSemanticParamRange;
	CMatrix<double> P_mlab, semdata; //note, F is a subset of semdata with addition '1' col
	CMatrix <double> mMap, mMap_T;

private:
	void loadSemdata();
	void loadP_mlab();
	

public:
	paramMap();
	void computeMap(); //using param map
	std::string& getNameforParam( unsigned int i0 );
	void setSemanticParams(const std::vector<bool> &states);
	void mapSemanticToEigenSpace( const std::map<paramMap::semanticParams, float> &mSemanticParamValues, CVector<double> &shapeParams );
	int getRange(paramMap::semanticParams param, bool lowEnd);
};

#endif //__PARAM_MAP__