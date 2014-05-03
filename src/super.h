#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<list>
#include<deque>
#include<cstring>
#include<string>
#include<cmath>
#include<algorithm>
using namespace std;

struct packet;
struct miniPkt;
class parameters;
class beta;
class lookBack;
inline void errEcho(string);
int myRandInt(int,int);
double myRand(double,double);
void errAbort(string);
bool isContained(packet,packet);
void simulation(beta,lookBack&,parameters);
double getExpSpikeTime(double,double);
double getGamma2SpikeTime(double,double);
void mySort(vector<miniPkt> &events);
//void simulation_spl_order2(beta,parameters);

struct miniPkt
{
	int event;
	double time;
};

struct data2
{
	double beta;
	double delay;
	bool isConnection;
};

struct packet
{
	bool isConnection;
	vector<int> parents;
	vector<int> delays;
	double beta;
	double prob;
	void displayPacket();
	void correct();
};

class parameters
{
public:
	int neuronNum;
	bool flag3Rand;
	double pRandL,pRandH;
	int delayRandL,delayRandH;
	string episodeFile,outfile;
	void getParameters(string);
	int randFreq;
	double teeUpdate;
	double lambdaM;
	double lambdaNorm;
	double theta;
	double teeSim;
	string stimulusFile;
	int order;
	double pConnect;
	double pBetaZero;
	int dist_flag;
};

#define BUFSIZE 10000
class readfile
{
	int* buffer;
	double* timebuffer;
	double timetemp;
	int pos;
	ifstream fptr;
	void buffin();
	void reset();
public:
	readfile(string);
	~readfile();
	int getnextevent(double&);
	int getnextevent();
	double getTime();
	void pushBackOne();	
};



class beta
{
	int num,maxOrder;
	vector< vector< vector<packet> > > betas;
	void displayMiniPkt(int,vector<miniPkt>);
	void displayMiniPkt(int,list<miniPkt>);
public:
	beta();
	void initBeta(parameters);
	void loadEpisodeProbabilities(parameters);
	void displayBeta(int order);
	void calculateBetas(parameters);
	double getVoltage(int,lookBack);
	void get2Values(vector< vector<data2> > &betas);
	void simulation_fast(parameters);
	
};

#define MAX_BUFF 50
class lookBack
{
	deque< vector<int> > table;
public:
	lookBack();
	bool occurence(int delay,int event);
	void updateLookBack(vector<int>);
	void dumpStream(string);
};
#if 0
class prob
{
	int num;
	double** prob2;
	double*** prob3;
	packet** prob4;
public:
	prob();
	void initProb(int);
	void loadEpisodeProbabilities(string);
	void putProbPkt(int child,packet pkt);
	void setRandProbs(double,double);
};
#endif
