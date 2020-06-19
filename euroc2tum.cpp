//
// Created by lab on 2020/6/8.
//

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>

using  namespace std;
int main() {

    double tmp;

    ifstream file_in_tum("/media/lab/E_Disk/My_Project/RESULTS/Vins-fusion-results/MH5/data.tum");
    ifstream file_in_csv("/media/lab/E_Disk/My_Project/RESULTS/Vins-fusion-results/MH5/data.csv");
    ofstream    file_out("/media/lab/E_Disk/My_Project/RESULTS/Vins-fusion-results/MH5/gt.txt");

    std::string config_file_line1;
    std::string config_file_line2;
    std::string cout_line1;
    std::string cout_line2;

    string timestamp;
    double tx;
    double ty;
    double tz;
    double qx;
    double qy;
    double qz;
    double qw;

    //TODO give up first line of data.csv
    std::getline(file_in_csv, config_file_line2);

    while (std::getline(file_in_tum, config_file_line1) && !file_in_tum.eof()) {

        std::istringstream line1(config_file_line1);
        line1 >> timestamp >>
              tx >>
              ty >>
              tz >>
              qx >>
              qy >>
              qz >>
              qw;

        std::getline(file_in_csv, config_file_line2);
        std::istringstream line2(config_file_line2);
        line2>>timestamp;
        timestamp = timestamp.substr(0,timestamp.find_first_of(","));
        file_out.precision(10);
        file_out << timestamp <<" "<<
                 tx <<" "<<
                 ty <<" "<<
                 tz <<" "<<
                 qx <<" "<<
                 qy <<" "<<
                 qz <<" "<<
                 qw << endl;
    }

    file_in_tum.close();

}
