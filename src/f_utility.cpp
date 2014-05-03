#include"super.h"
#include"mtrand.h"



MTRand dRand(time(0));
void errAbort(string s)
{
	cout << s << endl;
	exit(0);
}
inline void errEcho(string s) { cout << s << endl;}
double myRand(double low,double high)
{
	if(low > high) errEcho("in myRand : Interval not valid");
	double diff = high - low;
	double temp = dRand();
	//double p01 = (double)(temp%10001) / 10001;
	return (low + diff*temp);
}
int myRandInt(int low,int high)
{
	if(low > high) errEcho("in myRandInt : Interval not valid");
	double temp = myRand(low,high);
	return int(temp);
}

double getExpSpikeTime(double lambdaM,double voltage)
{
	double lambdaTemp = lambdaM/(1 + exp(-voltage));
	double  u = myRand(0,1);
	return ((-1/lambdaTemp)*log(u));
}

double getGamma2SpikeTime(double lambdaM,double voltage)
{
	return (getExpSpikeTime(lambdaM,voltage)+getExpSpikeTime(lambdaM,voltage));
}
void mySort(vector<miniPkt> &events)
{
		for(int x=0;x<events.size();x++)
		{
			for(int y=x+1;y<events.size();y++)
			{
				double timetemp; int eventtemp;
				if(events[x].time > events[y].time)
				{
					timetemp = events[y].time; eventtemp = events[y].event;
					events[y].time = events[x].time ; events[y].event = events[x].event;
					events[x].time = timetemp ; events[x].event = eventtemp;
				}
			}
		}
}

bool isContained(packet sub, packet set)
{
	if(sub.parents.size() >= set.parents.size()) return false;
	bool flag1 = true;
	for(int i=0 ; i < sub.parents.size() && (flag1 == true) ;i++)
	{
		bool flag2 = false;
		for(int j = 0;j < set.parents.size()&& (flag2 == false); j++)
		{
			if(sub.parents[i] == set.parents[j] && sub.delays[i] == set.delays[j])
			{
				flag2 = true;
			}
		}
		if(flag2 == false) flag1 = false;

	}
	return flag1;
}

void packet::displayPacket()
{
	cout << " <- { ";
	for(int i=0;i < parents.size() ; i++)
	{
		cout << parents[i] << ", ";
	}
	cout << "} \t";
	cout << beta << "\t" << prob;
	if(isConnection) cout <<" Yes";
	else cout << " No";
	cout <<endl;
}

void packet::correct()
{
	for(int i=0 ;i < parents.size() ;i++)
	{
		int ind = i;
		for(int j = i+1;j < parents.size();j++)
		{
			if(parents[ind] > parents[j])
				ind = j;
		}
		if(ind != i)
		{
			int tempE = parents[ind];
			int tempD = delays[ind];
			parents[ind] = parents[i];
			delays[ind] = delays[i];
			parents[i] = tempE;
			delays[i] = tempD;
		}
	}
}
#if 0
void simulation(beta Beta,lookBack& buffer,parameters param)
{
	fstream out;
	out.open(param.outfile.c_str(),ios::out);
	int num = param.neuronNum;
	int nUpdates = int(param.teeSim / param.teeUpdate);
	readfile stimFile(param.stimulusFile);
	for(int i = 0 ; i < nUpdates ; i++)
	{
		vector<miniPkt> events;
		for(int j=0;j < num;j++)
		{
			double voltage = 0;
			voltage = Beta.getVoltage(j+1,buffer);
			voltage = param.theta + voltage;
			double spikeTime = getExpSpikeTime(param.lambdaM,voltage);
			if(spikeTime <= param.teeUpdate)
			{
				miniPkt temp;
				temp.time = spikeTime+i*param.teeUpdate;
				temp.event = j;
				events.push_back(temp);
			}
		}
		int spike;
		while((spike = stimFile.getnextevent()) != -1)
		{
			double spikeTime = stimFile.getTime();
			if(spikeTime < (i+1)*param.teeUpdate)
			{
				miniPkt temp;
				temp.time = spikeTime;
				temp.event = spike-1;
				events.push_back(temp);
			}
			else
			{
				stimFile.pushBackOne();
				break;
			}
		}
		mySort(events);
		vector<int> onlyEvents;
		for(int j=0;j< events.size();j++)
		{
			int spike = events[j].event + 1;
			onlyEvents.push_back(spike);
			out << spike <<","<<events[j].time<<endl;
		}
		buffer.updateLookBack(onlyEvents);
	}
	out.close();
}
#endif
#if 0
void beta::simulation_spl_order2(parameters param)
{
	// Initializing the data structures
	vector< vector<miniPkt> > frontConnect;
	vector< list<miniPkt> > dds;

	//allocation memory for the data structures
	for(int i=0;i<param.neuronNum;i++)
	{
		vector<miniPkt> temp;
		list<miniPkt> temp1;
		frontConnect.push_back(temp);
		dds.push_back(temp1);
	}


	// getting the connections in the forward direction
	for(int odr = 0;odr < param.order-1;odr++)
	{
		for(int i=0;i<betas[odr].size();i++)
		{
			for(int j=0;j<betas[odr][i].size();j++)
			{
				if(betas[odr][i][j].isConnection == true)
				{
					for(int k=0;k<betas[odr][i][j].parents.size();k++)
					{
						int parent = betas[odr][i][j].parents[k];
						int delay = betas[odr][i][j].delays[k];
						bool flag = true;
						for(int chk=0;chk<frontConnect[parent].size();chk++)
						{
							if(frontConnect[parent][chk].event == i)
							{
								flag = false; break;
							}
						}
						if(flag == true)
						{
							miniPkt temp;
							temp.event = i;
							temp.time = delay;
							frontConnect[parent-1].push_back(temp);
						}
					}
				}
			}
		}
	}
// FOR DEBUGGING
#if 0
	for(int i=0;i<frontConnect.size();i++)
	{
		cout << i+1 << "-> { ";
		for(int j=0;j<frontConnect[i].size();j++)
		{
			cout << frontConnect[i][j].event+1 << "("<<frontConnect[i][j].time<<") ";
		}
		cout <<" }"<<endl;
	}
#endif
	//SIMULATION BEGINS
	fstream out;
	out.open(param.outfile.c_str(),ios::out);
	int num = param.neuronNum;
	int nUpdates = int(param.teeSim / param.teeUpdate);
	readfile stimFile(param.stimulusFile);


	for(int i=0;i<nUpdates;i++)
	{
		vector<miniPkt> events;
		for(int j=0;j < num;j++)
		{
			double voltage = param.theta;
			list<miniPkt>::iterator nextIter;
			for(int odr=1;odr<param.order-1;odr++)
			{
				for(int pkts=0;pkts<betas[odr][j].size();pkts++)
				{
					bool addVoltage=true,parentThere;
					for(int k = 0;addVoltage==true && k<betas[odr][j][pkts].parents.size();k++)
					{
						int parent = betas[odr][j][pkts].parents[k];
						parentThere = false;
						for(list<miniPkt>::iterator listIter=dds[j].begin();listIter!=dds[j].end();listIter++)
						{
							if(i*param.teeUpdate==(*listIter).time && (*listIter).event == parent)
							{
								parentThere=true;break;
							}
						}
						addVoltage = addVoltage*parentThere;
					}
					if(addVoltage == true)
						voltage = voltage + betas[odr][j][pkts].beta;
				}
			}
			for(list<miniPkt>::iterator listIter = dds[j].begin();listIter != dds[j].end();listIter = nextIter)
			{
				nextIter = ++listIter;
				--listIter;
				if(i*param.teeUpdate == (*listIter).time)
				{
					voltage = voltage + betas[0][j][(*listIter).event].beta;
					dds[j].erase(listIter);
				}
			}
			// getting a spiking time - with exponential distribution
			double spikeTime = getExpSpikeTime(param.lambdaM,voltage);
			if(spikeTime <= param.teeUpdate)
			{
				miniPkt spkie;
				spkie.time = i*param.teeUpdate+spikeTime;
				spkie.event = j+1;
				events.push_back(spkie);
				//updating the dds of the neurons that are affected by firing of 'j'
				for(int w=0;w<frontConnect[j].size();w++)
				{
					int nextevent = frontConnect[j][w].event;
					double nextdelay = frontConnect[j][w].time;
					miniPkt temp;
					temp.time = (i+nextdelay)*param.teeUpdate;
					temp.event = j;
					dds[nextevent].push_back(temp);
				}
			}
		}
		int spike;
		while((spike = stimFile.getnextevent()) != -1)
		{
			double spikeTime = stimFile.getTime();
			if(spikeTime < (i+1)*param.teeUpdate)
			{
				miniPkt tempevent;
				tempevent.time = spikeTime;
				tempevent.event = spike;
				//pushing into the data stream
				events.push_back(tempevent);
				//updating dds
				for(int w=0;w<frontConnect[spike-1].size();w++)
				{
					int nextevent = frontConnect[spike-1][w].event;
					double nextdelay = frontConnect[spike-1][w].time;
					miniPkt temp;
					temp.time = (i+nextdelay)*param.teeUpdate;
					temp.event = spike-1;
					dds[nextevent].push_back(temp);
				}
			}
			else
			{
				stimFile.pushBackOne();
				break;
			}
		}
		//Sorting based on time
		mySort(events);
		for(int x=0; x<events.size() ; x++)
		{
			out << events[x].event << "," << events[x].time << endl;
		}
	}
}
#endif
