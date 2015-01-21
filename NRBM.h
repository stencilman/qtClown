// CRBM
// A collection of functions for RBM representations
// TWIST, MATRIX, AXIS-ANGLE 
//
// Author: Juergen Gall 

#ifndef NRBMH
#define NRBMH

#include "CVector.h"
#include "CMatrix.h"
#include <vector>

namespace NRBM {
  // Transforms a rigid body motion in matrix representation to a twist representation
  void RBM2Twist(CVector<float> &T, CMatrix<float>& RBM);
  void Twist2RBM(CVector<float>& aTwist, CMatrix<float>& A);
  void AdjointTwist(CVector<float> &T, CMatrix<float>& RBM);
  double Pitch(CVector<float> &TV);
  
  // Converts rigid body motion into rotation vector and translation vector
  void RBM2RVT(const CMatrix<float>* RBM, CVector<double>& rvt);
  void RVT2RBM(const CVector<double>* rvt, CMatrix<float>& RBM);

  // Converts rotation matrix into rotation vector representation
  void RM2RV(const CMatrix<float>* R, CVector<double>& rv);
  void RV2RM(const CVector<double>* rv, CMatrix<float>& R);

  // mean of weighted rotations
  void meanRotation(const std::vector<CMatrix<float> >* vR, const std::vector<double>* vW, CMatrix<float>& meanR);
  void invRBM(CMatrix<float>& RBM);
  void invRM(CMatrix<float>& R);
  void QT2DQ(const CVector<float>& q0, const CVector<float>& t, CVector<float>& dq);   
  void QDQ2RBM(const CVector<float>& q0, const CVector<float>& dq, CMatrix<float>& RBM);
  void RBM2QDQ(const CMatrix<float>& RBM, CVector<float>& q0, CVector<float>& dq);
  void RM2Quat(const CMatrix<float>& R, CVector<float>& q);
  void Quat2RM(const CVector<float>& q, CMatrix<float>& R);

  void toOpenGL(const CMatrix<float>& RBM, float* mat);
}

#endif
