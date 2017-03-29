#include <lwpr.hh>
// #include <math.h>

/* Class abstracting the Unit Learning Machine comprised of the LWPR and a
 * cerebellar-like microcircuit. More concretely, the Purkinje cells- and Deep
 * Cerebellar Nuclei- layers (PC and DCN, respectively).
 */
class LWPR_andPC_DCN{
  public:
    // LWPR's weights
    std::vector<double> pk_t;
    std::vector<double> pk;
    // PC dendrites' weights
    std::vector<double> w;
    std::vector<double> w_t;
    // DCN dendrites' weights
    std::vector<double> w_pc_dcn;
    std::vector<double> w_mf_dcn;

    std::vector<doubleVec> LWPR_output;
    std::vector<doubleVec> PC_output;
    std::vector<doubleVec> DCN_output;
    std::vector<doubleVec> tot_output;


    std::vector<doubleVec> ML_prediction(const doubleVec& input_lwpr, doubleVec efferent_copy);
    int ML_rfs();
    void ML_update(const doubleVec& input_lwpr, const doubleVec& train_lwpr);

    // Constructor
    LWPR_andPC_DCN(int n_inputs_i, int n_outputs_i, int n_actuators);
    ~LWPR_andPC_DCN();

  private:
    int n_in;
    int n_out;
    int n_act;
    int n_RFs;
    double betha;
    std::vector<LWPR_Object> models;
};

// ---------------------    Constructor
LWPR_andPC_DCN::LWPR_andPC_DCN(int n_inputs_i, int n_outputs_i, int n_actuators){
  std::cout << "LWPR_andPC_DCN object -> Created" << std::endl;

  // Initial PC-DCN weights
  pk.push_back(0);
  pk_t.push_back(0);
  w.push_back(0);
  w_t.push_back(0);
  w_pc_dcn.push_back(0);
  w_mf_dcn.push_back(0);

  n_in  = n_inputs_i;
  n_out = n_outputs_i;
  n_act = n_actuators;
  n_RFs = 0;
  betha = 0.05;

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

// ---------------------    Deconstructor
LWPR_andPC_DCN::~LWPR_andPC_DCN(){
  models.clear();
  std::cout << "LWPR_andPC_DCN object-> Destroyed" << std::endl;
}

// ---------------------    Member functions
std::vector<doubleVec> LWPR_andPC_DCN::ML_prediction(const doubleVec& input_lwpr, doubleVec fb_signal){
  std::vector<doubleVec> lwpr_prediction;
  // Prediction for each object
  for(std::vector<LWPR_Object>::iterator iter = models.begin(); iter != models.end(); iter++){
    // LWPR contribution
    lwpr_prediction.push_back(iter->predict_with_weights(input_lwpr, pk_t, n_RFs));

    // Weights - PC dendritess
    // PC_output = 0.0;
    // adjust lengths
    int i = 0;
    double PC_output_i = 0.0;
    std::vector<double>::iterator i_p = pk_t.begin();
    for(std::vector<double>::iterator i_w = w.begin(); i_w != w.end(); ++i_w){
      *i_w = (*i_w) + betha * fb_signal[i] * (*i_p);
      PC_output_i += (*i_p) * (*i_w);
      ++i;
    }
    PC_output.push_back(doubleVec(PC_output_i));
    lwpr_prediction.push_back(w);

    // DCN - adder/substractor
    // DCN learning rules

  }

  return lwpr_prediction; //output_ml, output_dcn
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
