#include"super.h"


beta::beta ()
{

}

void
beta::initBeta (parameters param)
{
  num = param.neuronNum;
  maxOrder = param.order;
  for (int i = 0; i < maxOrder - 1; i++)
    {
      vector < vector < packet > >tempOrder;
      for (int j = 0; j < num; j++)
	{
	  vector < packet > temp;
	  tempOrder.push_back (temp);
	}
      betas.push_back (tempOrder);
    }
  for (int i = 0; i < num; i++)
    {
      for (int j = 0; j < num; j++)
	{
	  packet tempPkt;
	  tempPkt.parents.push_back (j + 1);
	  tempPkt.delays.
	    push_back (myRandInt (param.delayRandL, param.delayRandH));
	  if (myRand (0, 1) < param.pConnect)
	    {
	      tempPkt.isConnection = true;
	      tempPkt.prob = myRand (param.pRandL, param.pRandH);
	    }
	  else
	    {
	      tempPkt.isConnection = false;
	      tempPkt.prob = param.pBetaZero;
	    }
	  betas[0][i].push_back (tempPkt);
	}
    }
  if (param.flag3Rand == true)
    {
      for (int i = 0; i < num; i++)
	{
	  for (int j = 0; j < num; j++)
	    {
	      for (int k = j + 1; k < num; k++)
		{
		  packet temp;
		  temp.parents.push_back (j + 1);
		  temp.parents.push_back (k + 1);
		  temp.delays.
		    push_back (myRandInt
			       (param.delayRandL, param.delayRandH));
		  temp.delays.
		    push_back (myRandInt
			       (param.delayRandL, param.delayRandH));
		  if (myRand (0, 1) < param.pConnect)
		    {
		      temp.isConnection = true;
		      temp.prob = myRand (param.pRandL, param.pRandH);
		    }
		  else
		    {
		      temp.isConnection = false;
		      temp.prob = param.pBetaZero;
		    }
		  betas[1][i].push_back (temp);
		}
	    }
	}
    }
  //cout << betas.size() << endl;
  //for(int i  =0;i<betas.size();i++)
  //      cout << betas[i].size() << endl;
}

#if 0

#endif
void
beta::loadEpisodeProbabilities (parameters param)
{
  fstream fin;
  fin.open (param.episodeFile.c_str (), ios::in);
  if (fin == NULL)
    {
      errAbort (" Episode File not found. Exiting... ");
    }
  int count;
  fin >> count;
  for (int i = 0; i < count; i++)
    {
      packet episodePkt;
      episodePkt.isConnection = true;
      int order;
      fin >> order;
      int child;
      fin >> child;
      for (int j = 1; j < order; j++)
	{
	  int tempParent, tempDelay;
	  fin >> tempParent;
	  fin >> tempDelay;
	  episodePkt.parents.push_back (tempParent);
	  episodePkt.delays.push_back (tempDelay);
	}
      fin >> episodePkt.prob;
      //cout << order <<" "<< child;
      episodePkt.correct ();
      //episodePkt.displayPacket();
      int index = 0;
      for (int j = 0; j < episodePkt.parents.size (); j++)
	{
	  index = index * (num - j - 1) + (episodePkt.parents[j] - 1 - j);
	}
      if (order == 2)
	{
	  if (episodePkt.parents.size () != 1)
	    {
	      cout << "Episode number -" << i << " is wrong. " << endl;
	      exit (0);
	    }
	  betas[0][child - 1][index] = episodePkt;

	}
      else if (order == 3)
	{
	  if (episodePkt.parents.size () != 2)
	    {
	      cout << "Episode number -" << i << " is wrong. " << endl;
	      exit (0);
	    }
	  if (param.flag3Rand == true)
	    betas[1][child - 1][index] = episodePkt;
	  else
	    betas[1][child - 1].push_back (episodePkt);
	}
      else if (order >= 4)
	{
	  if (episodePkt.parents.size () != order - 1)
	    {
	      cout << "Episode number -" << i << " is wrong. " << endl;
	      exit (0);
	    }
	  betas[order - 2][child - 1].push_back (episodePkt);
	}
    }

}

void
beta::displayBeta (int order)
{
  for (int i = 0; i < betas[order - 2].size (); i++)
    {
      for (int j = 0; j < betas[order - 2][i].size (); j++)
	{
	  cout << i + 1;
	  betas[order - 2][i][j].displayPacket ();
	}
    }
}

void
beta::calculateBetas (parameters param)
{
  //calculating beta for order 2
  for (int i = 0; i < betas[0].size (); i++)
    {
      for (int j = 0; j < betas[0][i].size (); j++)
	{
	  if (betas[0][i][j].isConnection == false)
	    {
	      betas[0][i][j].beta = 0;
	      continue;
	    }
	  double lambdaTemp;
	  if (param.dist_flag == 0)
	    {
	      lambdaTemp = -log (1 - betas[0][i][j].prob) / param.teeUpdate;
	    }
	  else if (param.dist_flag == 1)
	    {
	      lambdaTemp = sqrt (2 * betas[0][i][j].prob) / param.teeUpdate;
	    }
	  betas[0][i][j].beta =
	    -param.theta - log (param.lambdaM / lambdaTemp - 1);
	}
    }
  //calculating beta for order 3
  for (int i = 0; maxOrder >= 3 && i < betas[1].size (); i++)
    {
      for (int j = 0; j < betas[1][i].size (); j++)
	{
	  if (betas[1][i][j].isConnection == false)
	    {
	      betas[1][i][j].beta = 0;
	      continue;
	    }
	  double lambdaTemp;
	  if (param.dist_flag == 0)
	    {
	      lambdaTemp = -log (1 - betas[1][i][j].prob) / param.teeUpdate;
	    }
	  else if (param.dist_flag == 1)
	    {
	      lambdaTemp = sqrt (2 * betas[1][i][j].prob) / param.teeUpdate;
	    }
	  //lambdaTemp = - log(1 - betas[1][i][j].prob) / param.teeUpdate;
	  double sigmaBetaSub = 0.0;
	  for (int k = 0; k < betas[1][i][j].parents.size (); k++)
	    {
	      int parent = betas[1][i][j].parents[k];
	      if (betas[0][i][parent - 1].delays[0] ==
		  betas[1][i][j].delays[k])
		sigmaBetaSub = sigmaBetaSub + betas[0][i][parent - 1].beta;
	    }
	  betas[1][i][j].beta =
	    -param.theta - sigmaBetaSub - log (param.lambdaM / lambdaTemp -
					       1);
	}
    }
#if 1
  //calculating beta for order 4 and above
  for (int k = 2; k < maxOrder - 1; k++)
    {
      for (int i = 0; i < betas[k].size (); i++)
	{
	  for (int j = 0; j < betas[k][i].size (); j++)
	    {
	      double lambdaTemp;
	      if (param.dist_flag == 0)
		{
		  lambdaTemp =
		    -log (1 - betas[k][i][j].prob) / param.teeUpdate;
		}
	      else if (param.dist_flag == 1)
		{
		  lambdaTemp =
		    sqrt (2 * betas[k][i][j].prob) / param.teeUpdate;
		}
	      //lambdaTemp = - log(1 - betas[k][i][j].prob) / param.teeUpdate;
	      double sigmaBetaSub = 0;
	      for (int m = 0; m < betas[k][i][j].parents.size (); m++)
		{
		  int parent = betas[k][i][j].parents[m];
		  if (betas[k][i][j].delays[m] ==
		      betas[0][i][parent - 1].delays[0])
		    sigmaBetaSub =
		      sigmaBetaSub + betas[0][i][parent - 1].beta;
		}
	      for (int low = 1; low < k; low++)
		{
		  for (int m = 0; m < betas[low][i].size (); m++)
		    {
		      if (isContained (betas[low][i][m], betas[k][i][j]))
			sigmaBetaSub = sigmaBetaSub + betas[low][i][m].beta;
		    }
		}
	      betas[k][i][j].beta =
		-param.theta - sigmaBetaSub -
		log (param.lambdaM / lambdaTemp - 1);
	    }
	}
    }
#endif

}

void
beta::displayMiniPkt (int event, vector < miniPkt > vec)
{
  cout << event + 1 << " -> { ";
  for (int i = 0; i < vec.size (); i++)
    {
      cout << vec[i].event + 1 << "(" << vec[i].time << ") ";
    }
  cout << endl;
}

void
beta::displayMiniPkt (int event, list < miniPkt > vec)
{
  cout << event + 1 << " -> { ";
  for (list < miniPkt >::iterator i = vec.begin (); i != vec.end (); i++)
    {
      cout << (*i).event + 1 << "(" << (*i).time << ") ";
    }
  cout << endl;
}

double
beta::getVoltage (int event, lookBack buf)
{
  int n = event - 1;
  double voltage = 0.0;
  for (int order = 0; order < betas.size (); order++)
    {
      for (int i = 0; i < betas[order][n].size (); i++)
	{
	  if (betas[order][n][i].isConnection == false)
	    continue;
	  bool flag = true;
	  for (int k = 0; k < order + 1 && flag == true; k++)
	    {
	      if (buf.
		  occurence (betas[order][n][i].delays[k],
			     betas[order][n][i].parents[k]) == false)
		flag = false;
	    }
	  if (flag == true)
	    voltage = voltage + betas[order][n][i].beta;
	}
    }
  return voltage;
}


void
beta::get2Values (vector < vector < data2 > >&outBeta)
{
  for (int i = 0; i < num; i++)
    {
      for (int j = 0; j < num; j++)
	{
	  outBeta[i][j].beta = betas[0][i][j].beta;
	  outBeta[i][j].isConnection = betas[0][i][j].isConnection;
	  outBeta[i][j].delay = betas[0][i][j].delays[0];
	}
    }
}


void
beta::simulation_fast (parameters param)
{
  // Initializing the data structures
  vector < vector < miniPkt > >frontConnect;
  vector < list < miniPkt > >dds;

  //allocation memory for the data structures
  for (int i = 0; i < param.neuronNum; i++)
    {
      vector < miniPkt > temp;
      list < miniPkt > temp1;
      frontConnect.push_back (temp);
      dds.push_back (temp1);
    }


  // getting the connections in the forward direction
  for (int odr = 0; odr < param.order - 1; odr++)
    {
      for (int i = 0; i < betas[odr].size (); i++)
	{
	  for (int j = 0; j < betas[odr][i].size (); j++)
	    {
	      if (betas[odr][i][j].isConnection == true)
		{
		  for (int k = 0; k < betas[odr][i][j].parents.size (); k++)
		    {
		      int parent = betas[odr][i][j].parents[k];
		      int delay = betas[odr][i][j].delays[k];
		      bool flag = true;
		      int pos;
		      for (int chk = 0;
			   chk < frontConnect[parent - 1].size (); chk++)
			{
			  if (frontConnect[parent - 1][chk].event == i)
			    {
			      pos = chk;
			      flag = false;
			      break;
			    }
			}
		      miniPkt temp;
		      temp.event = i;
		      temp.time = delay;
		      if (flag == true)
			frontConnect[parent - 1].push_back (temp);
		      else
			frontConnect[parent - 1][pos] = temp;
		    }
		}
	    }
	}
    }
// FOR DEBUGGING
#if 0
  for (int i = 0; i < frontConnect.size (); i++)
    {
      displayMiniPkt (i, frontConnect[i]);
    }
#endif
  //SIMULATION BEGINS
  fstream out;
  out.open (param.outfile.c_str (), ios::out);
  int num = param.neuronNum;
  int nUpdates = int (param.teeSim / param.teeUpdate);
  readfile stimFile (param.stimulusFile);


  for (int i = 0; i < nUpdates; i++)
    {
      vector < miniPkt > events;
      //cout << "time: " << i*param.teeUpdate << endl;
      for (int j = 0; j < num; j++)
	{
	  //displayMiniPkt(j,dds[j]);
	  double voltage = param.theta;
	  list < miniPkt >::iterator nextIter;
	  for (int odr = 1; odr < param.order - 1; odr++)
	    {
	      for (int pkts = 0; pkts < betas[odr][j].size (); pkts++)
		{
		  bool addVoltage = true, parentThere;
		  for (int k = 0;
		       addVoltage == true
		       && k < betas[odr][j][pkts].parents.size (); k++)
		    {
		      int parent = betas[odr][j][pkts].parents[k];
		      parentThere = false;
		      for (list < miniPkt >::iterator listIter =
			   dds[j].begin (); listIter != dds[j].end ();
			   listIter++)
			{
			  if (i * param.teeUpdate == (*listIter).time
			      && (*listIter).event == parent - 1)
			    {
			      parentThere = true;
			      break;
			    }
			}
		      addVoltage = addVoltage * parentThere;
		    }
		  if (addVoltage == true)
		    voltage = voltage + betas[odr][j][pkts].beta;
		}
	    }
	  for (list < miniPkt >::iterator listIter = dds[j].begin ();
	       listIter != dds[j].end (); listIter = nextIter)
	    {
	      nextIter = ++listIter;
	      --listIter;
	      if (i * param.teeUpdate == (*listIter).time)
		{
		  voltage = voltage + betas[0][j][(*listIter).event].beta;
		  dds[j].erase (listIter);
		}
	    }
	  double spikeTime;
	  if (param.dist_flag == 0)
	    {
	      // getting a spiking time - with exponential distribution
	      spikeTime = getExpSpikeTime (param.lambdaM, voltage);
	    }
	  else if (param.dist_flag == 1)
	    {
	      spikeTime = getGamma2SpikeTime (param.lambdaM, voltage);
	    }
	  if (spikeTime < param.teeUpdate)
	    {
	      miniPkt spkie;
	      
		// check point : modifying the below line to reduce time resolution to param.teeUpdate
		// spkie.time = i*param.teeUpdate+spikeTime;
		spkie.time = i * param.teeUpdate;
	      spkie.event = j + 1;
	      events.push_back (spkie);
	      //updating the dds of the neurons that are affected by firing of 'j'
	      for (int w = 0; w < frontConnect[j].size (); w++)
		{
		  int nextevent = frontConnect[j][w].event;
		  double nextdelay = frontConnect[j][w].time;
		  miniPkt temp;
		  temp.time = (i + nextdelay) * param.teeUpdate;
		  temp.event = j;
		  dds[nextevent].push_back (temp);
		}
	    }
	}

      // Including External Spikes into the data stream :)

      int spike;
      while ((spike = stimFile.getnextevent ()) != -1)
	{
	  double spikeTime = stimFile.getTime ();
	  if (spikeTime < (i + 1) * param.teeUpdate)
	    {
	      miniPkt tempevent;
	      tempevent.time = spikeTime;
	      tempevent.event = spike;
	      //pushing into the data stream
	      events.push_back (tempevent);
	      //updating dds
	      for (int w = 0; w < frontConnect[spike - 1].size (); w++)
		{
		  int nextevent = frontConnect[spike - 1][w].event;
		  double nextdelay = frontConnect[spike - 1][w].time;
		  miniPkt temp;
		  temp.time = (i + nextdelay) * param.teeUpdate;
		  temp.event = spike - 1;
		  dds[nextevent].push_back (temp);
		}
	    }
	  else
	    {
	      stimFile.pushBackOne ();
	      break;
	    }
	}
      //Sorting based on time
      mySort (events);
      for (int x = 0; x < events.size (); x++)
	{
	  out << events[x].event << "," << events[x].time << endl;
	}
    }
}
