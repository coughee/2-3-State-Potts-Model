#include <vector>
class Lattice {
 public:
  std::vector<std::vector< int > > lattice;
  std::vector<std::vector< double > > energies;
  std::vector< double > spinDensity;
  int size;
  int qMax;

  //current coordinates for change
  int curX;
  int curY;

  //current spin value
  int curSpin;

  //Magnetic field
  double mField;
  double spinCoupling;
  //Markov params
  double T;
  
  Lattice(int size, int qmax, double mField, double sc);
  Lattice(const Lattice& orig);
  virtual ~Lattice();
  void printLattice();
  void printEnergies();
  void pickSite();
  double calcEnergy(int i, int j);
  void doStep();
  void mcLoop(double Tstart, double Tend, double numSteps);
  void noTempLoop(double T, double numSteps);
  void changeSpin(int oldspin, int newspin);
  double calcOrderParam();
  void printSpins();
  void magFieldChange(double numMag, double numSteps);
 private:
  void initialiseLattice(int size);
  void randomConfig();
  void initialEnergies();
  double interaction(int s1, int s2);
  
};
