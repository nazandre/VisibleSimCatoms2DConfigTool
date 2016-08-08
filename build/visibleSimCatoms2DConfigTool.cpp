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

  newConfiguration->makeItAdmissible();
  cerr << "Newly generated configuration made admissible!" << endl;

  newConfiguration->exportXML(arguments.output);
  cerr << "Newly generated configuration exported in " << arguments.output << "!" << endl;
  
  return EXIT_SUCCESS;
}
