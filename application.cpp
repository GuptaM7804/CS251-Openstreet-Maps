// graph.h <Starter Code>
// Name: Manav Gupta
// UIC Spring 2022
// Project 7 Openstreet Maps II
//
// creative component: 
// takes in map of Nodes, vector of Footways, vector of Buildings
// and a graph
// Takes in a bunch of building names and times
// the classes occur in and at
// and outputs the shortest route from each class to the next
// in order of when they occur

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <utility>
#include <limits>
#include <cassert>
#include "dist.h"
#include "graph.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;
double INF = numeric_limits<double>::max(); // infinity

// prioritize
//
// checks the second variable in from 2 pairs of long long and double
// if those are same, then checks first variable
class prioritize {
    public:
      bool operator()(const pair<long long, double>& p1,
  const pair<long long, double>& p2) const {
        if (p1.second == p2.second) {
         return p1.first > p2.first;
        } else {
          return p1.second > p2.second;
        }
      }
};

// Dijkstra
//
// algorithm to determine shortest path between vertices in a graph
// returns a vector of all the vertices visited
// uses a starting point to determine shortest path to each vertex
// updates the distances map and the predV map
// distances keeps track of distance between 1 vertex to another
// predV keeps track of previos vertex visited
vector<long long> Dijkstra(graph<long long, double>& G,
long long startV,
map<long long, double>& distances,
map<long long, double>& predV) {
  // current vertex
  pair<long long, double> curV;
  // priority queue of unvisited vertices
  priority_queue<pair<long long, double>,
  vector<pair<long long, double>>,
  prioritize> unvisitedPQ;
  // vector of vertices
  vector<long long> visited;
  // set of vertices
  set<long long> visiting;
  // all vertices in graph in a vector
  vector<long long> vertices = G.getVertices();

  for (auto e : vertices) {
    // each vertex is unreachable/edge weight is infinity
    distances[e] = INF;
    // push each vertex in priority queue
    unvisitedPQ.push(make_pair(e, INF));
  }

  // start vertex weight is 0 since we start at ti
  distances[startV] = 0;
  unvisitedPQ.push(make_pair(startV, 0));

  while (!unvisitedPQ.empty()) {
    // get top from priority queue (vertex with least weight)
    curV = unvisitedPQ.top();
    // remove top vertex
    unvisitedPQ.pop();

    // if distance is INF for to vertex then it is unreachable
    // so break out of loop
    if (distances[curV.first] == INF) {
      break;
    } else if (visiting.count(curV.first) == 1) {
      // if visiting set already has vertex, continue
      continue;
    } else {
      // else insert vertex in visited vector and set
      visited.push_back(curV.first);
      visiting.insert(curV.first);
    }

    // set of neighbors for current vertex
    set<long long> neighbors = G.neighbors(curV.first);
    for (auto i : neighbors) {
      double weight;
      // get weight of edge from current vertex to its neighbors
      G.getWeight(curV.first, i, weight);
      // alternate weight is current vertex's weight + new weight
      double alternate = distances[curV.first] + weight;

      // if alternate is less than neighbor's weight
      if (alternate < distances[i]) {
        // that neighbor's weight is alternate
        distances[i] = alternate;
        // update unvisited queue
        unvisitedPQ.push(make_pair(i, distances[i]));
        // predV insert neighbor's previous is current vertex
        predV[i] = curV.first;
      }
    }
  }
  return visited;
}

// prioritize2
//
// another prioritze class, with strings and ints
// instead of long long and double
class prioritize2 {
    public:
      bool operator()(const pair<string, int>& p1,
  const pair<string, int>& p2) const {
        return p1.second > p2.second;
      }
};

// creative
//
// takes in map of Nodes, vector of Footways, vector of Buildings
// and a graph
// Takes in a bunch of building names and times
// the classes occur in and at
// and outputs the shortest route from each class to the next
// in order of when they occur
// assumes that no classes overlap and user inputs different times
void creative(map<long long, Coordinates>& Nodes,
vector<FootwayInfo>& Footways,
vector<BuildingInfo>& Buildings,
graph<long long, double> G) {
  string yesNo, input;
  priority_queue<pair<string, int>,
  vector<pair<string, int>>,
  prioritize2> buildingsPQ;
  
  cout << endl;
  cout << "Do You want shortest path from each of your classes";
  cout << "(enter Y for yes and N for no):" << endl;
  getline(cin, yesNo);

  // loop for if multiple schedule's need to be entered
  while (yesNo != "N") {
    cout << "Enter the name of the building, followed by a space ";
    cout << "(partial name or abbreviation) and ";
    cout << "the time the class occurs ";
    cout << "(military style, eg. 0800 for 8:00am) ";
    cout << "then enter # to stop inputting classes ";
    cout << "(input classes 1 by 1):" << endl;
    int count = 1;
    int count2 = 1;
    cout << "Name and time for class ('#' to stop) " << count << ": ";
    // get name of building and time for first class
    getline(cin, input);
    int x;
    // loop until "#"
    while (input != "#") {
      // find " " to separate building name and time for the class
      size_t pos = input.find(" ");
      string Bname = input.substr(0, pos);
      input = input.substr(pos + 1, input.size() - 1);
      x = stoi(input);
      // insert building name and time in priority queue
      buildingsPQ.push(make_pair(Bname, x));
      count++;
      cout << "Name and time for class ('#' to stop) " << count << ": ";
      getline(cin, input);
    }
    BuildingInfo class1, class2, DestinationB;
    bool class1Found = false;
    bool class2Found = false;
    // stringstream for output
    stringstream pathway;
    // while priority queue is not empty
    while (!buildingsPQ.empty()) {
      // first building name is top of queue (first class according to time)
      string Name1 = buildingsPQ.top().first;
      for (auto i : Buildings) {
        // checks if name exists in Buildings vector
        if (i.Abbrev == buildingsPQ.top().first) {
          class1 = i;
          // class name found is true
          class1Found = true;
        }
      }
      // if Building name entered is not an abbreviation
      // then check for fullname in Buildings vector
      if (class1Found == false) {
        for (auto i : Buildings) {
          if (i.Fullname.find(buildingsPQ.top().first) != string::npos) {
            class1 = i;
            // class name found is ture
            class1Found = true;
          }
        }
      }
      // remove that class name and time from queue
      buildingsPQ.pop();
      // if priority queue is empty then break
      if (buildingsPQ.empty()) {
        break;
      }
      // first class is found, check next class's abbreviation
      if (class1Found == true) {
        for (auto i : Buildings) {
          if (i.Abbrev == buildingsPQ.top().first) {
            class2 = i;
            // second class found is true
            class2Found = true;
          }
        }
      }
      // if first class found but not the next one, check fullname
      if (class1Found == true && class2Found == false) {
        for (auto i : Buildings) {
          if (i.Fullname.find(buildingsPQ.top().first) != string::npos) {
            class2 = i;
            // second class found is true
            class2Found = true;
          }
        }
      }
      // if class 1 not found, ask for another input
      if (class1Found == false) {
        cout << Name1 << " is not a building." << endl;
        cout << "Reenter the name of the building and the time: ";
        getline(cin, input);
        size_t pos = input.find(" ");
        string Bname = input.substr(0, pos);
        input = input.substr(pos + 1, input.size() - 1);
        x = stoi(input);
        buildingsPQ.push(make_pair(Bname, x));
      } else if (class2Found == false) {
        // if class 2 not found, ask for another input
        cout << buildingsPQ.top().first << " is not a building." << endl;
        cout << "Reenter the name of the building and the time: ";
        getline(cin, input);
        size_t pos = input.find(" ");
        string Bname = input.substr(0, pos);
        input = input.substr(pos + 1, input.size() - 1);
        x = stoi(input);
        buildingsPQ.push(make_pair(Bname, x));
      }
      // iff both classes found...
      if (class1Found == true && class2Found == true) {
        pair<double, double> nearest;
        double c1Dist, c2Dist;
        map<double, long long> c1Distances, c2Distances;
        for (auto i : Footways) {
          for (auto j : i.Nodes) {
            nearest.first = Nodes[j].Lat;
            nearest.second = Nodes[j].Lon;
            // distance of each node to classes nodes
            c1Dist = distBetween2Points(class1.Coords.Lat, class1.Coords.Lon,
              nearest.first, nearest.second);
            c1Distances[c1Dist] = j;
            c2Dist = distBetween2Points(class2.Coords.Lat, class2.Coords.Lon,
              nearest.first, nearest.second);
            c2Distances[c2Dist] = j;
          }
        }
        long long class1ID, class2ID;
        map<long long, double> distances;
        vector<long long> visited;
        map<long long, double> predV;
        stack<long long> path;
        // ID is first node's ID
        class1ID = c1Distances.begin()->second;
        class2ID = c2Distances.begin()->second;
        // run dijkstra's algorithm
        visited = Dijkstra(G, class1ID, distances, predV);
        // if class is reachable
        if (distances[class2ID] != INF) {
          pathway << "Distance from class " << count2 << " to class ";
          pathway << count2 + 1 << ": "<< distances[class2ID];
          pathway << " miles" << endl;
          pathway << "Path: ";
          long long x = class2ID;
          // backwards trace from destination ID to beginning ID
          while (x != class1ID){
              path.push(x);
              x = predV.at(x);
          }
          pathway << class1ID;
          // while path not empty, insert path into stringstream
          while (!path.empty()) {
            long long pathNodes = path.top();
            pathway << "->";
            pathway << pathNodes;
            path.pop();
          }
          pathway << endl;
        } else {
          // if class is not reachable output unreachable
          pathway << "Sorry, path from class " << count2 << " to class ";
          pathway << count2 + 1 << " doesn't exist :'(" << endl;
        }
      }
      count2++;
    }
    // output the pathway for the schedule
    cout << endl << pathway.str() << endl;
    cout << "Do You want shortest path from each of your classes";
    cout << "based on schedule (enter Y for yes and N for no):" << endl;
    // if another schedule need's to be inputted...
    getline(cin, yesNo);
  }
}

// application
//
// takes in map of Nodes, vector of Footways, vector of Buildings
// and a graph
// Takes in building name input from 2 people
// then outputs the shortest path for each person
// to the closest building in the middle of both people
void application(map<long long, Coordinates>& Nodes,
vector<FootwayInfo>& Footways,
vector<BuildingInfo>& Buildings,
graph<long long, double> G) {
  string person1Building, person2Building;

  cout << endl;
  cout << "Enter person 1's building (partial name or abbreviation), or #> ";
  getline(cin, person1Building);

  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);

    // similar to creative()
    BuildingInfo building1, building2, DestinationB;
    bool building1Found = false;
    bool building2Found = false;

    for (auto i : Buildings) {
      if (i.Abbrev == person1Building) {
        building1 = i;
        building1Found = true;
      }
    }
    if (building1Found == false) {
      for (auto i : Buildings) {
        if (i.Fullname.find(person1Building) != string::npos) {
          building1 = i;
          building1Found = true;
        }
      }
    }
    if (building1Found == true) {
      for (auto i : Buildings) {
        if (i.Abbrev == person2Building) {
          building2 = i;
          building2Found = true;
        }
      }
    }
    if (building1Found == true && building2Found == false) {
      for (auto i : Buildings) {
        if (i.Fullname.find(person2Building) != string::npos) {
          building2 = i;
          building2Found = true;
        }
      }
    }
    if (building1Found == false) {
      cout << "Person 1's building not found" << endl;
    } else if (building2Found == false) {
      cout << "Person 2's building not found" << endl;
    }
    if (building1Found == true && building2Found == true) {
      cout << endl;
      // get the midpoint between the two buildings
      Coordinates midpoint = centerBetween2Points(building1.Coords.Lat,
      building1.Coords.Lon, building2.Coords.Lat,
      building2.Coords.Lon);

      // check the building closest to the midpoint
      double min = INF;
      double MinDistFromMid;
      for (auto i : Buildings) {
        MinDistFromMid = distBetween2Points(midpoint.Lat,
          midpoint.Lon, i.Coords.Lat, i.Coords.Lon);
        if (MinDistFromMid < min) {
          min = MinDistFromMid;
          DestinationB = i;
        }
      }

      cout << "Person 1's point:" << endl;
      cout << " " << building1.Fullname << endl;
      cout << " (" << building1.Coords.Lat << ", ";
      cout << building1.Coords.Lon << ")" << endl;

      cout << "Person 2's point:" << endl;
      cout << " " << building2.Fullname << endl;
      cout << " (" << building2.Coords.Lat << ", ";
      cout << building2.Coords.Lon << ")" << endl;

      cout << "Destination Building:" << endl;
      cout << " " << DestinationB.Fullname << endl;
      cout << " (" << DestinationB.Coords.Lat << ", ";
      cout << DestinationB.Coords.Lon << ")" << endl;
      cout << endl;

      // similar to creative()
      pair<double, double> nearest;
      map<double, long long> p1Dist, p2Dist, destDist;
      double p1_nearest_dist, p2_nearest_dist, dest_nearest_dist;
      for (auto i : Footways) {
        for (auto j : i.Nodes) {
          nearest.first = Nodes[j].Lat;
          nearest.second = Nodes[j].Lon;
          p1_nearest_dist = distBetween2Points(building1.Coords.Lat,
            building1.Coords.Lon, nearest.first, nearest.second);
          p1Dist[p1_nearest_dist] = j;
          p2_nearest_dist = distBetween2Points(building2.Coords.Lat,
            building2.Coords.Lon, nearest.first, nearest.second);
          p2Dist[p2_nearest_dist] = j;
          dest_nearest_dist = distBetween2Points(DestinationB.Coords.Lat,
            DestinationB.Coords.Lon, nearest.first, nearest.second);
          destDist[dest_nearest_dist] = j;
        }
      }
      map<long long, double> distances1, distances2, predV1, predV2;
      vector<long long> p1visited, p2visited;
      stack<long long> path1, path2;
      long long p1ID = p1Dist.begin()->second;
      long long p2ID = p2Dist.begin()->second;
      long long destID = destDist.begin()->second;
      pair<double, double> p1Nearest, p2Nearest, destNearest;
      p1Nearest.first = Nodes[p1ID].Lat;
      p1Nearest.second = Nodes[p1ID].Lon;
      p2Nearest.first = Nodes[p2ID].Lat;
      p2Nearest.second = Nodes[p2ID].Lon;
      destNearest.first = Nodes[destID].Lat;
      destNearest.second = Nodes[destID].Lon;
      cout << "Nearest P1 node:" << endl;
      cout << " " << p1ID << endl;
      cout << " (" << p1Nearest.first
        << ", " << p1Nearest.second << ")" << endl;
      cout << "Nearest P2 node:" << endl;
      cout << " " << p2ID << endl;
      cout << " (" << p2Nearest.first
        << ", " << p2Nearest.second << ")" << endl;
      cout << "Nearest destination node:" << endl;
      cout << " " << destID << endl;
      cout << " (" << destNearest.first
        << ", " << destNearest.second << ")" << endl;
      cout << endl;

      // run dijkstra's algorithm on both buildings
      p1visited = Dijkstra(G, p1ID, distances1, predV1);
      p2visited = Dijkstra(G, p2ID, distances2, predV2);
      // if building 1 cannot reach building 2, output unreachable
      if (distances1[p2ID] >= INF) {
        cout << "Sorry, destination unreachable." << endl;
      } else if (distances1[destID] >= INF || distances2[destID] >= INF) {
        // if 1 of the buildings is unreachable
        // to the building closest to midpoint
        cout << "At least one person was unable ";
        cout << "to reach the destination building. ";
        cout << "Finding next closest building..." << endl;
        // set to keep track of building names already checked
        set<string> unreachableB;
        unreachableB.insert(DestinationB.Fullname);
        // infinite loop
        while (true) {
          min = INF;
          // check next closest building to the previous one
          for (auto i : Buildings) {
            MinDistFromMid = distBetween2Points(midpoint.Lat,
              midpoint.Lon, i.Coords.Lat, i.Coords.Lon);
            if (MinDistFromMid < min) {
              if (unreachableB.count(i.Fullname) == 0) {
                min = MinDistFromMid;
                DestinationB = i;
              }
            }
          }
          map<double, long long> newDestDist;
          for (auto i : Footways) {
            for (auto j : i.Nodes) {
              nearest.first = Nodes[j].Lat;
              nearest.second = Nodes[j].Lon;
              dest_nearest_dist = distBetween2Points(DestinationB.Coords.Lat,
                DestinationB.Coords.Lon, nearest.first, nearest.second);
              newDestDist[dest_nearest_dist] = j;
            }
          }
          destID = newDestDist.begin()->second;
          destNearest.first = Nodes[destID].Lat;
          destNearest.second = Nodes[destID].Lon;
          cout << endl;
          cout << "New destination building:" << endl;
          cout << " " << DestinationB.Fullname << endl;
          cout << " (" << DestinationB.Coords.Lat << ", ";
          cout << DestinationB.Coords.Lon << ")" << endl;
          cout << "Nearest destination node:" << endl;
          cout << " " << destID << endl;
          cout << " (" << destNearest.first
            << ", " << destNearest.second << ")" << endl;
          cout << endl;
          // if new middle building is reachable by both buildings
          // break loop
          if (distances1[destID] != INF && distances2[destID] != INF) {
            break;
          } else {
            // else insert this building to unreachable buildings names set
            unreachableB.insert(DestinationB.Fullname);
            cout << "At least one person was unable ";
            cout << "to reach the destination building. ";
            cout << "Finding next closest building..." << endl;
          }
        }
      }
      // similar to creative()
      if (distances1[destID] != INF) {
        cout << "Person 1's distance to dest: ";
        cout << distances1[destID] << " miles" << endl;
        cout << "Path: ";
        long long i = destID;
        while (i != p1ID) {
          path1.push(i);
          i = predV1.at(i);
        }
        cout << p1ID;
        while (!path1.empty()) {
          cout << "->" << path1.top();
          path1.pop();
        }
        cout << endl;
      }
      cout << endl;
      // similar to creative()
      if (distances2[destID] != INF) {
        cout << "Person 2's distance to dest: ";
        cout << distances2[destID] << " miles" << endl;
        cout << "Path: ";
        long long i = destID;
        while (i != p2ID) {
          path2.push(i);
          i = predV2.at(i);
        }
        cout << p2ID;
        while (!path2.empty()) {
          cout << "->" << path2.top();
          path2.pop();
        }
        cout << endl;
      }
    }
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    // get user input again to keep loop going
    getline(cin, person1Building);
  }
}

// main
int main() {
  // maps a Node ID to it's coordinates (lat, lon)
  map<long long, Coordinates>  Nodes;
  // info about each footway, in no particular order
  vector<FootwayInfo>          Footways;
  // info about each building, in no particular order
  vector<BuildingInfo>         Buildings;
  XMLDocument                  xmldoc;

  cout << "** Navigating UIC open street map **" << endl;
  cout << endl;
  cout << std::setprecision(8);

  string def_filename = "map.osm";
  string filename;

  cout << "Enter map filename> ";
  getline(cin, filename);

  if (filename == "") {
    filename = def_filename;
  }

  //
  // Load XML-based map file
  //
  if (!LoadOpenStreetMap(filename, xmldoc)) {
    cout << "**Error: unable to load open street map." << endl;
    cout << endl;
    return 0;
  }

  //
  // Read the nodes, which are the various known positions on the map:
  //
  int nodeCount = ReadMapNodes(xmldoc, Nodes);

  //
  // Read the footways, which are the walking paths:
  //
  int footwayCount = ReadFootways(xmldoc, Footways);

  //
  // Read the university buildings:
  //
  int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);

  //
  // Stats
  //
  assert(nodeCount == (int)Nodes.size());
  assert(footwayCount == (int)Footways.size());
  assert(buildingCount == (int)Buildings.size());

  cout << endl;
  cout << "# of nodes: " << Nodes.size() << endl;
  cout << "# of footways: " << Footways.size() << endl;
  cout << "# of buildings: " << Buildings.size() << endl;

  graph<long long, double> G;

  // add vertex from Nodes to graph
  for (auto &e : Nodes) {
    G.addVertex(e.first);
  }

  double distance;
  pair<double, double> c1, c2;

  // add edges for each vertex
  for (auto i : Footways) {
    for (unsigned int j = 0; j < i.Nodes.size() - 1; j++) {
      c1.first = Nodes[i.Nodes[j]].Lat;
      c2.first = Nodes[i.Nodes[j+1]].Lat;
      c1.second = Nodes[i.Nodes[j]].Lon;
      c2.second = Nodes[i.Nodes[j+1]].Lon;
      distance = distBetween2Points(c1.first, c1.second, c2.first, c2.second);
      G.addEdge(i.Nodes[j], i.Nodes[j+1], distance);
      G.addEdge(i.Nodes[j+1], i.Nodes[j], distance);
    }
  }

  cout << "# of vertices: " << G.NumVertices() << endl;
  cout << "# of edges: " << G.NumEdges() << endl;
  cout << endl;

  //
  // Menu
  //
  string userInput;
  cout << "Enter \"a\" for the standard application or "
        << "\"c\" for the creative component application> ";
  getline(cin, userInput);
  if (userInput == "a") {
    application(Nodes, Footways, Buildings, G);
  } else if (userInput == "c") {
    creative(Nodes, Footways, Buildings, G);
  }
  //
  // done:
  //
  cout << "** Done **" << endl;
  return 0;
}
