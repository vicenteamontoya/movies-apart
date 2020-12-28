
#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

class ActorGraph {
  public:
    class Actor;
    class Movie;

    class Actor {
      public:
        string name;
        bool visited;
        vector<Movie*> movies;
        Movie* previousMovie;

        Actor(string Name) {
            name = Name;
            visited = false;
            previousMovie = NULL;
        }
        ~Actor(){};
    };

    class Movie {
      public:
        string name;
        bool visited;
        vector<Actor*> actors;
        Actor* previousActor;
        Movie(string Name) {
            name = Name;
            visited = false;
            previousActor = NULL;
        }
        ~Movie() {}
    };

  public:
    unordered_map<string, Actor*> nodes;
    unordered_map<string, Movie*> movieList;

    ActorGraph();

    bool buildGraph(istream& is);

    void BFS(const string& fromActor, const string& toActor,
             string& shortestPath);

    ~ActorGraph();
};

#endif  // ACTORGRAPH_HPP