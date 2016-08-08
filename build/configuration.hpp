#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>
#include <vector>
#include <list>

#include "cell.hpp"
#include "lattice.hpp"
#include "vector3D.hpp"

//class Configuration;

class Configuration {

public:
  std::vector<Lattice*> lattices;
  
  Configuration();
  Configuration(std::string& _inputXML);
  Configuration(Vector2D& size);
  Configuration(const Configuration &l);
  ~Configuration();
  
  void importXML(std::string& _inputXML);
  void exportXML(std::string& _outputXML);

  Configuration* scaleUp();
  void makeAdmissible();

  void printStats();
};

#endif
