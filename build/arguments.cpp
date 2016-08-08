#include <iostream>
#include <cstdlib>
#include "arguments.hpp"
#include "lattice.hpp"

using namespace std;

Arguments::Arguments() {}

Arguments::Arguments(int argc, char *argv[]) {

  argv++;
  argc--;

  while ((argc > 0) && (argv[0][0] == '-')) {
    switch(argv[0][1]) {
    case 'i':
      input = argv[1];
      argc--;
      argv++;
      break;
    case 'o':
      output = argv[1];
      argc--;
      argv++;
      break;
    case 'h':
      help();
      break;
    default:
      cerr << "ERROR: Unknown argument -" <<  argv[0][1] << endl;
      help();
    }
    argc--;
    argv++;
  }

  if (input == "") {
    cerr << "ERROR: No input file" << endl;
    help();
  }
}

Arguments::Arguments(const Arguments &a) {
  input = a.input;
  output = a.output;
}

Arguments::~Arguments() { }

void Arguments::help() {
  //cerr << "VisibleSim Configuration File To Graph Analyzer File:" << endl;
  cerr << "Usage: visibleSimCatoms2DConfigTool -i <input> -o <output> [options]"<< endl;
  cerr <<"Options:" << endl;
  cerr <<"-h                  : print this usage and exit" << endl;
  exit(EXIT_SUCCESS);
} 
