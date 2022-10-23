#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <regex>
#include <stdexcept>
#include <tuple>


std::vector<double> linspace(double a, double b, int n) {
	std::vector<double> array;
	double step = (b - a) / (n - 1);

	while (a <= b) {
		array.push_back(a);
		a += step;           // could recode to better handle rounding errors
	}
	return array;
}

std::vector<std::string> split(std::string tmp, char c) {

	std::vector<std::string> res;

	int start = 0;

	for (int i = 0; i < tmp.length(); ++i) {
		if (tmp[i] == c) {
			res.push_back(tmp.substr(start, i - start));
			start = i + 1;
		}
		
	}
	res.push_back(tmp.substr(start, tmp.length() - start));

	return res;

}

namespace fs = std::experimental::filesystem;

std::tuple<std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>> load_grid(std::string filename) {


	std::ifstream fid(filename);
	std::vector<double> alphas_T, betas_T, gammas_T, weights_T;


	if (!fid) {
		std::cerr << "File " << filename << " cannot be opened\n";
	}
	else {

		std::string tmp;
		std::getline(fid, tmp);
		
		do {
			
			auto strs= split(tmp,  ',');
			
			if (strs.size() != 4) {
				std::cerr << "The triplet grid should contain 4 entries per line and not " << strs.size() << ".\n";
			}

			alphas_T.push_back(std::stod(strs[0].c_str()));
			betas_T.push_back(std::stod(strs[1].c_str()));
			gammas_T.push_back(std::stod(strs[2].c_str()));
			weights_T.push_back(std::stod(strs[3].c_str()));

			std::getline(fid, tmp);
		} while (fid);


	}

	return std::make_tuple(alphas_T, betas_T, gammas_T, weights_T);


}




void load_parameters(std::string file, double& Px, double& Py, double& Pz, double& D, double& E, double& omega_T, int& pts_grid_dip, std::string& grid_T, double& x_0, double& x_end, int& x_pts) {

	std::ifstream fid(file);

	std::stringstream strStream;
	strStream << fid.rdbuf(); //read the file
	std::string str = strStream.str(); //str holds the content of the file
	

	// Load Px
	std::smatch matches_Px;
	std::regex Px_regexp("Px:[ ]+([\\d\\.]*)");
	std::regex_search(str, matches_Px, Px_regexp);

	try {
		if (matches_Px.size() == 0) {
			throw std::invalid_argument("no zero-field population Px given in proper format");
		}

		Px = std::stod(matches_Px[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}


	
	


	// Load Py
	std::smatch matches_Py;
	std::regex Py_regexp("Py:[ ]+([\\d\\.]*)");
	std::regex_search(str, matches_Py, Py_regexp);

	try {
		if (matches_Py.size() == 0) {
			throw std::invalid_argument("no zero-field population Py given in proper format");
		}

		Py = std::stod(matches_Py[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}


	// Load Pz
	std::smatch matches_Pz;
	std::regex Pz_regexp("Pz:[ ]+([\\d\\.]*)");
	std::regex_search(str, matches_Pz, Pz_regexp);

	try {
		if (matches_Pz.size() == 0) {
			throw std::invalid_argument("no zero-field population Pz given in proper format");
		}

		Pz = std::stod(matches_Pz[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}

	// Load D
	std::smatch matches_D;
	std::regex D_regexp("D:[ ]+([-\\d\\.]*)");

	std::regex_search(str, matches_D, D_regexp);

	try {
		if (matches_D.size() == 0) {
			throw std::invalid_argument("no ZFS-value D given in proper format");
		}

		D = std::stod(matches_D[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}


	// Load E
	std::smatch matches_E;
	std::regex E_regexp("E:[ ]+([-\\d\\.]*)");

	std::regex_search(str, matches_E, E_regexp);

	try {
		if (matches_E.size() == 0) {
			throw std::invalid_argument("no ZFS-value E given in proper format");
		}

		E = std::stod(matches_E[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}


	
									   
	// Load wI
	std::smatch matches_omega_T;
	std::regex omega_T_regexp("omega_T:[ ]+([\\d\\.]*)");

	std::regex_search(str, matches_omega_T, omega_T_regexp);

	try {
		if (matches_omega_T.size() == 0) {
			throw std::invalid_argument("no Zeeman frequency omega_T given in proper format");
		}

		omega_T = std::stod(matches_omega_T[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}

	// Load grid_d
	std::smatch matches_pts_grid_dip;
	std::regex pts_grid_dip_regexp("pts_grid_dip:[ ]+([\\d\\.]*)");

	std::regex_search(str, matches_pts_grid_dip, pts_grid_dip_regexp);

	try {
		if (matches_pts_grid_dip.size() == 0) {
			throw std::invalid_argument("no grid for the dipolar vector in proper format");
		}

		pts_grid_dip = std::stod(matches_pts_grid_dip[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}


	// Load grid_t
	std::smatch matches_grid_T;
	std::regex grid_T_regexp("grid_T:[ ]+([a-zA-Z\\.\\\\_\\d]*)");

	std::regex_search(str, matches_grid_T, grid_T_regexp);

	try {
		if (matches_grid_T.size() == 0) {
			throw std::invalid_argument("no grid for the triplet in proper format");
		}

		grid_T = matches_grid_T[1];


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}




	// Load t_0
	std::smatch matches_x_0;
	std::regex x_0_regexp("x_0:[ ]+([-\\d\\.]*)");

	std::regex_search(str, matches_x_0, x_0_regexp);

	try {
		if (matches_x_0.size() == 0) {
			throw std::invalid_argument("no x_0 given in proper format");
		}

		x_0 = std::stod(matches_x_0[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}


	// Load t_end
	std::smatch matches_x_end;
	std::regex x_end_regexp("x_end:[ ]+([\\d\\.]*)");

	std::regex_search(str, matches_x_end, x_end_regexp);

	try {
		if (matches_x_end.size() == 0) {
			throw std::invalid_argument("no x_end given in proper format");
		}

		x_end = std::stod(matches_x_end[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}


	// Load n_t
	std::smatch matches_x_pts;
	std::regex x_pts_regexp("x_pts:[ ]+([\\d\\.]*)");

	std::regex_search(str, matches_x_pts, x_pts_regexp);

	try {
		if (matches_x_pts.size() == 0) {
			throw std::invalid_argument("no x_pts given in proper format");
		}

		x_pts = std::stod(matches_x_pts[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}






}
