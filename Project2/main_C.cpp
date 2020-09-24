#include "classtuff.hpp"
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

using namespace std;
using namespace arma;

int main(int argc, char const *argv[]) {

  //Define class object
  classtuff mysolver;

  //int size = pow(10,atof(argv[1]));
  int size = 350;
  double rho_max = 5;
  //Define matrix to solve Ax = lambda x
  mat A = zeros<mat>(size,size);

  //Initialize matrices
  A= mysolver.Initialize_C(size, A, rho_max);

  vec test_eigvals = mysolver.Jacobi_arm(A);
  mat qen = mysolver.Jacobi(A,1e-16);
  //cout << qen << endl;
  //cout << sort(test_eigvals) << endl;
  vec eigs = sort(qen.diag(),"descend");
  int b = eigs.n_elem;
  //cout << eigs << endl;
  cout << b << endl;
  cout << "eig 1: " << eigs[b-3] << endl;
  cout << "eig 2: " << eigs[b-2] << endl;
  cout << "eig 3: " << eigs[b-1] << endl;
  return 0;
}