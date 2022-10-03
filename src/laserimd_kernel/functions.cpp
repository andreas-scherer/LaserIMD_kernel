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
	std::vector<double> alphas_t, betas_t, gammas_t, weights_t;


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

			alphas_t.push_back(std::stod(strs[0].c_str()));
			betas_t.push_back(std::stod(strs[1].c_str()));
			gammas_t.push_back(std::stod(strs[2].c_str()));
			weights_t.push_back(std::stod(strs[3].c_str()));

			std::getline(fid, tmp);
		} while (fid);


	}

	return std::make_tuple(alphas_t, betas_t, gammas_t, weights_t);


}




void load_parameters(std::string file, double& Px, double& Py, double& Pz, double& D, double& E, double& wI, int& grid_d, std::string& grid_t, double& t_0, double& t_end, int& t_size) {

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
			throw std::invalid_argument("no Px given in proper format");
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
			throw std::invalid_argument("no Py given in proper format");
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
			throw std::invalid_argument("no Pz given in proper format");
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
			throw std::invalid_argument("no D given in proper format");
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
			throw std::invalid_argument("no E given in proper format");
		}

		E = std::stod(matches_E[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}


	
									   
	// Load wI
	std::smatch matches_wI;
	std::regex wI_regexp("wI:[ ]+([\\d\\.]*)");

	std::regex_search(str, matches_wI, wI_regexp);

	try {
		if (matches_wI.size() == 0) {
			throw std::invalid_argument("no Larmor frequency given in proper format");
		}

		wI = std::stod(matches_wI[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}

	// Load grid_d
	std::smatch matches_grid_d;
	std::regex grid_d_regexp("grid_d:[ ]+([\\d\\.]*)");

	std::regex_search(str, matches_grid_d, grid_d_regexp);

	try {
		if (matches_grid_d.size() == 0) {
			throw std::invalid_argument("no grid for the dipolar vector in proper format");
		}

		grid_d = std::stod(matches_grid_d[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}


	// Load grid_t
	std::smatch matches_grid_t;
	std::regex grid_t_regexp("grid_t:[ ]+([a-zA-Z\\.\\\\_\\d]*)");

	std::regex_search(str, matches_grid_t, grid_t_regexp);

	try {
		if (matches_grid_t.size() == 0) {
			throw std::invalid_argument("no grid for the triplet in proper format");
		}

		grid_t = matches_grid_t[1];


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}




	// Load t_0
	std::smatch matches_t_0;
	std::regex t_0_regexp("t_0:[ ]+([-\\d\\.]*)");

	std::regex_search(str, matches_t_0, t_0_regexp);

	try {
		if (matches_t_0.size() == 0) {
			throw std::invalid_argument("no grid for the dipolar vector given in proper format");
		}

		t_0 = std::stod(matches_t_0[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}


	// Load t_end
	std::smatch matches_t_end;
	std::regex t_end_regexp("t_end:[ ]+([\\d\\.]*)");

	std::regex_search(str, matches_t_end, t_end_regexp);

	try {
		if (matches_t_end.size() == 0) {
			throw std::invalid_argument("no t_end given in proper format");
		}

		t_end = std::stod(matches_t_end[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}


	// Load n_t
	std::smatch matches_t_size;
	std::regex t_size_regexp("t_size:[ ]+([\\d\\.]*)");

	std::regex_search(str, matches_t_size, t_size_regexp);

	try {
		if (matches_t_size.size() == 0) {
			throw std::invalid_argument("no n_t given in proper format");
		}

		t_size = std::stod(matches_t_size[1]);


	}
	catch (std::invalid_argument &er) {
		std::cout << "Exception occured" << "\n";
		std::cout << er.what() << "\n";
	}






}
