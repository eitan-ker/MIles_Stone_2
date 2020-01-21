//
// Created by meni on 17/01/2020.
//

#ifndef MILE_STONE2__ASTAR_H_
#define MILE_STONE2__ASTAR_H_
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "Searcher.h"
#include "Searchable.h"
void WriteDirection(int decideDirection);
template<class T, class Q, class P>
class AStar : public Searcher<T, Q, P> {
 private:
  vector<State<P>*> openlist;
  vector<State<P>*> closedlist;
  double totalCost;
  string shortestPath = "";
  int whenToGetLine;
 public:
  string getShortestPath() {
    return this->shortestPath;
  }
  AStar() {
    this->totalCost = 0;
    this->whenToGetLine = 0;
  }

  double calculateG(State<P> *state) {
    return state->getCost();
  }
  double calculateF(Searcheable<T, P>* searchable, State<P> *state) {
    return calculateG(state) + calculateH(searchable, state);
  }
  double calculateH(Searcheable<T, P>* searcheable, State<P> *state) { // calculate H using Manhattan distance
    return abs(state->getState()->getRow() - searcheable->getGoalState()->getState()->getRow()) +
        abs(state->getState()->getCol() - searcheable->getGoalState()->getState()->getCol());
  }
  int decideWhereICameFrom(State<P>* state) {/*return the direction we move from parent : 0-initial state,
 * 1-move to the left
 * 2-move to the right
 * 3-move down
 * 4-move up*/
    if (state->getCameFrom() == nullptr) {
      return 0;
    }
    if (state->getState()->getRow() == state->getCameFrom()->getState()->getRow()) {
      if (state->getState()->getCol() > state->getCameFrom()->getState()->getCol()) {
        return 2;
      } else {
        return 1;
      }
    } else {
      if (state->getState()->getRow() > state->getCameFrom()->getState()->getRow()) {
        return 3;
      } else {
        return 4;
      }
    }
  }
  void WriteDirection(int decideDirection, double totalCost) {
    string extra;
    switch (decideDirection) {
      case 1:
        extra = "Left (" + to_string(this->totalCost) + ")";
        this->shortestPath+=extra;
        break;
      case 2 :
        extra = "Right (" + to_string(this->totalCost) + ")";
        this->shortestPath+=extra;
        break;
      case 3 :
        extra = "Down (" + to_string(this->totalCost) + ")";
        this->shortestPath+=extra;
        break;
      case 4 :extra = "Up (" + to_string(this->totalCost) + ")";
        this->shortestPath+=extra;
        break;
      default:break;
    }
  }
  Q search(Searcheable<T, P> *searcheable) {
    State<P> *square;
    typename std::vector<State<P>*>::iterator it;
    double minCost = 0, minCost1 = 0;
    int i = 0, currentSquare = 0;
    openlist.push_back(searcheable->getInitialState());
    while (!openlist.empty()) {
      minCost = calculateF(searcheable, openlist[0]);
      for(auto itr : openlist) {
        minCost1 = calculateF(searcheable, itr);
        if (minCost > minCost1) { // find the point with the least f in the open list
          minCost = minCost1;
          currentSquare = i;
        }
        i++;
      }
      i = 0;
      typename std::vector<State<P>*>::iterator it1 = openlist.begin() + currentSquare;
      square = *it1;
      openlist.erase(it1);
      int decideDirection = decideWhereICameFrom(*it1);
      totalCost += (*it1)->getCost();
      WriteDirection(decideDirection, totalCost);
      if(decideDirection!= 0) {
        this->shortestPath+=",";
      }
      this->whenToGetLine++;
      if(this->whenToGetLine > 10) {
        this->shortestPath+="\r\n";
        this->whenToGetLine = 0;
      }
      vector<State<P>*> successors = searcheable->getAllPossibleStates(square); // generate square's successors
      for (it = successors.begin(); it != successors.end(); ++it) {
        (*it)->setCameFrom(*it1);
      }
      for (it = successors.begin(); it != successors.end(); ++it) {
        if ((*it)->Equals(searcheable->getGoalState())) {
          decideDirection = decideWhereICameFrom(*it);
          totalCost += (*it)->getCost();
          WriteDirection(decideDirection,totalCost);
          return getShortestPath();
        } else {
          State<P>* successor = *it;
          typename std::vector<State<P>*>::iterator it2 = std::find(openlist.begin(), openlist.end(), successor);
          typename std::vector<State<P>*>::iterator it3 = std::find(closedlist.begin(), closedlist.end(), successor); //find successor in closedlist
          if (it2 != openlist.end() && successor->Equals(*it2)) { /* if a node with the same position as
              successor is in the OPEN list which has a
              lower f than successor, skip this successor*/
            if (calculateF(searcheable, successor) >= calculateF(searcheable, *it2)) {
              //skip this phase
            } else {
              openlist.push_back(successor);
            }
          } else if (it3 != closedlist.end() && successor->Equals(*it3)) {
            if (calculateF(searcheable, successor) >= calculateF(searcheable, *it3)) {
              //skip this phase
            } else {
              openlist.push_back(successor);
            }
          } else {
            openlist.push_back(successor);
          }
        }
      }
      closedlist.push_back(square);
    }
    throw "Failed to find the Destination Cell\n";
  }
};
#endif //MILE_STONE2__ASTAR_H_