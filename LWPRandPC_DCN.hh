#include <lwpr.hh>
// #include <math.h>

class LWPR_andPC_DCN{
  public:
    std::vector<double> wt;
    std::vector<double> w;
    std::vector<double> w_pc_dcn;
    std::vector<double> w_mf_dcn;

    double ML_prediction(const doubleVec& input_lwpr, double efferent_copy);
    int ML_rfs();
    void ML_update(const doubleVec& input_lwpr, const doubleVec& train_lwpr);

    // Constructor
    LWPR_andPC_DCN(int n_inputs_i, int n_outputs_i, int n_actuators);
    // Deconstructor
    ~LWPR_andPC_DCN();

  private:
    int n_in;
    int n_out;
    int n_act;
    std::vector<LWPR_Object> models;
};

// Constructor
LWPR_andPC_DCN::LWPR_andPC_DCN(int n_inputs_i, int n_outputs_i, int n_actuators){
  std::cout << "LWPR_andPC_DCN object -> Created" << std::endl;

  // Initial PC-DCN weights
  wt.push_back(0);
  w.push_back(0);
  w_pc_dcn.push_back(0);
  w_mf_dcn.push_back(0);

  n_in  = n_inputs_i;
  n_out = n_outputs_i;
  n_act = n_actuators;

  LWPR_Object lwpr_model = LWPR_Object(n_in, n_out);
  for(int i=0; i < n_act; i++){
    // Set initial distance metric to (50 * I)
		lwpr_model.setInitD(50);
		lwpr_model.setInitAlpha(250);
		lwpr_model.initLambda(0.995f);
		lwpr_model.tauLambda(0.5f);
		lwpr_model.finalLambda(0.9995f);
		lwpr_model.wPrune(0.9f);
		lwpr_model.wGen(0.2);
		lwpr_model.diagOnly(true);
		lwpr_model.updateD(true);
		lwpr_model.useMeta(false);
		lwpr_model.metaRate(0.3);
		// lwpr_model.add_threshold(0.95);
		lwpr_model.kernel("Gaussian");

    // Store lwpr_model in our vector of models
    models.push_back(lwpr_model);
  }
}

// Deconstructor
LWPR_andPC_DCN::~LWPR_andPC_DCN(){
  models.clear();
  std::cout << "LWPR_andPC_DCN object-> Destroyed" << std::endl;
}

// // Member functions
double LWPR_andPC_DCN::ML_prediction(const doubleVec& input_lwpr, double fb_signal){
  doubleVec a, b;
  // LWPR prediction
  for(int i=0; i < n_act; i++){
    double x;
    a, w = models[i].predict_with_weights(input_lwpr, wt, i, x);

    // weights - PC dendrites

    // DCN - adder/substractor

    // DCN learning rules

  }

  return 0; //output_ml, output_dcn
}

void LWPR_andPC_DCN::ML_update(const doubleVec& input_lwpr, const doubleVec&efferent_copy){
  for(int i=0; i < n_act; i++){
    models[i].update(input_lwpr, efferent_copy);
  }
}

// int LWPR_andPC_DCN::ML_rfs(){
//   int num_rfs = numRFS();
//   cout << "num_rfs: " << num_rfs << '\n';
//   return num_rfs;
// }

// #endif /* SRC_NRP_MLANDC_H_ */
