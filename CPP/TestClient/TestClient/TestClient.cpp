// TestClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#ifdef _WIN64
#include "pch.h"
//#endif

#include <iostream>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
//#include <boost/thread.hpp>
#include <math.h>
#include <cstdlib>

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

int main(int argc, char* argv[])
{
	std::cout << "Running version 1.1.111" << std::endl;
	// Test random numbers
	//for (int i = 0; i < 10; ++i)
	//	std::cout << static_cast<double>(std::rand() % 100)/10 << std::endl;
	//return 0;
	//std::vector<double> reductionCoeff(23, 1);
	//for (int i = 1945; i < 2100; i++) {
	//	//std::cout << (i-1940) % 25 << std::endl;
	//	std::cout << i << " -> " << std::floor((i-1945)/15) << std::endl;
	//}

	bool quit = false;
	std::string infile, outfile;
	if (argc > 1) {
		infile = argv[1];
		if (infile.compare("quit") == 0)
			quit = true;
	}

	if (argc > 2) {
		outfile = argv[2];
	}
	else {
		outfile = "testClientResults.dat";
	}

	boost::asio::io_service io_service;

	boost::asio::ip::tcp::socket socket(io_service);
	socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234));
	
	std::string msg;
	if (quit) {
		msg = "quit\n";
	}
	else {
		if (argc > 1) {
			std::ifstream indata(infile.c_str());
			if (!indata.good()) {
				std::cout << "Can't open the file " << infile << std::endl;
			}
			else {
				std::string line;
				bool firstLine = true;
				while (getline(indata, line)) {
					if (firstLine) {
						msg += line;
						firstLine = false;
					}
					else {
						msg += " " + line;
					}
				}
 				msg += " ENDofMSG\n";
			}
		}
		else {
			// Default message
			msg = "endSimYear 2100";
			msg += " startRed 2000";
			msg += " endRed 2007"; 
			msg += " flowScen CVHM_92_03_BUD0";
			msg += " loadScen GNLM";
			msg += " unsatScen C2VSIM_SPRING_2015";
			msg += " unsatWC 0.01";
			msg += " bMap CVHMfarms";
			msg += " Nregions 4 Farm21 Farm17 Farm12 Farm15";
			msg += " Ncrops 12"; 
			msg += " 301 0.5";
			msg += " 302 0.5";
			msg += " 303 0.4";
			msg += " 400 0.7";
			msg += " 401 0.7";
			msg += " 402 0.6";
			msg += " 605 0.3";
			msg += " 1451 0.75";
			msg += " 1452 0.253";
			msg += " 1460 0.5501";
			msg += " 212910 0.102";
			msg += " 10003 0.458";
			msg += " ENDofMSG\n";
		}
	}
		
	boost::system::error_code error;
	boost::asio::write(socket, boost::asio::buffer(msg), error);

	if (!error) {
		std::cout << "Client sent hello message" << std::endl;
	}
	else {
		std::cout << "sent failed" << std::endl;
	}

	boost::asio::streambuf receive_buffer;
	boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);

	if (error && error != boost::asio::error::eof) {
		std::cout << "receive failed: " << error.message() << std::endl;
	}
	else {
		const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
		//std::cout << data << std::endl;
		std::string str(data);
		//std::cout << str << std::endl;
		//std::stringstream ss(str.c_str());

		std::vector<std::string> str1 = split(str, " ");
		int ii = 0;
		//ss << data;
		int tf = std::atoi(str1[ii].c_str()); ii++;
		
		//ss >> tf;
		if (tf == 1) {
			int Nbtc = std::atoi(str1[ii].c_str()); ii++;
			int NsimYears = std::atoi(str1[ii].c_str()); ii++;
			std::cout << "Data size: " << Nbtc << " x " << NsimYears << std::endl;
			//ss >> Nbtc;
			std::ofstream outstream;
			std::cout << "Printing Results..." << std::endl;
			outstream.open(outfile.c_str());
			double dd;
			outstream << Nbtc << " " << NsimYears << std::endl;
			for (int i = 0; i < Nbtc; ++i) {
				for (int j = 0; j < NsimYears; ++j) {
					//ss >> dd;
					dd = std::atof(str1[ii].c_str()); ii++;
					outstream << dd << " ";
				}
				outstream << std::endl;
			}
			outstream.close();
		}
		else {
			std::cout << data << std::endl;
		}
	}
	return 0;
}
