#include <iostream>
#include "armadillo"
#include "solver.hpp"
#include <new>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <string>
#include "time.h"
#include <stdio.h>
#include <tuple>
#include <cmath>
// polluting the namespaces
using namespace arma;
using namespace std;


int main(int argc, char* argv[])
{
   solver Mcint1;
   Mcint1.init_output();
   clock_t start, finish;
   start = clock();
   double L = 20.;
   double T = 2.4;
   int mcs_max = 1e5;
   int param_1 = 0.;
   Mcint1.Initialize(L, mcs_max,T,param_1);
   string filename = "MonteCarloRun.txt";
   Mcint1.MonteCarloV1(filename);
   finish = clock();
   double timeused = (double) (finish - start)/(CLOCKS_PER_SEC );
   cout << setprecision(10) << "Time used  for computing (single thread) = " << timeused  << " Seconds"<<endl;
   //Mcint1.find_PE(20, 80000);
   string filename2 = "E.txt";
   Mcint1.print_E_av(7e4,filename2);
return 0;
}
