// Class: ReadMLP
// Automatically generated by MethodBase::MakeClass
//

/* configuration options =====================================================

#GEN -*-*-*-*-*-*-*-*-*-*-*- general info -*-*-*-*-*-*-*-*-*-*-*-

Method         : MLP::MLP
TMVA Release   : 4.2.1         [262657]
ROOT Release   : 6.08/06       [395270]
Creator        : rebecca
Date           : Sat Dec 23 19:09:24 2017
Host           : Darwin macitois18.cern.ch 16.4.0 Darwin Kernel Version 16.4.0: Thu Dec 22 22:53:21 PST 2016; root:xnu-3789.41.3~3/RELEASE_X86_64 x86_64
Dir            : /usr/local/root
Training events: 1751055
Analysis type  : [Classification]


#OPT -*-*-*-*-*-*-*-*-*-*-*-*- options -*-*-*-*-*-*-*-*-*-*-*-*-

# Set by User:
NCycles: "200" [Number of training cycles]
HiddenLayers: "N+3" [Specification of hidden layer architecture]
NeuronType: "radial" [Neuron activation function type]
V: "False" [Verbose output (short form of "VerbosityLevel" below - overrides the latter one)]
VarTransform: "N" [List of variable transformations performed before training, e.g., "D_Background,P_Signal,G,N_AllClasses" for: "Decorrelation, PCA-transformation, Gaussianisation, Normalisation, each for the given class of events ('AllClasses' denotes all events of all classes, if no class indication is given, 'All' is assumed)"]
H: "True" [Print method-specific help message]
TestRate: "6" [Test for overtraining performed at each #th epochs]
UseRegulator: "False" [Use regulator to avoid over-training]
# Default:
RandomSeed: "1" [Random seed for initial synapse weights (0 means unique seed for each run; default value '1')]
EstimatorType: "CE" [MSE (Mean Square Estimator) for Gaussian Likelihood or CE(Cross-Entropy) for Bernoulli Likelihood]
NeuronInputType: "sum" [Neuron input function type]
VerbosityLevel: "Default" [Verbosity level]
CreateMVAPdfs: "False" [Create PDFs for classifier outputs (signal and background)]
IgnoreNegWeightsInTraining: "False" [Events with negative weights are ignored in the training (but are included for testing and performance evaluation)]
TrainingMethod: "BP" [Train with Back-Propagation (BP), BFGS Algorithm (BFGS), or Genetic Algorithm (GA - slower and worse)]
LearningRate: "2.000000e-02" [ANN learning rate parameter]
DecayRate: "1.000000e-02" [Decay rate for learning parameter]
EpochMonitoring: "False" [Provide epoch-wise monitoring plots according to TestRate (caution: causes big ROOT output file!)]
Sampling: "1.000000e+00" [Only 'Sampling' (randomly selected) events are trained each epoch]
SamplingEpoch: "1.000000e+00" [Sampling is used for the first 'SamplingEpoch' epochs, afterwards, all events are taken for training]
SamplingImportance: "1.000000e+00" [ The sampling weights of events in epochs which successful (worse estimator than before) are multiplied with SamplingImportance, else they are divided.]
SamplingTraining: "True" [The training sample is sampled]
SamplingTesting: "False" [The testing sample is sampled]
ResetStep: "50" [How often BFGS should reset history]
Tau: "3.000000e+00" [LineSearch "size step"]
BPMode: "sequential" [Back-propagation learning mode: sequential or batch]
BatchSize: "-1" [Batch size: number of events/batch, only set if in Batch Mode, -1 for BatchSize=number_of_events]
ConvergenceImprove: "1.000000e-30" [Minimum improvement which counts as improvement (<0 means automatic convergence check is turned off)]
ConvergenceTests: "-1" [Number of steps (without improvement) required for convergence (<0 means automatic convergence check is turned off)]
UpdateLimit: "10000" [Maximum times of regulator update]
CalculateErrors: "False" [Calculates inverse Hessian matrix at the end of the training to be able to calculate the uncertainties of an MVA value]
WeightRange: "1.000000e+00" [Take the events for the estimator calculations from small deviations from the desired value to large deviations only over the weight range]
##


#VAR -*-*-*-*-*-*-*-*-*-*-*-* variables *-*-*-*-*-*-*-*-*-*-*-*-

NVar 8
nHits                         nHits                         nHits                         nHits                                                           'F'    [2,18]
e9e25Sh                       e9e25Sh                       e9e25Sh                       e9e25Sh                                                         'F'    [0.223209112883,1]
e1e9Sh                        e1e9Sh                        e1e9Sh                        e1e9Sh                                                          'F'    [0.159292444587,1]
sumUSh                        sumUSh                        sumUSh                        sumUSh                                                          'F'    [2.02385636028e-10,251.844528198]
sumVSh                        sumVSh                        sumVSh                        sumVSh                                                          'F'    [2.56061630233e-11,174.307571411]
asymUVSh                      asymUVSh                      asymUVSh                      asymUVSh                                                        'F'    [-1,1]
speedSh                       speedSh                       speedSh                       speedSh                                                         'F'    [-98087.1953125,479840.125]
dtTrSh                        dtTrSh                        dtTrSh                        dtTrSh                                                          'F'    [-999975.1875,164.864715576]
NSpec 0


============================================================================ */

#include <vector>
#include <cmath>
#include <string>
#include <iostream>

#ifndef IClassifierReader__def
#define IClassifierReader__def

class IClassifierReader {

 public:

   // constructor
   IClassifierReader() : fStatusIsClean( true ) {}
   virtual ~IClassifierReader() {}

   // return classifier response
   virtual double GetMvaValue( const std::vector<double>& inputValues ) const = 0;

   // returns classifier status
   bool IsStatusClean() const { return fStatusIsClean; }

 protected:

   bool fStatusIsClean;
};

#endif

class DNeutralShower_FCALQualityMLP : public IClassifierReader {

 public:

   // constructor
   DNeutralShower_FCALQualityMLP( std::vector<std::string>& theInputVars ) 
      : IClassifierReader(),
        fClassName( "DNeutralShower_FCALQualityMLP" ),
        fNvars( 8 ),
        fIsNormalised( false )
   {      
      // the training input variables
      const char* inputVars[] = { "nHits", "e9e25Sh", "e1e9Sh", "sumUSh", "sumVSh", "asymUVSh", "speedSh", "dtTrSh" };

      // sanity checks
      if (theInputVars.size() <= 0) {
         std::cout << "Problem in class \"" << fClassName << "\": empty input vector" << std::endl;
         fStatusIsClean = false;
      }

      if (theInputVars.size() != fNvars) {
         std::cout << "Problem in class \"" << fClassName << "\": mismatch in number of input values: "
                   << theInputVars.size() << " != " << fNvars << std::endl;
         fStatusIsClean = false;
      }

      // validate input variables
      for (size_t ivar = 0; ivar < theInputVars.size(); ivar++) {
         if (theInputVars[ivar] != inputVars[ivar]) {
            std::cout << "Problem in class \"" << fClassName << "\": mismatch in input variable names" << std::endl
                      << " for variable [" << ivar << "]: " << theInputVars[ivar].c_str() << " != " << inputVars[ivar] << std::endl;
            fStatusIsClean = false;
         }
      }

      // initialize min and max vectors (for normalisation)
      fVmin[0] = -1;
      fVmax[0] = 1;
      fVmin[1] = -1;
      fVmax[1] = 1;
      fVmin[2] = -1;
      fVmax[2] = 0.99999988079071;
      fVmin[3] = -1;
      fVmax[3] = 0.99999988079071;
      fVmin[4] = -1;
      fVmax[4] = 0.99999988079071;
      fVmin[5] = -1;
      fVmax[5] = 1;
      fVmin[6] = -1;
      fVmax[6] = 1;
      fVmin[7] = -1;
      fVmax[7] = 1;

      // initialize input variable types
      fType[0] = 'F';
      fType[1] = 'F';
      fType[2] = 'F';
      fType[3] = 'F';
      fType[4] = 'F';
      fType[5] = 'F';
      fType[6] = 'F';
      fType[7] = 'F';

      // initialize constants
      Initialize();

      // initialize transformation
      InitTransform();
   }

   // destructor
   virtual ~DNeutralShower_FCALQualityMLP() {
      Clear(); // method-specific
   }

   // the classifier response
   // "inputValues" is a vector of input values in the same order as the 
   // variables given to the constructor
   double GetMvaValue( const std::vector<double>& inputValues ) const;

 private:

   // method-specific destructor
   void Clear();

   // input variable transformation

   double fMin_1[3][8];
   double fMax_1[3][8];
   void InitTransform_1();
   void Transform_1( std::vector<double> & iv, int sigOrBgd ) const;
   void InitTransform();
   void Transform( std::vector<double> & iv, int sigOrBgd ) const;

   // common member variables
   const char* fClassName;

   const size_t fNvars;
   size_t GetNvar()           const { return fNvars; }
   char   GetType( int ivar ) const { return fType[ivar]; }

   // normalisation of input variables
   const bool fIsNormalised;
   bool IsNormalised() const { return fIsNormalised; }
   double fVmin[8];
   double fVmax[8];
   double NormVariable( double x, double xmin, double xmax ) const {
      // normalise to output range: [-1, 1]
      return 2*(x - xmin)/(xmax - xmin) - 1.0;
   }

   // type of input variable: 'F' or 'I'
   char   fType[8];

   // initialize internal variables
   void Initialize();
   double GetMvaValue__( const std::vector<double>& inputValues ) const;

   // private members (method specific)

   double ActivationFnc(double x) const;
   double OutputActivationFnc(double x) const;

   int fLayers;
   int fLayerSize[3];
   double fWeightMatrix0to1[12][9];   // weight matrix from layer 0 to 1
   double fWeightMatrix1to2[1][12];   // weight matrix from layer 1 to 2

   double * fWeights[3];
};

inline void DNeutralShower_FCALQualityMLP::Initialize()
{
   // build network structure
   fLayers = 3;
   fLayerSize[0] = 9; fWeights[0] = new double[9]; 
   fLayerSize[1] = 12; fWeights[1] = new double[12]; 
   fLayerSize[2] = 1; fWeights[2] = new double[1]; 
   // weight matrix from layer 0 to 1
   fWeightMatrix0to1[0][0] = -5.67548018894114;
   fWeightMatrix0to1[1][0] = 0.671209634882693;
   fWeightMatrix0to1[2][0] = 2.08345048146891;
   fWeightMatrix0to1[3][0] = 8.03348403391923;
   fWeightMatrix0to1[4][0] = -2.99437705684878;
   fWeightMatrix0to1[5][0] = -5.77002471289812;
   fWeightMatrix0to1[6][0] = 0.148954096917011;
   fWeightMatrix0to1[7][0] = 0.872486561760455;
   fWeightMatrix0to1[8][0] = 3.41922816394206;
   fWeightMatrix0to1[9][0] = -19.5590817002943;
   fWeightMatrix0to1[10][0] = -2.66195505436744;
   fWeightMatrix0to1[0][1] = -0.508924769224103;
   fWeightMatrix0to1[1][1] = -0.413826939497546;
   fWeightMatrix0to1[2][1] = 2.55208743545114;
   fWeightMatrix0to1[3][1] = 8.74978061120493;
   fWeightMatrix0to1[4][1] = 2.91640933526037;
   fWeightMatrix0to1[5][1] = 15.5992956753358;
   fWeightMatrix0to1[6][1] = 4.16454503643829;
   fWeightMatrix0to1[7][1] = 0.335951584076802;
   fWeightMatrix0to1[8][1] = 0.443761406492043;
   fWeightMatrix0to1[9][1] = -1.27608307823435;
   fWeightMatrix0to1[10][1] = 0.723088796764191;
   fWeightMatrix0to1[0][2] = 0.00236735007849696;
   fWeightMatrix0to1[1][2] = -0.32748149144678;
   fWeightMatrix0to1[2][2] = 0.0378203764594197;
   fWeightMatrix0to1[3][2] = 0.864139835325729;
   fWeightMatrix0to1[4][2] = 1.1741041486026;
   fWeightMatrix0to1[5][2] = 0.202813866259565;
   fWeightMatrix0to1[6][2] = -2.99801331683627;
   fWeightMatrix0to1[7][2] = -0.753917826643766;
   fWeightMatrix0to1[8][2] = 1.42114606981556;
   fWeightMatrix0to1[9][2] = -1.66405308842105;
   fWeightMatrix0to1[10][2] = 0.602137259462235;
   fWeightMatrix0to1[0][3] = 0.531704015403295;
   fWeightMatrix0to1[1][3] = 11.2465707337611;
   fWeightMatrix0to1[2][3] = 12.3039731123197;
   fWeightMatrix0to1[3][3] = 0.843360503501562;
   fWeightMatrix0to1[4][3] = 68.7904388998873;
   fWeightMatrix0to1[5][3] = 20.7060822974567;
   fWeightMatrix0to1[6][3] = -3.47388885611392;
   fWeightMatrix0to1[7][3] = 14.191555888897;
   fWeightMatrix0to1[8][3] = -20.8337091131509;
   fWeightMatrix0to1[9][3] = -0.403970269048419;
   fWeightMatrix0to1[10][3] = 32.0709599268463;
   fWeightMatrix0to1[0][4] = 2.84373100343368;
   fWeightMatrix0to1[1][4] = 3.44508123789802;
   fWeightMatrix0to1[2][4] = 18.8527523704618;
   fWeightMatrix0to1[3][4] = 11.8130280910594;
   fWeightMatrix0to1[4][4] = 42.7072286627262;
   fWeightMatrix0to1[5][4] = 24.6370840569919;
   fWeightMatrix0to1[6][4] = -2.86684710766166;
   fWeightMatrix0to1[7][4] = 19.8622638845141;
   fWeightMatrix0to1[8][4] = -31.583590821179;
   fWeightMatrix0to1[9][4] = 7.27382508545517;
   fWeightMatrix0to1[10][4] = 20.3083673421654;
   fWeightMatrix0to1[0][5] = 0.500208715016816;
   fWeightMatrix0to1[1][5] = -0.966403723015704;
   fWeightMatrix0to1[2][5] = 1.27221558852442;
   fWeightMatrix0to1[3][5] = 2.26006771546241;
   fWeightMatrix0to1[4][5] = 0.0785642731700331;
   fWeightMatrix0to1[5][5] = 0.617801329166234;
   fWeightMatrix0to1[6][5] = 0.398921931295215;
   fWeightMatrix0to1[7][5] = 1.30742304878111;
   fWeightMatrix0to1[8][5] = -1.5854303524047;
   fWeightMatrix0to1[9][5] = 0.141055620539128;
   fWeightMatrix0to1[10][5] = 0.00899660912966566;
   fWeightMatrix0to1[0][6] = -0.0508883863396253;
   fWeightMatrix0to1[1][6] = -4.87942115767901;
   fWeightMatrix0to1[2][6] = -6.43897971268509;
   fWeightMatrix0to1[3][6] = -3.1501589923107;
   fWeightMatrix0to1[4][6] = -30.7770129585863;
   fWeightMatrix0to1[5][6] = -8.09296254959834;
   fWeightMatrix0to1[6][6] = 5.05250669726875;
   fWeightMatrix0to1[7][6] = -7.05308033490298;
   fWeightMatrix0to1[8][6] = 14.0953692508894;
   fWeightMatrix0to1[9][6] = 2.64029964307745;
   fWeightMatrix0to1[10][6] = -17.9294940033197;
   fWeightMatrix0to1[0][7] = -0.597870262314557;
   fWeightMatrix0to1[1][7] = 5.8670108587539;
   fWeightMatrix0to1[2][7] = 11.1416376728042;
   fWeightMatrix0to1[3][7] = 2.80651871091009;
   fWeightMatrix0to1[4][7] = 41.2273170837517;
   fWeightMatrix0to1[5][7] = 10.0532138457348;
   fWeightMatrix0to1[6][7] = -1.88967602119454;
   fWeightMatrix0to1[7][7] = 12.1243481622399;
   fWeightMatrix0to1[8][7] = -18.1601793890722;
   fWeightMatrix0to1[9][7] = -2.47560187111571;
   fWeightMatrix0to1[10][7] = 19.0890322060697;
   fWeightMatrix0to1[0][8] = -1.30446854774301;
   fWeightMatrix0to1[1][8] = 3.22895639429101;
   fWeightMatrix0to1[2][8] = 10.4491920342795;
   fWeightMatrix0to1[3][8] = 3.9033323280269;
   fWeightMatrix0to1[4][8] = 42.4730956576089;
   fWeightMatrix0to1[5][8] = 7.77940842702126;
   fWeightMatrix0to1[6][8] = -7.44115551912371;
   fWeightMatrix0to1[7][8] = 13.8025369842737;
   fWeightMatrix0to1[8][8] = -18.9472518819945;
   fWeightMatrix0to1[9][8] = -7.14534776675644;
   fWeightMatrix0to1[10][8] = 20.0808465165044;
   // weight matrix from layer 1 to 2
   fWeightMatrix1to2[0][0] = -1.36904487724954;
   fWeightMatrix1to2[0][1] = -3.88451495296165;
   fWeightMatrix1to2[0][2] = -3.21831596336124;
   fWeightMatrix1to2[0][3] = 0.515375727140815;
   fWeightMatrix1to2[0][4] = 2.75074777877701;
   fWeightMatrix1to2[0][5] = -1.26289162166287;
   fWeightMatrix1to2[0][6] = 1.52012250393066;
   fWeightMatrix1to2[0][7] = -2.62164354202067;
   fWeightMatrix1to2[0][8] = -1.59667983619438;
   fWeightMatrix1to2[0][9] = -2.22405141009694;
   fWeightMatrix1to2[0][10] = 8.38563962629394;
   fWeightMatrix1to2[0][11] = 0.68727802568716;
}

inline double DNeutralShower_FCALQualityMLP::GetMvaValue__( const std::vector<double>& inputValues ) const
{
   if (inputValues.size() != (unsigned int)fLayerSize[0]-1) {
      std::cout << "Input vector needs to be of size " << fLayerSize[0]-1 << std::endl;
      return 0;
   }

   for (int l=0; l<fLayers; l++)
      for (int i=0; i<fLayerSize[l]; i++) fWeights[l][i]=0;

   for (int l=0; l<fLayers-1; l++)
      fWeights[l][fLayerSize[l]-1]=1;

   for (int i=0; i<fLayerSize[0]-1; i++)
      fWeights[0][i]=inputValues[i];

   // layer 0 to 1
   for (int o=0; o<fLayerSize[1]-1; o++) {
      for (int i=0; i<fLayerSize[0]; i++) {
         double inputVal = fWeightMatrix0to1[o][i] * fWeights[0][i];
         fWeights[1][o] += inputVal;
      }
      fWeights[1][o] = ActivationFnc(fWeights[1][o]);
   }
   // layer 1 to 2
   for (int o=0; o<fLayerSize[2]; o++) {
      for (int i=0; i<fLayerSize[1]; i++) {
         double inputVal = fWeightMatrix1to2[o][i] * fWeights[1][i];
         fWeights[2][o] += inputVal;
      }
      fWeights[2][o] = OutputActivationFnc(fWeights[2][o]);
   }

   return fWeights[2][0];
}

inline double DNeutralShower_FCALQualityMLP::ActivationFnc(double x) const {
   // radial
   return exp(-x*x/2.0);
}
inline double DNeutralShower_FCALQualityMLP::OutputActivationFnc(double x) const {
   // sigmoid
   return 1.0/(1.0+exp(-x));
}
   
// Clean up
inline void DNeutralShower_FCALQualityMLP::Clear() 
{
   // clean up the arrays
   for (int lIdx = 0; lIdx < 3; lIdx++) {
      delete[] fWeights[lIdx];
   }
}
   inline double DNeutralShower_FCALQualityMLP::GetMvaValue( const std::vector<double>& inputValues ) const
   {
      // classifier response value
      double retval = 0;

      // classifier response, sanity check first
      if (!IsStatusClean()) {
         std::cout << "Problem in class \"" << fClassName << "\": cannot return classifier response"
                   << " because status is dirty" << std::endl;
         retval = 0;
      }
      else {
         if (IsNormalised()) {
            // normalise variables
            std::vector<double> iV;
            iV.reserve(inputValues.size());
            int ivar = 0;
            for (std::vector<double>::const_iterator varIt = inputValues.begin();
                 varIt != inputValues.end(); varIt++, ivar++) {
               iV.push_back(NormVariable( *varIt, fVmin[ivar], fVmax[ivar] ));
            }
            Transform( iV, -1 );
            retval = GetMvaValue__( iV );
         }
         else {
            std::vector<double> iV;
            int ivar = 0;
            for (std::vector<double>::const_iterator varIt = inputValues.begin();
                 varIt != inputValues.end(); varIt++, ivar++) {
               iV.push_back(*varIt);
            }
            Transform( iV, -1 );
            retval = GetMvaValue__( iV );
         }
      }

      return retval;
   }

//_______________________________________________________________________
inline void DNeutralShower_FCALQualityMLP::InitTransform_1()
{
   // Normalization transformation, initialisation
   fMin_1[0][0] = 2;
   fMax_1[0][0] = 16;
   fMin_1[1][0] = 2;
   fMax_1[1][0] = 18;
   fMin_1[2][0] = 2;
   fMax_1[2][0] = 18;
   fMin_1[0][1] = 0.323900818825;
   fMax_1[0][1] = 1;
   fMin_1[1][1] = 0.223209112883;
   fMax_1[1][1] = 1;
   fMin_1[2][1] = 0.223209112883;
   fMax_1[2][1] = 1;
   fMin_1[0][2] = 0.196359068155;
   fMax_1[0][2] = 1;
   fMin_1[1][2] = 0.159292444587;
   fMax_1[1][2] = 1;
   fMin_1[2][2] = 0.159292444587;
   fMax_1[2][2] = 1;
   fMin_1[0][3] = 3.04391463146e-08;
   fMax_1[0][3] = 123.788467407;
   fMin_1[1][3] = 2.02385636028e-10;
   fMax_1[1][3] = 251.844528198;
   fMin_1[2][3] = 2.02385636028e-10;
   fMax_1[2][3] = 251.844528198;
   fMin_1[0][4] = 1.07598518984e-09;
   fMax_1[0][4] = 32.411315918;
   fMin_1[1][4] = 2.56061630233e-11;
   fMax_1[1][4] = 174.307571411;
   fMin_1[2][4] = 2.56061630233e-11;
   fMax_1[2][4] = 174.307571411;
   fMin_1[0][5] = -1;
   fMax_1[0][5] = 1;
   fMin_1[1][5] = -1;
   fMax_1[1][5] = 1;
   fMin_1[2][5] = -1;
   fMax_1[2][5] = 1;
   fMin_1[0][6] = -1266.28295898;
   fMax_1[0][6] = 202.097763062;
   fMin_1[1][6] = -98087.1953125;
   fMax_1[1][6] = 479840.125;
   fMin_1[2][6] = -98087.1953125;
   fMax_1[2][6] = 479840.125;
   fMin_1[0][7] = -999975.1875;
   fMax_1[0][7] = 127.996780396;
   fMin_1[1][7] = -999973.375;
   fMax_1[1][7] = 164.864715576;
   fMin_1[2][7] = -999975.1875;
   fMax_1[2][7] = 164.864715576;
}

//_______________________________________________________________________
inline void DNeutralShower_FCALQualityMLP::Transform_1( std::vector<double>& iv, int cls) const
{
   // Normalization transformation
   if (cls < 0 || cls > 2) {
   if (2 > 1 ) cls = 2;
      else cls = 2;
   }
   const int nVar = 8;

   // get indices of used variables

   // define the indices of the variables which are transformed by this transformation
   static std::vector<int> indicesGet;
   static std::vector<int> indicesPut;

   if ( indicesGet.empty() ) { 
      indicesGet.reserve(fNvars);
      indicesGet.push_back( 0);
      indicesGet.push_back( 1);
      indicesGet.push_back( 2);
      indicesGet.push_back( 3);
      indicesGet.push_back( 4);
      indicesGet.push_back( 5);
      indicesGet.push_back( 6);
      indicesGet.push_back( 7);
   } 
   if ( indicesPut.empty() ) { 
      indicesPut.reserve(fNvars);
      indicesPut.push_back( 0);
      indicesPut.push_back( 1);
      indicesPut.push_back( 2);
      indicesPut.push_back( 3);
      indicesPut.push_back( 4);
      indicesPut.push_back( 5);
      indicesPut.push_back( 6);
      indicesPut.push_back( 7);
   } 

   static std::vector<double> dv;
   dv.resize(nVar);
   for (int ivar=0; ivar<nVar; ivar++) dv[ivar] = iv[indicesGet.at(ivar)];
   for (int ivar=0;ivar<8;ivar++) {
      double offset = fMin_1[cls][ivar];
      double scale  = 1.0/(fMax_1[cls][ivar]-fMin_1[cls][ivar]);
      iv[indicesPut.at(ivar)] = (dv[ivar]-offset)*scale * 2 - 1;
   }
}

//_______________________________________________________________________
inline void DNeutralShower_FCALQualityMLP::InitTransform()
{
   InitTransform_1();
}

//_______________________________________________________________________
inline void DNeutralShower_FCALQualityMLP::Transform( std::vector<double>& iv, int sigOrBgd ) const
{
   Transform_1( iv, sigOrBgd );
}
