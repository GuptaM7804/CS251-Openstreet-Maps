// graph.h <Starter Code>
// < Your name >
//
// Basic graph class using adjacency matrix representation.  Currently
// limited to a graph with at most 100 vertices.
//
// University of Illinois at Chicago
// CS 251: Fall 2021
// Project #7 - Openstreet Maps
//

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <unordered_map>

using namespace std;

template<typename VertexT, typename WeightT>
class graph {
 private:
  unordered_map<VertexT, unordered_map<VertexT, WeightT>> adjList;
  vector<VertexT> vertices;

  bool _LookupVertex(VertexT v) const {
    if (adjList.count(v) == 0) {
      return false;
    }
    return true;
  }

 public:
  //
  // constructor:
  //
  graph(int n) {}
  graph() {}

  void clear() {
    adjList.clear();
  }

  graph& operator=(const graph &other) {
    (*this).adjList = other.adjList;
  }

  //
  // NumVertices
  //
  // Returns the # of vertices currently in the graph.
  //
  int NumVertices() const {
    return static_cast<int>(this->adjList.size());
  }

  //
  // NumEdges
  //
  // Returns the # of edges currently in the graph.
  //
  int NumEdges() const {
    int count = 0;
    for (auto i : adjList) {
      for (unsigned int j = 0; j < i.second.size(); j++) {
        count++;
      }
    }
    return count;
  }

  //
  // addVertex
  //
  // Adds the vertex v to the graph if there's room, and if so
  // returns true.  If the graph is full, or the vertex already
  // exists in the graph, then false is returned.
  //
  bool addVertex(VertexT v) {
    if (_LookupVertex(v) == true) {
      return false;
    }
    unordered_map<VertexT, WeightT> map;
    adjList[v] = map;
    return true;
  }

  //
  // addEdge
  //
  // Adds the edge (from, to, weight) to the graph, and returns
  // true.  If the vertices do not exist or for some reason the
  // graph is full, false is returned.
  //
  // NOTE: if the edge already exists, the existing edge weight
  // is overwritten with the new edge weight.
  //
  bool addEdge(VertexT from, VertexT to, WeightT weight) {
    if (_LookupVertex(from) == false) {
      return false;
    }
    if (_LookupVertex(to) == false) {
      return false;
    }
    unordered_map<VertexT, WeightT> map = adjList[from];
    map[to] = weight;
    adjList[from] = map;
    return true;
  }

  //
  // getWeight
  //
  // Returns the weight associated with a given edge.  If
  // the edge exists, the weight is returned via the reference
  // parameter and true is returned.  If the edge does not
  // exist, the weight parameter is unchanged and false is
  // returned.
  //
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
    if (_LookupVertex(from) == false) {
      return false;
    }
    if (_LookupVertex(to) == false) {
      return false;
    }
    unordered_map<VertexT, WeightT> map;
    map = adjList.at(from);
    if (map.count(to) == 1) {
      weight = map.at(to);
      return true;
    } else {
      return false;
    }
  }

  //
  // neighbors
  //
  // Returns a set containing the neighbors of v, i.e. all
  // vertices that can be reached from v along one edge.
  // Since a set is returned, the neighbors are returned in
  // sorted order; use foreach to iterate through the set.
  //
  set<VertexT> neighbors(VertexT v) const {
    set<VertexT>  S;
    if (_LookupVertex(v) == false) {
      return S;
    }
    unordered_map<VertexT, WeightT> map;
    map = adjList.at(v);
    for (auto& i : map) {
      S.insert(i.first);
    }
    return S;
  }

  //
  // getVertices
  //
  // Returns a vector containing all the vertices currently in
  // the graph.
  //
  vector<VertexT> getVertices() const {
    vector<VertexT> vertices;
    for (auto& i : adjList) {
      vertices.push_back(i.first);
    }
    return vertices;
  }

  //
  // dump
  //
  // Dumps the internal state of the graph for debugging purposes.
  //
  // Example:
  //    graph<string,int>  G(26);
  //    ...
  //    G.dump(cout);  // dump to console
  //
  void dump(ostream& output) const {
    output << "***************************************************" << endl;
    output << "********************* GRAPH ***********************" << endl;

    output << "**Num vertices: " << this->NumVertices() << endl;
    output << "**Num edges: " << this->NumEdges() << endl;

    output << endl;
    output << "**Vertices:" << endl;
    int count = 0;
    for (auto i : adjList) {
      output  << " " << count << ". " << i.first << endl;
      count++;
    }

    output << endl;
    output << "**Edges:" << endl;

    unordered_map<VertexT, WeightT> map;
    for (auto x : adjList){
      map = adjList.at(x.first);
      output << " " << x.first << ": ";
      for (auto y : map) {
        output << "(" << x.first << "," << y.first << "," << y.second << ") ";
      }
      output << endl;
    }
  }
};
