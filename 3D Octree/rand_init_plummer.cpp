#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

//inverse functie voor de theta hoek.
double sph_theta(double x){
	return acos(1-2*x);
}

//inverse functie voor de phi hoek.
double sph_phi(double x){
	return 2*M_PI*x;
}

//inverse functie voor de straal.
double sph_r(double x, double a){
	return a/sqrt(pow(x,-(2/3.)) -1);
}

//genereert een random double uit een uniforme distributie, de seed 
//wordt zelf ook random bepaald en is niet reproduceerbaar.
double randdouble(){
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<double> unif(0.,1.);
    double r = unif(gen);
    return r;
}

//Maakt een random positie volgens de plummer distributie. Het maakt hiervoor
//gebruik van drie random double uit een uniforme distributie.
vector<double> pull_x(double a){
	double X1=randdouble();
	double X2=randdouble();
	double X3=randdouble();
	double theta=sph_theta(X1);
	double phi=sph_phi(X2);
	double x=sph_r(X3,a);
	vector<double> r(5);
	r[0]=x*sin(theta)*cos(phi);
	r[1]=x*sin(theta)*sin(phi);
	r[2]=x*cos(theta);
	r[3]=x;			 //r_coordinaat van het deeltje
	return r;
}

//Bepaalt de escape velocity uit de plummer straal, de totale massa en
//het r-coordinaat.
double v_esc(double a, double M, double r){
	return sqrt((2*M)/sqrt(r*r+a*a));
}

//de snelheidsdistributie van de plummersfeer.
double v_dist(double q){
	return (512*pow((1-q*q),3.5)*q*q)/(7.*M_PI);
}

//Maakt een random snelheid van het plummer deeltje, hiervoor wordt de
//rejection methode gebruikt. Phi en theta worden op dezelfde manier
//bepaald als in de "pull_x" functie.
vector<double> pull_v(double a, double M, double r){
	double X1=randdouble();
	double X2=randdouble();
	double p_max=(50176.*sqrt(2.))/(19683.*M_PI);
	double v_e=v_esc(a,M,r);
	double theta=sph_theta(X1);
	double phi=sph_phi(X2);
	double q;
	vector<double> v(3);
	int j=1;
	while(j==1){
		double x=randdouble();
		double y=p_max*randdouble();
		if(y <= v_dist(x)){
			q=x;
			j=0;
		}
	}
	v[0]=q*v_e*sin(theta)*cos(phi);
	v[1]=q*v_e*sin(theta)*sin(phi);
	v[2]=q*v_e*cos(theta);
	return v;
}

//Berekent het massacentrum van de plummer sfeer en past de originele 
//coordinaten ermee aan. Het schrijft ook uit wat het massacentrum was.
void COM(vector<vector<double>>& r, const vector<double>& m, int n, double m_t){
	vector<double> R(3);
	for(int i=0;i<n;i++){
		for(int j=0;j<3;j++){
			R[j] += m[i]*r[i][j];
		}
	}
	for(int i=0;i<3;i++){
		R[i] /= m_t;
	}
	for(int i=0;i<n;i++){
		for(int j=0;j<3;j++){
			r[i][j] -= R[j];
		}
	}
	for(double x:R){
		cout << x << endl;
	}
}

//Berekent de norm van twee vectoren
double norm(const vector<double>& x, const vector<double>& y){
	return sqrt((x[0]-y[0])*(x[0]-y[0]) + 
				(x[1]-y[1])*(x[1]-y[1]) + 
				(x[2]-y[2])*(x[2]-y[2]));
}

//berekent de potentielen energie, kinetische energie en totale energie
//en corrigeert de posities en snelheden ermee
void E(vector<vector<double>>& r,vector<vector<double>>& v,
		   const vector<double>& m, double n){
	double U=0.;
	double V=0.;
	for(int i=0;i<n;i++){
		for(int j=0;j<i;j++){
			U -= (m[i]*m[j])/norm(r[i],r[j]);
		}
	}
	for(int i=0;i<n;i++){
		V += 0.5*m[i]*(v[i][0]*v[i][0]+v[i][1]*v[i][1]+v[i][2]*v[i][2]);
	}
	for(int i=0;i<n;i++){
		for(int j=0;j<3;j++){
			r[i][j] = -2*U*r[i][j];
		}
	}
	for(int i=0;i<n;i++){
		for(int j=0;j<3;j++){
			v[i][j] = v[i][j]/(2.*sqrt(V));
		}
	}
	cout << "E_kin = " << V << endl;
	cout << "E_pot = " << U << endl;
	cout << "E_tot = " << U+V <<endl;
}

//Maakt de initiele condities van een plummer model met een gegeven
//straal 'a', aantal deeltjes 'n' en een cutoff afstand van 'cutoff'.
//De condities worden dan ook uitgeschreven naar een text bestand.
void init(int n, double a, double cutoff){
	vector<double> m(n);
	vector<double> r(n, (cutoff+1.));		//zodat alle waarden groter zijn dan de cutoff, dit is voor de while loop
	vector<vector<double>> x(n,vector<double>(5));
	vector<vector<double>> v(n,vector<double>(3));
	ofstream outfile ("initial_plummer.txt"); //naam geven aan de outfile
	outfile << setprecision(12);		//precisie van de outfile data bepalen
	for(int i=0;i<n;i++){
		while(r[i]>cutoff){		//het deeltje wordt toegelaten als het binnen de cutoff regio ligt
			x[i] = pull_x(a);
			m[i] = 1/((double)n);		//elk deeltje even zwaar
			r[i] = x[i][3];
		}
	}
	double m_t=0.;
	for(int i=0;i<n;i++){
		m_t += m[i];
	}
	cout << m_t << endl;
	for(int i=0;i<n;i++){
		v[i] = pull_v(a,m_t,r[i]);
	}
	COM(x,m,n,m_t);
	COM(x,m,n,m_t); // wordt een tweede keer aangeroepen om COM=0 te checken
	COM(v,m,n,m_t);
	COM(v,m,n,m_t); // wordt een tweede keer aangeroepen om vCOM=0 te checken
	E(x,v,m,n);
	E(x,v,m,n);		// wordt een tweede keer aangeroepen om E_tot = -1/4 te checken
	outfile << "#m x y z vx vy vz" << endl;
	for(int i=0;i<n;i++){
		outfile << m[i] << ' '
				<< x[i][0] << ' ' << x[i][1] << ' ' << x[i][2] << ' '
				<< v[i][0] << ' ' << v[i][1] << ' ' << v[i][2] << endl;
	}
}

int main(){
	int n=1;
	cout << "Amount of particles: " << flush;
	cin >> n;
	double a=(3.*M_PI/16.);
	double cutoff=5.;
	init(n,a,cutoff);
}
