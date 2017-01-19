#include <iostream>
#include <cstdlib>
#include <stack>

#include "lattice.hpp"
#include "tinyxml.h"

using namespace std;

//#define DEBUG_IMPORT
//#define DEBUG_SCALE_UP
//#define DEBUG_MAKE_ADMISSIBLE

#define EVEN(x) (x%2 == 1)
#define ODD(x) (x%2 == 0)

//#define DISABLE_SEED_CHECK

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
#ifdef DEBUG_IMPORT
  cerr << "Add " << c->id << " at "
       << "(" << c->position.getString2D() << ")"
       << endl;
#endif
}

Cell* Lattice::getCell(Vector2D &p) {
  return grid[getIndex(p)];
}

void Lattice::removeCell(Vector2D &p) {
  /*Cell *c = getCell(p);
    if ( c != NULL) {
    for (vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it) {
    if (c == *it) {
    cells.erase(it);
    break;
    }
    }
    delete c;
    grid[getIndex(p)] = NULL;
    }*/
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

Vector3D Lattice::dominantNeighborColor(Vector2D &p) {
  Cell *cell;
  list<Cell*> neighbors = getNeighbors(p);

  vector<Vector3D> neighborCol;
  vector<unsigned int> occ;
  
  for (list<Cell*>::iterator it=neighbors.begin();
       it != neighbors.end(); ++it) {
    cell = *it;
    bool found = false;
    unsigned int i = 0;
    for(vector<Vector3D>::iterator itc=neighborCol.begin(); itc != neighborCol.end(); itc++) {
      if(*itc == cell->color) {
	found = true;
	break;
      }
      i++;
    }
    if (!found) {
      neighborCol.push_back(cell->color);
      occ.push_back(1);
    } else {
      occ[i]++;
    }
  }

  Vector3D grey(100,100,100);
  // locate max
  unsigned int max = 0;
  for(unsigned int j = 1; j < occ.size(); j++) {
    if (occ[j] > occ[max]) {
      max = j;
    }
    if (occ[j] == occ[max] && neighborCol[max] == grey) {
      max = j;
    }
  }
  return neighborCol[max];
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
	//col = averageNeighborColor(pos);
	col = dominantNeighborColor(pos);
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

  border.clear();
  borderBool = vector<bool>(cells.size());
  borderBool.assign(borderBool.size(),false);
  
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
    stack.pop();
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
}

void Lattice::makeAdmissible() {
  Cell *c1, *c2;
  Vector2D pos;
  Vector3D col;
  bool change = true;
  list<Vector2D> neighborCells;
  int n;
  
  fillHoles();
  //cerr << "Holes filled!" << endl;

  //cerr << "Compute border..." << endl;
  computeBorder();
  //cerr << "Border computed!" << endl;
  
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
	  n = getNumNeighbors(pos,true);
	  if (n > 3) {
	    change = true;
	    col = dominantNeighborColor(pos);//averageNeighborColor(pos);
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
#define T_X_EVEN(x) (3*x + 3 - off)
#define T_X_ODD(x) (3*x + 1 - off) 
#define T_X_SEED(x) (3*x + 1)

Lattice* Lattice::scaleUp() {
  int off = 0;
  Vector2D tsize = Vector2D(T_X_EVEN(size.x), T_Y(size.y));
  Lattice *tl = new Lattice(tsize);

  Vector2D seedPos = getSeed();
  
  for (vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it) {
    Cell *ic = *it;
#ifdef DEBUG_SCALE_UP
    cerr << "ic: " << "(" << ic->position.getString2D() << ")" << endl;
#endif
    int ix = ic->position.x;
    int iy = ic->position.y;
#ifdef DISABLE_SEED_CHECK
    bool seed = false;
#else
    bool seed = isASeed(ic);
#endif
   
    if (!seed) {

      bool rightOfSeed = false;
      if (ix >= seedPos.x) {
	rightOfSeed = true; 
      }
      
      if (rightOfSeed) {
	off = 1;
      } else {
	off = 0;
      }
      
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
	
#ifdef DEBUG_SCALE_UP
      cerr << "add tc" << "(" << tv.getString2D() << ")" << endl;
#endif
      
      
      list<Vector2D> neighbors = tl->getConnectivity(tc);
      for (list<Vector2D>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
	Vector2D &pos = *it;
	Cell *n = tl->getCell(pos);
	if (n != NULL) {
	  cout << "ERROR: Cell has already been inserted!" << endl;
	  return NULL;
	}
        
#ifdef DEBUG_SCALE_UP
      cerr << "add tc" << "(" << pos.getString2D() << ")" << endl;
#endif
      
      n = new Cell(pos,tc->color);
      tl->insert(n);
      
      }
    } else {
      int ty = 0;
      int tx = T_X_SEED(ix);
	
      Vector2D tv = Vector2D(tx,ty);
	
      Cell *tc = new Cell(tv, ic->color);
      tl->insert(tc);
    }
  }
  return tl;
}

vector<Vector2D> Lattice::getCellsInCommon(Lattice *lattice) {
  vector<Vector2D> v;
  Cell *ic, *tc;
  for (vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it) {
    ic = *it;
    tc = lattice->getCell(ic->position);
    if (tc != NULL) {
      v.push_back(ic->position);
    }
  }
  return v;
}

bool Lattice::isASeed(Cell *c) {
  for (vector<Vector2D>::iterator it = seeds.begin(); it != seeds.end(); ++it) {
    Vector2D &s = *it;
    if (s == c->position) {
      return true;
    }
  }
  return false;
}

Vector2D Lattice::getSeed() {
  Vector2D c(0,0);
  if (seeds.size() > 0) {
    c = *(seeds.begin());
  }
  return c;
}

void Lattice::printSeeds() {
  cerr << "Seeds: ";
  for (vector<Vector2D>::iterator it = seeds.begin(); it != seeds.end(); ++it) {
    Vector2D &s = *it;
    cerr << "(" << s.getString2D() << ")" << ",";
  }
  cerr << endl;
}
