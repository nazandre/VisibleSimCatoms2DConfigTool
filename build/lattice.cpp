#include <iostream>
#include <cstdlib>
#include <stack>

#include "lattice.hpp"
#include "tinyxml.h"

using namespace std;

#define EVEN(x) (x%2 == 1)
#define ODD(x) (x%2 == 0)

/****** Lattice Class ******/
Lattice::Lattice() {
  grid = NULL;
}

Lattice::Lattice(Vector2D& s) {
  size = s; 
  initGrid();
}
Lattice::Lattice(const Lattice &l) {
  grid = NULL;
}

Lattice::~Lattice() {
  for (vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it) {
    Cell *n = *it;
    delete n;
  }
  delete [] grid;
}

int Lattice::getIndex(Vector2D &p) {
  int index = p.x + p.y*size.x;
  return index;
}

void Lattice::insert(Cell* c) {
  c->id = cells.size();
  cells.push_back(c);
  grid[getIndex(c->position)] = c;
}

Cell* Lattice::getCell(Vector2D &p) {
  return grid[getIndex(p)];
}

void Lattice::initGrid() {
  grid = new Cell*[size.x*size.y]{NULL};
}

bool Lattice::isIn(Vector2D &p) {
  return (p.x >= 0 && p.x < size.x &&
	  p.y >= 0 && p.y < size.y);
}

list<Cell*> Lattice::getNeighbors(Cell *c) {
  return getNeighbors(c->position);
}

list<Cell*> Lattice::getNeighbors(Vector2D& p) {
  list<Cell*> l;
  list<Vector2D> nCells = getConnectivity(p); // depends on p and lattice type
  for (list<Vector2D>::iterator it=nCells.begin();
       it != nCells.end(); ++it) {
    Cell* cell = getCell(*it);
    if (cell != NULL) {
      l.push_back(cell);
    }
  }
  return l;
}


list<Vector2D> Lattice::getConnectivity(Cell *c) {
  return getConnectivity(c->position);
}

list<Vector2D> Lattice::getConnectivity(Vector2D &p) {
  list<Vector2D> l;
  list<Vector2D> nCells = getRelativeConnectivity(p); // depends on p and lattice type
  for (list<Vector2D>::iterator it=nCells.begin();
       it != nCells.end(); ++it) {
    Vector2D v = p + *it;
    if (isIn(v)) {
      l.push_back(v);
    }
  }
  return l;
}

Vector3D Lattice::averageNeighborColor(Vector2D &p) {
  Vector3D col;
  Cell *cell;
  list<Cell*> neighbors = getNeighbors(p);

  col.x = 0; col.y = 0; col.z = 0;
  for (list<Cell*>::iterator it=neighbors.begin();
       it != neighbors.end(); ++it) {
    cell = *it;
    col = col + cell->color;
  }
  col.x = col.x / neighbors.size();
  col.y = col.y / neighbors.size();
  col.z = col.z / neighbors.size();
  return col;
}

list<Vector2D> Lattice::getRelativeConnectivity(Vector2D &p) {
  list<Vector2D> nCells;

  nCells.push_back(Vector2D(1,0));
  nCells.push_back(Vector2D(-1,0));
  
  nCells.push_back(Vector2D(0,1));
  nCells.push_back(Vector2D(0,-1));
  
  if (EVEN(p.y)) {
    nCells.push_back(Vector2D(1,1));
    nCells.push_back(Vector2D(1,-1));
  } else {
    nCells.push_back(Vector2D(-1,1));
    nCells.push_back(Vector2D(-1,-1));
  }

  return nCells;
}

int Lattice::getNumNeighbors(Cell *c, bool ground) {
  return getNumNeighbors(c->position,ground);
}

int Lattice::getNumNeighbors(Vector2D &p, bool ground) {
  int n = 0;
  list<Cell*> neighbors = getNeighbors(p);

  n = neighbors.size(); 
  if (ground && (p.y == 0)) {
    n += 2;
  }
  return n;
}

bool Lattice::isAHole(Vector2D &p) { // One cell for now!
  list<Cell*> neighbors;
  int n;
  //Cell *cell;
  
  if (getCell(p) == NULL) {
    n = getNumNeighbors(p,true);
    if (n == 6) { // 1-cell hole
      return true;
    }
    /*else { // more than 1-cell hole?
      neighbors = getNeighbors(p);
      bool hole = true;
      for (list<Cell*>::iterator it=neighbors.begin();
	   it != neighbors.end(); ++it) {
	cell = *it;
	if (!borderBool[cell->id]) {
	  return false;
	}
      }
    }
  } else {
    return false;
    }*/
  }
  return false;
}

void Lattice::fillHoles() {
  Cell *cell;
  Vector2D pos;
  Vector3D col;
  
  for (int x = 0; x < size.x; x++) {
    for (int y = 0; y < size.y; y++) {
      pos = Vector2D(x,y);
      if (isAHole(pos)) {
	col = averageNeighborColor(pos);
	cell = new Cell(pos,col);
	insert(cell);
      }
    }
  }
}

// Self-reconfiguration algorithm: about admissibility
bool Lattice::isFullySurrounded(Cell *c) {
  int n = getNumNeighbors(c,true);
  return (n == 6);
}

void Lattice::computeBorder() {
  Cell *c1, *c2;
  Vector2D pos;
  list<Cell*> neighbors;
  stack<Cell*> stack;
  
  borderBool.assign(border.size(),false);
  
  pos.y = 0;
  for (pos.x = 0; pos.x < size.x; pos.x++) {
    c1 = getCell(pos);
    if (c1) {
      break;
    }
  }
  border.push_back(c1);
  borderBool[c1->id] = true;
  stack.push(c1);
  
  while(!stack.empty()) {
    c1 = stack.top();
    neighbors = getNeighbors(c1);
    for (list<Cell*>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
      c2 = *it;
      if (!isFullySurrounded(c2) && !borderBool[c2->id]) {
	border.push_back(c2);
	borderBool[c2->id] = true;
	stack.push(c2);
      }
    }
  }

  /*for (vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it) {
    Cell *c = *it;
    if (isOnBorder(c)) {
      border.push_back(c);
    }
    }*/
}

void Lattice::makeItAdmissible() {
  Cell *c1, *c2;
  Vector2D pos;
  Vector3D col;
  bool change = true;
  list<Vector2D> neighborCells;
  int n;
  
  computeBorder();
  fillHoles();

  while (change) {
    change = false;
    for (vector<Cell*>::iterator it = border.begin(); it != border.end(); ++it) {
      c1 = *it;
      neighborCells = getConnectivity(c1);
      for (list<Vector2D>::iterator it = neighborCells.begin();
	   it != neighborCells.end(); ++it) {
	pos = *it;
	c2 = getCell(pos);
	if (c2 == NULL) { // is an empty cell
	  n = getNumNeighbors(pos);
	  if (n > 3) {
	    change = true;
	    col = averageNeighborColor(pos);
	    c2 = new Cell(pos,col);
	    // color
	    insert(c2);
	  }
	}
      }
    }
    if (change) {
      computeBorder();
    }
  }
}

#define T_Y(y) (3*y + 1)
#define T_X_EVEN(x) (3*x + 3)
#define T_X_ODD(x) (3*x + 1) 

Lattice* Lattice::scaleUp() {
  Vector2D tsize = Vector2D(T_X_EVEN(size.x), T_Y(size.y));
  Lattice *tl = new Lattice(tsize);

  for (vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it) {
    Cell *ic = *it;
    int ix = ic->position.x;
    int iy = ic->position.y;

    int ty = T_Y(iy);
    int tx;
    if (EVEN(iy)) {
      tx = T_X_EVEN(ix);
    } else {
      tx = T_X_ODD(ix);
    }
    Vector2D tv = Vector2D(tx,ty);

    Cell *tc = new Cell(tv, ic->color);
    tl->insert(tc);

    list<Vector2D> neighbors = getConnectivity(tc);
    for (list<Vector2D>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
      Vector2D &pos = *it;
      Cell *n = getCell(pos);
      if (n != NULL) {
	cout << "ERROR!" << endl;
	return NULL;
      }
      n = new Cell(pos,tc->color);
      tl->insert(n);
    }
  }
  return tl;
}
