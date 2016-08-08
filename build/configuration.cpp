#include <iostream>
#include <fstream>
#include <cstdlib>

#include "configuration.hpp"
#include "tinyxml.h"

using namespace std;

/****** Lattice Class ******/
Configuration::Configuration() {}

Configuration::Configuration(string &_inputXML) {
  importXML(_inputXML);
}

Configuration::Configuration(Vector2D& s) {
  Lattice *initial = new Lattice(s);
  lattices.push_back(initial);
}

Configuration::Configuration(const Configuration &c) {
  assert(true);
}

Configuration::~Configuration() {
  for (vector<Lattice*>::iterator it = lattices.begin(); it != lattices.end(); ++it) {
    Lattice *l = *it;
    delete l;
  }
}

void Configuration::importXML(string &_inputXML) {
  TiXmlDocument xmlDoc(_inputXML);
  TiXmlNode *node = NULL;
  Vector3D defaultColor(100,100,100); // grey

  // open xml file
  if (!xmlDoc.LoadFile()) {
    cerr << "Could not load " << _inputXML << endl;
    exit(EXIT_FAILURE);
  }

  // read grid size
  node = xmlDoc.FirstChild("world");
  int lx = 0,ly = 0;
  if (node) {
    TiXmlElement* worldElement = node->ToElement();
    const char *attr= worldElement->Attribute("gridsize");
    if (!attr) {
      attr= worldElement->Attribute("gridSize");
    }
    if (attr) {
      string str=attr;
      int pos1 = str.find_first_of(',');
      int pos2 = str.find_last_of(',');
      lx = atoi(str.substr(0,pos1).c_str());
      ly = atoi(str.substr(pos2+1,str.length()-pos1-1).c_str());
    } else {
      cerr << "ERROR: Grid size not specified! " << endl;
      exit(EXIT_FAILURE);
    }
  } else {
    cerr << "ERROR: No world in XML file" << endl;
    exit(EXIT_FAILURE);
  }

  Vector2D size(lx,ly);
  cerr << "Grid size: "
       << "(x=" << size.x << ","
       << "z=" << size.y << ")"
       << endl;  

  // Grid initialization
  Lattice *initial = new Lattice(size);

  // read modules and import them into the grid
  TiXmlNode *nodeBlock = node->FirstChild("blockList");
  if (nodeBlock) {

    const char *attr= nodeBlock->ToElement()->Attribute("color");
    if (attr) {
      string str(attr);
      int pos1 = str.find_first_of(','),
	pos2 = str.find_last_of(',');
      defaultColor.x = atof(str.substr(0,pos1).c_str());
      defaultColor.y = atof(str.substr(pos1+1,pos2-pos1-1).c_str());
      defaultColor.z = atof(str.substr(pos2+1,str.length()-pos1-1).c_str());
    }
    
    nodeBlock = nodeBlock->FirstChild("block");
    while (nodeBlock) {
      Vector2D position;
      Vector3D color = defaultColor;
      TiXmlElement* element = nodeBlock->ToElement();
      attr = element->Attribute("position");
      if (attr) {
	string str(attr);
	int pos1 = str.find_first_of(',');
	int pos2 = str.find_last_of(',');
	position.x = atoi(str.substr(0,pos1).c_str());
	position.y = atoi(str.substr(pos2+1,str.length()-pos1-1).c_str());
      } else {
	// line number ?
	cerr << "ERROR: No position specified for a module!" << endl;
	exit(EXIT_FAILURE);
      }

      attr = element->Attribute("color");
      if (attr) {
	string str(attr);
	int pos1 = str.find_first_of(',');
	int pos2 = str.find_last_of(',');
	color.x = atof(str.substr(0,pos1).c_str());
	color.y = atof(str.substr(pos1+1,pos2-pos1-1).c_str());
	color.z = atof(str.substr(pos2+1,str.length()-pos1-1).c_str());
      } else {
	// line number ?
	cerr << "ERROR: No position specified for a module!" << endl;
	exit(EXIT_FAILURE);
      }
            
      // add block into the grid
      Cell *c = new Cell(position,color);
      initial->insert(c);
      nodeBlock = nodeBlock->NextSibling("block");
    }
  } else {
    cerr << "WARNING: No module in " << _inputXML << endl;
  }

  lattices.push_back(initial);

  // read targets
  node = xmlDoc.FirstChild("world");
  node = node->FirstChild("targetList");
  TiXmlNode *target = node->FirstChild("target");
  
  while(target) {
    TiXmlElement* element = target->ToElement();
    const char *attr = element->Attribute("format");
    if (attr) {
      string s(attr);
      if (attr != string("grid")) {
	cerr << "ERROR: Unsupported xml format" << endl;
      }
    }

    Lattice *targetLattice = new Lattice(size);
    TiXmlNode *cell = target->FirstChild("cell");
    while(cell) {
      Vector2D position;
      Vector3D color = defaultColor;
      TiXmlElement* element = cell->ToElement();
      attr = element->Attribute("position");
      if (attr) {
	string str(attr);
	int pos1 = str.find_first_of(',');
	int pos2 = str.find_last_of(',');
	position.x = atoi(str.substr(0,pos1).c_str());
	position.y = atoi(str.substr(pos2+1,str.length()-pos1-1).c_str());
      } else {
	// line number ?
	cerr << "ERROR: No position specified for a module!" << endl;
	exit(EXIT_FAILURE);
      }

      attr = element->Attribute("color");
      if (attr) {
	string str(attr);
	int pos1 = str.find_first_of(',');
	int pos2 = str.find_last_of(',');
	color.x = atof(str.substr(0,pos1).c_str());
	color.y = atof(str.substr(pos1+1,pos2-pos1-1).c_str());
	color.z = atof(str.substr(pos2+1,str.length()-pos1-1).c_str());
      } else {
	// line number ?
	cerr << "ERROR: No position specified for a module!" << endl;
	exit(EXIT_FAILURE);
      }
            
      // add block into the grid
      Cell *c = new Cell(position,color);
      targetLattice->insert(c);
      cell = cell->NextSibling("cell");
    }
    lattices.push_back(targetLattice);
    target = target->NextSibling("target");
  }  
}

void Configuration::exportXML(string &_outputXML) {
  ofstream output;
  Vector2D &latticeSize = lattices[0]->size;
  Vector3D sceneSize(latticeSize.x,5,latticeSize.y);
  Vector3D defaultColor = Vector3D(100,100,100);

  output.open (_outputXML);
  
  //target(angle azimut,angle elevation, distance) 
  int angleAzimut = 0;
  int angleElevation = 25;
  double coef = 115./100.;

  Vector3D midPoint = sceneSize/2;
  
  int distance = max(sceneSize.x, sceneSize.y)*coef;

  cerr << "exporting to VisibleSim..." << endl;

  output << "<?xml version=\"1.0\" standalone=\"no\" ?>" << endl;
  
  // header
  output << "<world gridSize=\"" <<  latticeSize.getString3D(1) << "\">" << endl;
  output << "  <camera target=\"" << midPoint.getString() << "\" directionSpherical=\"" << angleAzimut << "," << angleElevation << "," << distance << "\" angle=\"45\"/>" << endl;
  output << "  <spotlight target=\"" << midPoint.getString() << "\" directionSpherical=\"" << angleAzimut << "," << angleElevation << "," << distance << "\" angle=\"30\"/>" << endl;
  
  // initial configuration
  output << "  <blockList color=\""
	 << defaultColor.getString()
	 << "\">" << endl;

  Lattice *current = lattices[0];
  for (unsigned int i = 0; i < current->cells.size(); i++) {
    Cell *c = current->cells[i];
    
    output << "    <block position=\"" << c->position.getString3D(0) << "\""
	   << " color=\"" << c->color.getString() << "\""
	   << "/>" << endl;
  }
  output << "  </blockList>" << endl;

  // targets
  output << "  <targetList>" << endl;
  for (unsigned int i = 1; i < lattices.size(); i++) {   
    output << "    <target format=\"grid\">" << endl;

    current = lattices[i];
    for (unsigned int i = 0; i < current->cells.size(); i++) {
      Cell *c = current->cells[i];
      output << "    <cell position=\"" << c->position.getString3D(0) << "\""
	     << " color=\"" << c->color.getString() << "\""
	     << "/>" << endl;
    }
    
    output << "    </target>" << endl;
  }
  
  output << "  </targetList>" << endl;
  output << "</world>" << endl;
}

Configuration* Configuration::scaleUp() {
  Configuration *configuration = new Configuration();
  //Vector2D size;
  //int i = 0;
  
  for (vector<Lattice*>::iterator it = lattices.begin(); it != lattices.end(); ++it) {
    Lattice *l = *it;
    Lattice *tl = l->scaleUp();
    //i++;
    //    cerr << "ScaleUp lattice " << i << ": "
    // << l->cells.size() << " => " << tl->cells.size()
    //	 << endl;
    //size = tl->size;
    configuration->lattices.push_back(tl);
  }

  //cerr << "New configuration lattice size: "
  //     << "(" << size.x << "," << size.y << ")"
  //     << endl;
  
  return configuration;
}

void Configuration::makeAdmissible() {
  //int i = 0;
  for (vector<Lattice*>::iterator it = lattices.begin(); it != lattices.end(); ++it) {
    //i++;
    //cerr << "Make admissible: Lattice " << i << endl; 
    Lattice *l = *it;
    l->makeAdmissible();
  }
}

void Configuration::printStats() {
  Vector2D& size = lattices[0]->size;
  
  cerr << "Lattice size: "
       << "(" << size.x << "," << size.y << ")"
       << endl;

  cerr << "Initial: "
       << lattices[0]->cells.size()
       << endl;

  if (lattices.size() > 1) {
    cerr << "Initial: "
	 << lattices[1]->cells.size()
	 << endl;
  }
}
