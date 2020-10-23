#include "object.hpp"
#include "solving.hpp"
#include <iostream>
#include <new>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string>
#include "armadillo"
#include "time.h"
#include <stdio.h>
#include <tuple>
#include <math.h>
#include <typeinfo>
using namespace std;
using namespace arma;

int main(int argc, char const *argv[]) {

  int Dimension = 3;


  int IntegrationPoints = 5e7;
  double FinalTime = 100;

  //double TimeStep = FinalTime/((double) IntegrationPoints);
  double sun_mass = 1.0;
  double merc_mass = 1.651e-7;
  int fixed =0;
  int alpha =1;
  double beta = 2;
  cout << "Beta " << beta << endl;
  //When initializing the planets; make sure to initialize the sun last, and the planet furthest away from the sun second last.
  //Mercury
  object planetmercury(merc_mass,0.3075, 0,0 ,0,12.44,0);
  //Sun
  object sun(sun_mass, 0,0,0,0,0,0);

  solving binary_verlet;
  binary_verlet.add(planetmercury); binary_verlet.add(sun);
  binary_verlet.VelocityVerlet(Dimension,IntegrationPoints,FinalTime,1,0., beta, fixed,alpha);

  return 0;
}