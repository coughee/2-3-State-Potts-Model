#include "Lattice.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cmath>

Lattice::Lattice(int size, int qMax, double mField, double sc){
  srand(time(0));
  this->qMax = qMax;
  this->size = size;
  this->mField = mField;
  this->spinCoupling = sc;
  
  initialiseLattice(size);
  randomConfig();
  
  
}

Lattice::Lattice(const Lattice& orig){

}

Lattice::~Lattice(){

}

void Lattice::initialiseLattice(int size){
  this->lattice.resize(size);
  for(int i = 0; i < size; i++){
    this->lattice[i].resize(size);
  }
  this->energies.resize(size);
  for(int i = 0; i < size; i++){
    this->energies[i].resize(size);
  }
  

}

void Lattice::initialEnergies(){
  for(int i = 0; i  < size; i++){
    for(int j = 0; j < size; j++){
      this->energies[i][j] = calcEnergy(i,j);
    }
  }
}

void Lattice::randomConfig(){
  this->spinDensity.resize(qMax);
  for(int i = 0; i < qMax; i++){
    spinDensity[i] = 0;
  }
  double curSpin;
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      curSpin = rand() % this->qMax;
      this->lattice[i][j] = curSpin;
      this->spinDensity[curSpin] += 1;
    }
  }
  initialEnergies();
  
}

void Lattice::printLattice(){
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      std::cout << this->lattice[i][j];
    }
    std::cout << "\n";
  }
}


void Lattice::printEnergies(){
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      std::cout << this->energies[i][j] << "\t";
    }
    std::cout << "\n";
  }
}

void Lattice::pickSite(){
  this->curX = rand() % this->size;
  this->curY = rand() % this->size;
}

double Lattice::calcEnergy(int i, int j){
  double ecur = 0;
  double cVal = this->lattice[i][j];
  if(i > 0){
    if(i < this->size - 1){
      ecur += interaction(cVal,this->lattice[i + 1][j]);
      ecur += interaction(cVal,this->lattice[i - 1][j]);
    }
    else{
      ecur += interaction(cVal,this->lattice[i - 1][j]);
      ecur += interaction(cVal,this->lattice[0][j]);
    }
  }
  else{
    ecur += interaction(cVal,this->lattice[i + 1][j]);
    ecur += interaction(cVal,this->lattice[this->size - 1][j]);
  }
  
  if(j > 0){
    if(j < this->size - 1){
      ecur += interaction(cVal,this->lattice[i][j + 1]);
      ecur += interaction(cVal,this->lattice[i][j - 1]);
    }
    else{
      ecur += interaction(cVal,this->lattice[i][j - 1]);
      ecur += interaction(cVal,this->lattice[i][0]);
    }
  }
  else{
    ecur += interaction(cVal,this->lattice[i][j + 1]);
    ecur += interaction(cVal,this->lattice[i][this->size - 1]);
  }
  if(cVal == 0){

      ecur += mField;

  }
  return ecur;
}

double Lattice::interaction(int s1, int s2){
  if(s1 == s2){
    return this->spinCoupling*1.;
  }
  else{
    return 0;
  }
}

void Lattice::doStep(){
  pickSite();
  int oldspin, newspin;
  double enew,eold,echange;
  oldspin = lattice[curX][curY];
  newspin = oldspin;

  while(newspin == oldspin){
    newspin = rand() % this->qMax;
  }
  lattice[curX][curY] = newspin;
  changeSpin(oldspin,newspin);
  eold = energies[curX][curY];
  enew = calcEnergy(curX,curY);

  echange = enew - eold;

  if(echange >= 0){
    //Accept
    energies[curX][curY] = enew;
    return;
  }
  else if(exp(echange/this->T) >= (double)rand()/(double)(RAND_MAX)){
    //Accept
    energies[curX][curY] = enew;
    return;
  }
  else{
    //Undo
    lattice[curX][curY] = oldspin;
    changeSpin(newspin,oldspin);
    return;
  }
  
}

void Lattice::changeSpin(int oldspin, int newspin){
  spinDensity[oldspin] -= 1;
  spinDensity[newspin] += 1;
}

void Lattice::mcLoop(double Tstart, double Tend, double numSteps){
  this->T = Tstart;
  

}

void Lattice::noTempLoop(double T, double numSteps){
  this->T = T;
  double numRecords = 100;
  double recordStep = numSteps / numRecords;
  double Tstep =exp((log(0.01) - log(T)) / numSteps);
  double count = 0;
  for(int i = 0; i < numSteps; i++){
    count += 1;
    if(count == recordStep){
      //      printLattice();
      //std::cout << "\n\n\n\n\n";
      count = 0;
      std::cout << calcOrderParam() << "\t" << this->T << std::endl;
      printSpins();
      std::cout << "\n\n";
    }
    //this->T *= Tstep;
    doStep();
  }
}

double Lattice::calcOrderParam(){
  double ox = 0,oy = 0;
  double s = 0.8660254;
  double c = 0.5;
  if(this->qMax == 2){
    return (double)(spinDensity[0] - spinDensity[1])/(this->size*this->size);
  }
  if(this->qMax == 3){
    oy += spinDensity[0];
    ox += spinDensity[1]*s;
    ox -= spinDensity[2]*s;
    oy -= spinDensity[1]*c;
    oy -= spinDensity[2]*c;
    return oy/this->size/this->size;
  }
}

void Lattice::printSpins(){
  for(int i = 0; i < qMax; i++){
    std::cout << i << ": " << spinDensity[i] << "\t";
  }
  std::cout << "\n";
}

void Lattice::magFieldChange(double numMag,double numSteps){
  this->T = 1;
  double magStart = -0.02;
  double magEnd = 0.02;
  double magStep = (magEnd - magStart)/numMag;
  this->mField = magStart;
  double magnetisation;
  std::vector<std::vector< double > > results;
  std::vector<double> curResult;
  curResult.resize(2);
  for(int i = 0; i < numMag; i++){
    curResult[0] = this->mField;
    curResult[1] = 0;
    for(int j = 0; j < numSteps; j++){
      if(j > numSteps/10){
	curResult[1] += calcOrderParam();
      }
      doStep();
    }
    curResult[1] /= numSteps;
    results.push_back(curResult);
    this->mField += magStep;
    randomConfig();
  }

  for(int i = 0; i < results.size(); i++){
    std::cout << results[i][0] << "\t" << results[i][1] << "\n";
  }

}



