#include"super.h"

int
main ()
{
  srand (time (0));
  parameters param;
  beta Beta;
  lookBack buf;
  param.getParameters ("inputfile.txt");
  cout << "parameters obtained.. " << endl;
  Beta.initBeta (param);
  cout << "init beta done.." << endl;
  Beta.loadEpisodeProbabilities (param);
  cout << "load probs done " << endl;
  Beta.calculateBetas (param);
  cout << "calculate betas done" << endl;
  //Beta.displayBeta(2);
  cout << "running simulationsssss.. ";
  Beta.simulation_fast (param);
  cout << " DONE." << endl;
  //buf.dumpStream("outfile");
}
