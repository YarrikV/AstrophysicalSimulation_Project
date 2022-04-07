#ifndef INITIALISATION_H_
#define INITIALISATION_H_

#include <iostream>

using namespace std;

// * Just an interface setup
string initialiser(double &delta, double &eps, double &h, double &time_end, double &size, int &write, string &initial, string &extension) {
    cout << "======================================================================================================================================" << endl;
    cout << "                              Welcome to HEROIN (HiERarchical Octree sImulatioNs)                                                                         " << endl;
    cout << "                                    by J. De Vuyst, S. Frederix,                                                                      " << endl;
    cout << "                                       L. Moreels and Y. Vanwalleghem                                                                 " << endl;
    cout << "======================================================================================================================================" << endl;


    cout << "                                   Declaration of some basic parameters                                                               " << "\n" << endl;

    int N;

    cout << "Please enter the amount of bodies (eg. 1000): " << flush;
    cin >> N;

    cout << "Please enter the softening parameter (a good value is N^(-.3)): " << flush;
    cin >> eps;
    
    cout << "Please enter the opening angle for the octree approximation (eg. 0.8): " << flush;
    cin >> delta;

    cout << "Please enter the time step (eg. .01): " << flush;
    cin >> h;

    cout << "For the size of the box you should consider taking the maximum coordinate of a particle and at least double this (eg. if the initial "
    << "conditions go from -L to L, take the size larger than 2L). Please enter the size of the box (eg. 20.): " << flush;
    cin >> size;

    cout << "Please enter for how long you want to simulate the system (eg. 12.): " << flush;
    cin >> time_end;

    cout << "Please enter per how many time steps you want to write to the files (eg. 10): " << flush;
    cin >> write;

    cout << "Please enter an extension for the files to write to (eg NBody): " << flush;
    cin >> extension;

    cout << "For the file with the initial conditions, the file must be of the following structure: " << "\n" << endl;
    cout << "\t" << "N" << endl;
    cout << "\t" <<  "m_1 x_1 y_1 z_1 vx_1 vy_1 vz_1" << endl;
    cout << "\t" << "..." << "\n" << endl;
    cout << "Please enter your file name (eg. initial.txt): " << flush;
    cin >> initial;

    cout << "\n" << "                                   Physics of the system                                                                              " << "\n" << endl;
  
    cout << "In this simulation we make use of natural units: given a length scale R0 and mass scale M0 we set G = 1 and the time and velocity scales "
    << "can then be calculated as: " << endl;
    cout << "\t" << "T0 = sqrt(R0^3 / G M0)" << "       \t" << "V0 = R0/T0" << "\n" << endl;

    cout << "There are 3 predefined sets: jovian (solar system), earth (solar system), binary (star system), cluster. If you want other simulation units please type other" << endl;
    cout << "Please enter your preferred system (eg. jovian): " << flush;

    string system;
    cin >> system;

    double G = 6.67408e-11;

    if(system == "jovian") {
        cout << "M0 = Jovian mass" << "\n"
        << "R0 = AU" << "\n"
        << "T0 = 5.15 y" << "\n"
        << "V0 = 0.19 AU/y" << endl;
    }
    else if(system == "earth") {
        cout << "M0 = Earth mass" << "\n"
        << "R0 = AU" << "\n"
        << "T0 = 91.9 y" << "\n"
        << "V0 = 0.011 AU/y" << endl;
    }
    else if(system == "binary") {
        cout << "M0 = Solar mass" << "\n"
        << "R0 = pc" << "\n"
        << "T0 = 14.92 My" << "\n"
        << "V0 = 0.067 pc/My" << endl;
    }
    else if(system == "cluster") {

        double M0 = N * 1.988e30;
        double R0 = 3.086e19;
        double T0 = sqrt(R0 * R0 * R0 / (G * M0)) / 60 / 60 / 24 / 365.25;
        double V0 = R0 / T0;

        cout << "M0 = N * Solar mass" << "\n"
        << "R0 = 1 kpc" << "\n"
        << "T0 = " << T0 << " y" << "\n"
        << "V0 = " << V0 << " kpc/y" << endl;
    }
    else {
        double M0;
        double R0;

        cout << "Please enter your mass scale in kg: " << flush;
        cin >> M0;

        cout << "Please enter your length scale in m: " << flush;
        cin >> R0;

        double T0 = sqrt(R0 * R0 * R0 / (G * M0)) / 60 / 60 / 24 / 365.25;
        R0 /= 3.086e16;
        double V0 = R0 / T0;

        cout << "M0 = " << M0 << " kg" << "\n"
        << "R0 = " << R0<< " pc" << "\n"
        << "T0 = " << T0 << " y" << "\n"
        << "V0 = " << V0 << " pc/y" << endl;
    }

    
    cout << "\n" << "                                   The Simulation                                                                              " << "\n" << endl;
    
    string integrator;
    cout << "Please choose your integrator (leapfrog or PEFRL): " << flush;
    cin >> integrator;

    return integrator;
}

#endif