#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "NRP_MLandC.h"

#define SEED_RAND()     srand48(time(NULL))
#define URAND()         drand48()

double cross(double x1,double x2) {
   double a = exp(-10*x1*x1);
   double b = exp(-50*x2*x2);
   double c = 1.25*exp(-5*(x1*x1 + x2*x2));

   if (a>b) {
      return (a>c) ? a:c;
   } else {
      return (b>c) ? b:c;
   }
}

int main(void)
{
	int ret = 0;
	doubleVec x(2);
	doubleVec y(1);

	double mse;
	unsigned int i,j,numTest;

	FILE *fp1, *fp2;
	NRP_MLandC model(2,2,0.25f);

	/* See above definition, we either use srand() on Windows or srand48 everywhere else */
	SEED_RAND();

	doubleVec u(3);

	for (j=0;j<20;j++) {
	  mse = 0.0;

	  for (i=0;i<1000;i++) {

		 x[0] = 2.0*URAND()-1.0;
		 x[1] = 2.0*URAND()-1.0;
		 y[0] = cross(x[0],x[1]) + 0.1*URAND()-0.05;

		 // Update the model with one sample
		 model.ML_update(x,y);
	  }
	}

	fp1 = fopen("output_predict.txt","w");
	fp2 = fopen("output_cerebelar.txt","w");

	mse = 0.0;
	numTest=0;

	for (x[1]=-1.0; x[1]<=1.01; x[1]+=0.05) {
	  for (x[0]=-1.0; x[0]<=1.01; x[0]+=0.05) {
		 y[0] = cross(x[0],x[1]);

		 // Use the model for predicting an output
		 VEC_doubleVec ret = model.ML_predict(x,y);
		 doubleVec yp(ret[0]);
		 doubleVec c(ret[1]);

		 mse += (y[0]-yp[0])*(y[0]-yp[0]);
		 numTest++;

		 fprintf(fp1,"%8.5f %8.5f %8.5f\n",x[0],x[1],yp[0]);
		 fprintf(fp2,"%8.5f %8.5f %8.5f\n",x[0],x[1],c[0]);
	  }
	  fprintf(fp1,"\n\n");
	  fprintf(fp2,"\n\n");
	}
	fclose(fp1);
	fclose(fp2);

	printf("MSE on test data (%d) = %f\n",numTest,mse/(double) numTest);

	printf("\nTo view the output, start gnuplot, and type:\n");
	printf("   splot \"output.txt\"\n\n");

	return ret;
}
