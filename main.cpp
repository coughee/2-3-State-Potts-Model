#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include "Lattice.h"

using namespace std;

int main(int argc, char* argv[]) {
  if(argc < 7){
    cout << "Usage is: numsteps numspins magfield spincoupling latticesize Temperature. Exiting." << endl;
    return 0;
  }
  double numSteps = atof(argv[1]);
  int numSpins = atoi(argv[2]);
  double magfield = atof(argv[3]);
  double spinCoupling = atof(argv[4]);
  int latticesize = atoi(argv[5]);
  double Temp = atof(argv[6]);
    
  //(lattice size, num spins, mag field, spin coupling)
  Lattice lat = Lattice(latticesize,numSpins,magfield,spinCoupling);
  //lat.noTempLoop(Temp,numSteps);
  lat.magFieldChange(100,numSteps);
  //lat.printEnergies();
}
