#include <cstdlib>
#include <iostream>
#include <fstream>

#include "arguments.hpp"
#include "configuration.hpp"

using namespace std;

int main(int argc, char *argv[]) {

  Arguments arguments(argc,argv);
  cerr << "Input: " << arguments.input << endl;
  cerr << "Output: " << arguments.output << endl;
  
  Configuration configuration(arguments.input);
  cerr << "Configuration " << arguments.input << " imported!" << endl;

  Configuration *newConfiguration = configuration.scaleUp();
  cerr << "Configuration scaled up!" << endl;
  
  newConfiguration->makeAdmissible();
  cerr << "Newly generated configuration made admissible!" << endl;

  newConfiguration->exportXML(arguments.output);
  cerr << "Newly generated configuration exported in " << arguments.output << "!" << endl;


  // print configuration stats
  cerr << "------------------------------" << endl;
  cerr << "Input configuration stats: " << endl;
  configuration.printStats();
  cerr << "------------------------------" << endl;
   
  cerr << "------------------------------" << endl;
  cerr << "Generated configuration stats:" << endl;
  newConfiguration->printStats();
  cerr << "------------------------------" << endl;
  
  delete newConfiguration;
  
  return EXIT_SUCCESS;
}
