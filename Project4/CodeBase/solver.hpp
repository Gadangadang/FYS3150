#ifndef SOLVER_HPP
#define SOLVER_HPP
#define _USE_MATH_DEFINES
#include <cmath>
#include "armadillo"

using namespace arma;
class solver
{
    public:
    // Attributes
    mat m_smatrix;
    long m_part;





    //Initializer
    void Initialize(int n_spins, int mcs, double init_temp, double final_temp, double t_step);
    //Functions
    void Metropolis(mat m_smatrix, double& E, double&M, double *w);
    void output();
    




};

#endif // SOLVER_MCINT 