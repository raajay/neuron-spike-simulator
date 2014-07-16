#include"super.h"
//#define MAX_BUF 20


void
parameters::getParameters (string ipFile)
{
  fstream in;
  in.open (ipFile.c_str (), ios::in);
  cout << "\nINPUT PARAMETERS\n" << endl;
  while (in)
    {
      flag3Rand = false;
      episodeFile = "episodeFile.txt";
      stimulusFile = "stimulusFile.txt";
      outfile = "stream.txt";
      string str;
      in >> str;
      //if(str == "episode_file:")
      //              {in >> episodeFile;cout << "episode_file = " << episodeFile << endl; }
      //else if(str=="flag_random:")
      //              {string temp;in >> temp; if(temp == "yes") flag3Rand = true; else flag3Rand = false;
      //                      cout << "flag3Rand = "<<flag3Rand <<endl;}
      if (str == "numberOfNeurons:")
	{
	  in >> neuronNum;
	  cout << "Number of Neurons = " << neuronNum << endl;
	}
      else if (str == "pRandLow:")
	{
	  in >> pRandL;
	  cout << "Lower limit of Random Probability = " << pRandL << endl;
	}
      else if (str == "pRandHigh:")
	{
	  in >> pRandH;
	  cout << "Upper limit of Random Probability = " << pRandH << endl;
	}
      else if (str == "delayRandLow:")
	{
	  in >> delayRandL;
	  cout << "Lower Limit of Random Delay = " << delayRandL << endl;
	}
      else if (str == "delayRandHigh:")
	{
	  in >> delayRandH;
	  cout << "Upper Limit of Random Delay = " << delayRandH << endl;
	}
      else if (str == "randomFrequency:")
	{
	  in >> randFreq;
	  cout << "Random firing rate = " << randFreq << " Hz." << endl;
	}
      else if (str == "tUpdate:")
	{
	  in >> teeUpdate;
	  cout << "Time step  = " << teeUpdate << " seconds." << endl;
	}
      else if (str == "simulationTime:")
	{
	  in >> teeSim;
	  cout << "Total Simulation for " << teeSim << " seconds." << endl;
	}
      //else if(str == "stimulus_file:")
      //              {in >> stimulusFile;cout << "Stimulus taken from " << stimulusFile <<endl;}
      else if (str == "maxOrderOfInteraction:")
	{
	  in >> order;
	  cout << "Maximum order of interacton is : " << order << endl;
	}
      else if (str == "percentageConnections:")
	{
	  in >> pConnect;
	  cout << "Percentage of random connections = " << pConnect << endl;
	}
      else if (str == "spikeDistribution:")
	{
	  string temp;
	  in >> temp;
	  if (temp == "gamma2")
	    dist_flag = 1;
	  else
	    dist_flag = 0;
	  cout << "Inter-spike distribution is : " << temp << endl;
	}
      //else if(str == "output_file:")
      //              {in >> outfile;cout << "Output stored to " << outfile <<endl;}
      else;
    }
  if (dist_flag == 0)
    {
      lambdaNorm = randFreq;
      lambdaM = -log (1 - .99) / teeUpdate;
    }
  else if (dist_flag == 1)
    {
      lambdaNorm = sqrt (2 * randFreq / teeUpdate);
      lambdaM = sqrt (2 * .99) / teeUpdate;
    }
  theta = -log (lambdaM / lambdaNorm - 1);
  pBetaZero = 1 - exp (-lambdaM * teeUpdate / (1 + exp (-theta)));
  cout << "parameters calculated : lambda M = " << lambdaM << endl;
  cout << "parameters calculated : theta = " << theta << endl;
  cout << endl;


}


lookBack::lookBack ()
{
#if 1
  for (int i = 0; i < MAX_BUFF; i++)
    {
      vector < int >temp;
      table.push_back (temp);
    }
#endif
}

bool
lookBack::occurence (int delay, int event)
{
  //if(table.size() < delay) return false;
  bool falg = false;
  for (int i = 0; i < table[delay - 1].size () && falg == false; i++)
    {
      if (table[delay - 1][i] == event)
	falg = true;
    }
  return falg;
}

void
lookBack::updateLookBack (vector < int >newVect)
{
  int siz = table.size ();
  table[siz - 1].clear ();
  table.pop_back ();
  table.push_front (newVect);
}

void
lookBack::dumpStream (string outfile)
{
  fstream out;
  out.open (outfile.c_str (), ios::out);
  int n = table.size ();
  for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < table[n - 1 - i].size (); j++)
	{
	  cout << table[n - 1 - i][j] << "," << i * .001 << endl;
	}
    }
}
