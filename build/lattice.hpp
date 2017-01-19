#ifndef LATTICE_H_
#define LATTICE_H_

#include <string>
#include <vector>
#include <list>

#include "cell.hpp"
#include "vector3D.hpp"

class Lattice {

public:
  Lattice *parent;
  
  Vector2D size;
  Cell** grid;
  std::vector<Cell*> cells;
  std::vector<Cell*> border;
  std::vector<bool> borderBool;
  std::vector<Vector2D> seeds;

  Vector2D offset;
  
  Lattice();
  Lattice(Vector2D& size);
  Lattice(const Lattice &l);
  ~Lattice();

  inline void setParent(Lattice *p) {parent = p;}
  
  bool isIn(Vector2D &p); 
  int getIndex(Vector2D &p);
  void insert(Cell* c);
  Cell* getCell(Vector2D &p);
  void removeCell(Vector2D &p);
  
  void initGrid();
  
  std::list<Cell*> getNeighbors(Cell *c);
  std::list<Cell*> getNeighbors(Vector2D& p);
    
  std::list<Vector2D> getConnectivity(Cell *c);
  std::list<Vector2D> getConnectivity(Vector2D &p);

  std::list<Vector2D> getRelativeConnectivity(Vector2D &p);
  
  int getNumNeighbors(Cell *c, bool ground = false);
  int getNumNeighbors(Vector2D &p, bool ground = false);

  Vector3D averageNeighborColor(Vector2D &p);
  Vector3D dominantNeighborColor(Vector2D &p);
  
  // Self-reconfiguration algorithm: about admissibility
  void computeBorder();
  bool isAHole(Vector2D &p);
  void fillHoles();
  bool isFullySurrounded(Cell *c);
  void makeAdmissible();
  std::vector<Vector2D> getCellsInCommon(Lattice *lattice);
  bool isASeed(Cell *c);
  Vector2D getSeed();
  // Scale
  Lattice* scaleUp();

  void printSeeds();
};

#endif
