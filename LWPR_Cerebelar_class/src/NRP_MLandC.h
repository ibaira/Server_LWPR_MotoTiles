/*
 * NRP_MLandC.h
 *
 *  Created on: Nov 14, 2015
 *      Author: Silvia Tolu
 */

#ifndef SRC_NRP_MLANDC_H_
#define SRC_NRP_MLANDC_H_

#include <lwpr.hh>

typedef std::vector<LWPR_Object> VEC_LWPR_Object;
typedef std::vector<doubleVec> VEC_doubleVec;

class NRP_MLandC {
public:
	NRP_MLandC(int number_of_joints, int number_of_inputs, double beta);
	~NRP_MLandC();

	/* \brief ML_predict(X, FBACK): VEC_doubleVec
	 * This function uses parameter X to estimate a value Y (Y = F(X)). Likewise, it uses
	 * parameter FBACK to update the internal cerebelar circuit. Finally, it returns a
	 * vector comprised two elements, the predicted value (first element of the vector)
	 * and the cerebelar contribution (second element of the vector). Both the predicted
	 * value and cerebelar contribution are also vectors with size equal to the number
	 * of joints in the system.
	 */
	VEC_doubleVec ML_predict(doubleVec& x, doubleVec fback);

	/* \brief ML_update(X,Y)
	 * This function reorganises the internal neural network according the training pair
	 * (X,Y) - Y = F(X)
	 */
	void ML_update(doubleVec& x, doubleVec& y);

private:
	VEC_LWPR_Object models;
	VEC_doubleVec cWeights, pWeights;
	int njoints, nin, numRFS;
	double beta;
};

#endif /* SRC_NRP_MLANDC_H_ */
