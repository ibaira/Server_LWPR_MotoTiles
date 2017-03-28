/*
 * NRP_MLandC.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: Silvia Tolu
 */

#include "NRP_MLandC.h"

NRP_MLandC::NRP_MLandC(int number_of_joints, int number_of_inputs, double beta_th) {
	// TODO Auto-generated constructor stub

	njoints = number_of_joints;
	nin = number_of_inputs;
	beta = beta_th;
	numRFS = 0;
	/* Weights initialization */
	doubleVec fCo(1,0.0f);
	cWeights.push_back(fCo);
	cWeights.push_back(fCo);

	for(int i=0; i<number_of_joints; i++)
	{
		/* Model generation - used to create others as a copy of this one */
		LWPR_Object model(nin, 1);

		/* Set initial distance metric to 50*(identity matrix) */
		model.setInitD(50);
		/* Set init_alpha to 250 in all elements */
		model.setInitAlpha(250);
		/* Set init_lambda to 0.995 in all elements */
		model.initLambda(0.995f);
		/* Set tau_lambda to 0.5 in all elements */
		model.tauLambda(0.5f);
		/* Set final_lambda to 0.9995 in all elements */
		model.finalLambda(0.9995f);
		/* Set w_prune to 0.9 */
		model.wPrune(0.9f);
		/* Set w_gen to 0.2 */
		model.wGen(0.2);
		/* Set diag_only to 1 */
		model.diagOnly(true);
		/* Set update_D to 1 */
		model.updateD(true);
		/* Set meta to 0 */
		model.useMeta(false);
		/* Set meta_rate to 0.3 */
		model.metaRate(0.3);
		/* Set add_threshold to 0.95 */
		model.add_threshold(0.95);
		/* Set module kernel to "Gaussian" */
		model.kernel("Gaussian");

		/* Store model in the vector */
		models.push_back(model);
	}
}

NRP_MLandC::~NRP_MLandC() {
	// TODO Auto-generated destructor stub

	models.clear();
}


/* \brief ML_predict(X, FBACK): VEC_doubleVec
 * This function uses parameter X to estimate a value Y (Y = F(X)). Likewise, it uses
 * parameter FBACK to update the internal cerebelar circuit. Finally, it returns a
 * vector comprised two elements, the predicted value (first element of the vector)
 * and the cerebelar contribution (second element of the vector). Both the predicted
 * value and cerebelar contribution are also vectors with size equal to the number
 * of joints in the system.
 */
VEC_doubleVec NRP_MLandC::ML_predict(doubleVec& x, doubleVec fback)
{
	VEC_doubleVec ret;
	doubleVec yp(njoints);
	doubleVec c(njoints);

	int i = 0;
	for (std::vector<LWPR_Object>::iterator it = models.begin() ; it != models.end(); ++it)
	{
		doubleVec wt(numRFS + 64);
		doubleVec temp = it->predict_with_weights(x, wt, numRFS);
		yp[i] = temp[0];

		if(wt.size()!=0)
		{
			if(wt.size() != cWeights[i].size())
				cWeights[i].resize(wt.size(), 0);

			std::vector<double>::iterator wt_it = wt.begin();
			for(std::vector<double>::iterator c_it = cWeights[i].begin(); c_it != cWeights[i].end(); ++c_it)
			{
				*c_it = (*c_it) + (beta * (*wt_it * fback[i]));
				c[i] += (*wt_it) * (*c_it);
				++wt_it;
			}
		}
		++i;
	}
	ret.push_back(yp);
	ret.push_back(c);

	return ret;
}

/* \brief ML_update(X,Y)
 * This function reorganises the internal neural network according the training pair
 * (X,Y) - Y = F(X)
 */
void NRP_MLandC::ML_update(doubleVec& x, doubleVec& y)
{
	for (std::vector<LWPR_Object>::iterator it = models.begin() ; it != models.end(); ++it)
		it->update(x,y);
}

//function [output_ml, output_C] = ML_prediction(obj, inputML, fbacktorq)
//	output_ml = zeros(1,obj.njoints);
//	output_C = zeros(1,obj.njoints);
//	for i = 1 : obj.njoints
//		[output_ml(i), wt] = lwpr_predict(obj.models(i).model,inputML(:,i));
//		if (~isempty(wt))
//			if (length(obj.cWeights(i).data) <= length(wt))
//				obj.cWeights(i).data = [obj.cWeights(i).data; zeros(1,(length(wt) - length(obj.cWeights(i).data)))'];
//				obj.pWeights(i).data = [obj.pWeights(i).data; zeros(1,(length(wt) - length(obj.pWeights(i).data)))'];
//				obj.cWeights(i).data = obj.pWeights(i).data + (obj.beta * (fbacktorq(i) * wt))';
//				obj.pWeights(i).data = obj.cWeights(i).data;
//			end
//			output_C(i) = obj.cWeights(i).data' * wt';
//		end
//	end
//end
//
//
//function ML_update(obj, inputML, train_MLoutput)
//	for i = 1 : obj.njoints
//		obj.models(i).model = lwpr_update(obj.models(i).model, inputML(:,i), train_MLoutput(i));
//	end
//end
