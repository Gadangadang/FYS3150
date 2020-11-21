#include "armadillo"
#include "solver.hpp"
#include <iostream>
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
#include <stdlib.h>

// polluting the namespaces
using namespace arma;
using namespace std;


void solver::Initialize(int n_spins, int mcs, double init_temp, int param_1){
// Initialize internal Class variables
    m_smatrix = zeros<mat>(n_spins, n_spins);
    m_spins = n_spins;
    m_tot_spins = m_spins*m_spins;
    m_mcs = mcs;
    m_M = 0;
    m_E = 0;
    m_init_temp = init_temp;
    // long m_part = -1; // what does this do ? Example sets this to -1, calls it random??
    m_w = vec(17);
    m_average = vec(5);
    m_E_vals = vec(m_mcs);
    m_smatrix.fill(1);
// function to initialise energy, magnetization, and populate spin-matrix
    for(int y =0; y < m_spins; y++) {
    for (int x= 0; x < m_spins; x++){
    if(param_1==0){
      if(ran1()<0.5){m_smatrix(y,x) *=-1;}
    }
    m_M += (double) m_smatrix(y, x);
    }
    }

// setup initial energy
    for(int y =0; y < m_spins; y++) {
    for (int x= 0; x < m_spins; x++){
    // kan bytte om denne til å ligge på linje 37 ??
    m_E -= (double) m_smatrix(y, x)*(m_smatrix(periodic(y,m_spins,-1), x) + m_smatrix(y, periodic(x,m_spins,-1)));
    }
    }

// setup array for possible energy changes
    for( int de =-8; de <= 8; de++) m_w[de+8] = 0;
    for( int de =-8; de <= 8; de+=4) m_w[de+8] = exp(-de/init_temp);
}// end function initialise

double solver::ran1(){ // can I even call double(long) to specify long storage of type double?;
    double Rnum = dis(generator);
    return Rnum;
}

int solver::periodic(int i, int limit, int add){
    //  Algorithm to keep the iteration within the lattice by detecting the boundary.
    return (i+limit+add) % (limit);
}
void solver::Metropolis(){

// loop over all spins
            for(int spin =0; spin < m_tot_spins; spin++){
            int ix = (int) (ran1()*(double)m_spins);
            int iy = (int) (ran1()*(double)m_spins);
            int deltaE = 2*m_smatrix(iy, ix)*
            (m_smatrix(iy, periodic(ix,m_spins,-1)) + m_smatrix(periodic(iy,m_spins,-1), ix) +
             m_smatrix(iy, periodic(ix,m_spins,1)) + m_smatrix(periodic(iy,m_spins,1), ix));
            if ( ran1() < m_w(deltaE+8) ) {
            m_smatrix(iy, ix) *= -1; // flip one spin and accept new spin config
            // update energy and magnetization
            m_M += (double) 2*m_smatrix(iy, ix);
            m_E += (double) deltaE;
            m_counter++;
            }
}// End of the Metropolis function.
}

void solver::MonteCarloV1(){
        m_counter =0;
    // Monte Carlo cycles
    for (int cycles = 1; cycles <= m_mcs; cycles++){
        Metropolis();
    // update expectation values
        m_E_vals[cycles] = m_E;
        m_average(0) += m_E; m_average(1) += m_E*m_E;
        m_average(2) += m_M; m_average(3) += m_M*m_M; m_average(4) += fabs(m_M);
        m_cycles = cycles;
        output();
    }
}// end function MonteCarloV1

void solver::MonteCarloV2(string filename){
    // Monte Carlo cycles
    m_filename = filename;
    for (int cycles = 1; cycles <= m_mcs; cycles++){
        m_counter =0;
        Metropolis();
    // update expectation values
        m_average(0) += m_E; m_average(1) += m_E*m_E;
        m_average(2) += m_M; m_average(3) += m_M*m_M; m_average(4) += fabs(m_M);
        m_cycles = cycles;

    }
    tcoutput(m_filename);
}// end function MonteCarloV1

void solver::init_output(string filename){
  ofstream ofile;
      m_filename = filename;
      ofile.open(m_filename, ofstream::out | ofstream::trunc);
      ofile << setiosflags(ios::showpoint | ios::uppercase);
      ofile << setw(15) << "Temperature";
      ofile << setw(15) << "MC_cycles";
      ofile << setw(15) << "E average";
      ofile << setw(15) << "E variance";
      ofile << setw(15) << "M average";
      ofile << setw(15) << "M variance";
      ofile << setw(15) << "M abs total";
      ofile << setw(15) << "Specific heat Capacity";
      ofile << setw(15) << "Susceptibility";
      ofile << setw(15) << "Number of accepted configs" << endl;
  ofile.close();
}

void solver::print_E_av(int stabile_indx, string filename){
  ofstream ofile;
  ofile.open(filename);
  ofile << setiosflags(ios::showpoint | ios::uppercase);
  for(int i=stabile_indx+1;i<m_mcs; i++){
    ofile << setw(15) << setprecision(8) << m_E_vals[i]<<endl;
  }
  ofile.close();
}
void solver::output(){
// Borrowed most of this. Will probably make changes to the output structure, maybe.
  ofstream ofile;
  ofile.open(m_filename, fstream::app);
  double norma = 1/((double) (m_cycles));  // divided by total number of cycles
  double Etotal_average = m_average[0]*norma;
  double E2total_average = m_average[1]*norma;
  double Mtotal_average = m_average[2]*norma;
  double M2total_average = m_average[3]*norma;
  double Mabstotal_average = m_average[4]*norma;
  // all expectation values are per spin, divide by 1/n_spins/n_spins
  double Evariance = (E2total_average- Etotal_average*Etotal_average)/m_tot_spins;
  double Mvariance = (M2total_average - Mtotal_average*Mtotal_average)/m_tot_spins;
  ofile << setiosflags(ios::showpoint | ios::uppercase);
  ofile << setw(15) << setprecision(8) << m_init_temp;
  ofile << setw(15) << setprecision(8) << m_cycles;
  ofile << setw(15) << setprecision(8) << Etotal_average/m_tot_spins;
  ofile << setw(15) << setprecision(8) << Evariance/m_init_temp_sq;
  ofile << setw(15) << setprecision(8) << Mtotal_average/m_tot_spins;
  ofile << setw(15) << setprecision(8) << Mvariance/m_init_temp;
  ofile << setw(15) << setprecision(8) << Mabstotal_average/m_tot_spins;
  ofile << setw(15) << setprecision(8) << m_counter<<endl;
  ofile.close();
}// end output function

void solver::tcoutput(string filename){
  ofstream ofile;
  ofile.open(filename, fstream::app);
  double norma = 1/((double) (m_cycles));  // divided by total number of cycles
  double Etotal_average = m_average[0]*norma;
  double E2total_average = m_average[1]*norma;
  double Mtotal_average = m_average[2]*norma;
  double M2total_average = m_average[3]*norma;
  double Evariance = (E2total_average- Etotal_average*Etotal_average)/m_tot_spins;
  double Mvariance = (M2total_average - Mtotal_average*Mtotal_average)/m_tot_spins;
  double Mabstotal_average = m_average[4]*norma;

  double cv = Evariance/(m_init_temp*m_init_temp);
  double xi = Mvariance/(m_init_temp*m_init_temp);
  ofile << setiosflags(ios::showpoint | ios::uppercase);
    ofile << setw(15);
    ofile << setw(15);
    ofile << setw(15) << setprecision(8) << Etotal_average;
    ofile << setw(15);
    ofile << setw(15);
    ofile << setw(15);
    ofile << setw(15) << setprecision(8) << Mabstotal_average;
    ofile << setw(15) << setprecision(8) << cv;
    ofile << setw(15) << setprecision(8) << xi;
    ofile << setw(15);
  ofile.close();
}
