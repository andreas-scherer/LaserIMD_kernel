// laserimd_kernel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>
#include "constants.h"

/*
The variables have the following meaning:
Px, Py and Pz: zero-field population
D, E: values for the zero-field splitting
omega_T: Zeeman frequency of the transient triplet
pts_grid_dip: Number of points of the grid of the dipolar vector
betas_dip: the angles of the grid of the dipolar vector
weights_dip: the weights for the grid of the dipolar vector
grid_T: the grid for the transient triplet label
alphas_T, betas_T, gammas_T: the Euler angles for the grid of the transient triplet
weights_T: the weights for the grid of the transient triplet
x_0, x_end, x_pts: the values for the x-axis for which the kernel is calculated
omega_dip: the dipolar coupling frequency
omega_p, omega_0, omega_m: the LaserIMD frequency for the three coherent transfer pathways (p stands for +, 0 for 0 and m for -)
Sp, S0, Sm: the signal conributions of the three coherent transfer pathways
Pp, P0, Pm: the population of the high-field triplet states
S: the result S_{LaserIMD}^{non-sec}
*/


std::vector<double> linspace(double, double, int);
std::tuple<std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>> load_grid(std::string);
void load_parameters(std::string, double&, double&, double&, double&, double&, double&, int&, std::string&, double&, double&, int&);

double w_dipole() {

	return mu_0 / 2 * mu_B*mu_B*gfree*gfree / hplanck * 1e21;
}


int main(int argc, char *argv[]) {


	if (argc < 3)
	{
		std::cout <<"Not enough arguments given" << std::endl;
		return 0;
	}


	double Px, Py, Pz, D, E, omega_T, x_0, x_end;
	int pts_grid_dip, x_pts;
	std::string grid_T;

	load_parameters(argv[1], Px, Py, Pz, D, E, omega_T, pts_grid_dip, grid_T, x_0, x_end, x_pts);
	
	
	std::cout << Px << std::endl;
	std::cout << Py << std::endl;
	std::cout << Pz << std::endl;
	std::cout << D << " MHz" << std::endl;
	std::cout << E << " MHz" << std::endl;
	std::cout << omega_T << " GHz" << std::endl;
	std::cout << pts_grid_dip << std::endl;
	std::cout << grid_T << std::endl;
	std::cout << x_0 << std::endl;
	std::cout << x_end << std::endl;
	std::cout << x_pts << std::endl;


	D = D * 1e6 * 2 * pi;
	E = E * 1e6 * 2 * pi;
	omega_T = omega_T * 1e9 * 2 * pi;

	auto x = linspace(x_0, x_end, x_pts);
	

	auto betas_dip = linspace(0, pi/2.0, pts_grid_dip);
	std::vector<double> weights_dip(betas_dip.size(), 0.0);
	double sum = 0;

	for (int k = 0; k < betas_dip.size(); ++k) {
		weights_dip[k] = sin(betas_dip[k]);
		sum += weights_dip[k];
	}

	for (int k = 0; k < betas_dip.size(); ++k) {
		weights_dip[k] = weights_dip[k]/sum; 
	}

	


	std::vector<double> alphas_T, betas_T, gammas_T, weights_T;


	std::tie(alphas_T, betas_T, gammas_T, weights_T) = load_grid(grid_T);

	
	std::vector<double> S(x.size(), 0.0);
	
	double omega_dip, delta_ZFS, omega_p, omega_0, omega_m, Sp, S0, Sm, Pp, P0, Pm;

	omega_dip = w_dipole();

	int n_T = alphas_T.size();

	for (int k=0; k< alphas_T.size();++k) {
		
		std::cout << k << " / " << n_T << std::endl;

		Pp = ((Pz / 2)*std::pow(sin(betas_T[k]), 2) + (Px / 2)*(std::pow(cos(betas_T[k]), 2) + std::pow(sin(betas_T[k]), 2) * std::pow(sin(gammas_T[k]), 2)) + (Py / 2)*(std::pow(cos(betas_T[k]), 2) + std::pow(sin(betas_T[k]), 2) * std::pow(cos(gammas_T[k]), 2)));
		P0 = (Pz *std::pow(cos(betas_T[k]), 2) + Px *(std::pow(sin(betas_T[k]), 2) * std::pow(cos(gammas_T[k]), 2)) + Py *(std::pow(sin(betas_T[k]), 2) * std::pow(sin(gammas_T[k]), 2)));
		Pm = Pp;

		for (int j = 0; j < betas_dip.size(); ++j) {

			

			
			delta_ZFS = (3.0 * sin(2.0 * betas_T[k])*cos(alphas_T[k])*D - 6.0 * sin(betas_T[k])*(cos(betas_T[k])*cos(2.0 * gammas_T[k])*cos(alphas_T[k]) - sin(2.0 * gammas_T[k])*sin(alphas_T[k]))*E) / (4.0 *omega_T);

			

			omega_p = (3.0 * std::pow(cos(betas_dip[j]), 2) - 1.0)*omega_dip + delta_ZFS * sin(2.0 * betas_dip[j]) * omega_dip;
			omega_0 = -2.0 * delta_ZFS * sin(2.0 * betas_dip[j]) *omega_dip;
			omega_m = -(3.0 * std::pow(cos(betas_dip[j]), 2) - 1.0)*omega_dip + delta_ZFS * sin(2.0 * betas_dip[j]) * omega_dip;

			

			for (int i = 0; i < x.size(); ++i) {

					
				Sp = cos(omega_p*x[i])*Pp;
				S0 = cos(omega_0*x[i])*P0;
				Sm = cos(omega_m*x[i])*Pm;
				
				S[i] = S[i] + weights_T[k] * weights_dip[j]* (Sp+S0+Sm);

			}


		}

	}


	std::ofstream save_file_x(argv[2] + std::string("_x.bin"), std::ios::binary);

	for (auto& x0 : x) {

		save_file_x.write(reinterpret_cast<char*>(&x0), sizeof(x0));
	}
	

	std::ofstream save_file_y(argv[2]+std::string("_y.bin"), std::ios::binary);

	for (auto& k : S) {
		
		save_file_y.write(reinterpret_cast<char*>(&k), sizeof(k));
	}
	


}