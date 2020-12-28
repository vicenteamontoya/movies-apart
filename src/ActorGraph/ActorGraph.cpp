#include "ActorGraph.hpp"

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

ActorGraph::ActorGraph() {}

/* Build the actor graph from dataset file.
 * Each line of the dataset file must be formatted as:
 * ActorName <tab> MovieName <tab> Year
 * Two actors are connected by an undirected edge if they have worked in a movie
 * before.
 */
bool ActorGraph::buildGraph(istream& is) {
    bool readHeader = false;

    while (is) {
        string s;
        if (!getline(is, s)) break;

        // skip the header of the file
        if (!readHeader) {
            readHeader = true;
            continue;
        }

        // read each line of the dataset to get the movie actor relation
        istringstream ss(s);
        vector<string> record;
        while (ss) {
            string str;
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }

        // if format is wrong, skip current line
        if (record.size() != 3) {
            continue;
        }

        // extract the information
        string actorName = record[0];
        string movieName = record[1] + "`" + record[2];

        Actor* actor;
        Movie* movie;

        // If actor is not in the graph
        if (nodes.find(actorName) == nodes.end()) {
            // Creating new actor and inserting
            actor = new Actor(actorName);
            nodes.insert({actorName, actor});
        } else {
            actor = nodes[actorName];
        }

        // If movie is not in the graph
        if (movieList.find(movieName) == movieList.end()) {
            movie = new Movie(movieName);
            movieList.insert({movieName, movie});
        } else {
            movie = movieList[movieName];
        }

        // Inserting actor's name in movie
        movie->actors.push_back(actor);

        // Inserting movie in actor's movies
        actor->movies.push_back(movie);
    }

    // if failed to read the file, clear the graph and return
    if (!is.eof()) {
        unordered_map<string, Actor*>::iterator nodes_itr =
            nodes.begin();  // iterate through the actor collection
        unordered_map<string, Actor*>::iterator nodes_end = nodes.end();

        unordered_map<string, Movie*>::iterator movieList_itr =
            movieList.begin();  // iterate through the movie collection
        unordered_map<string, Movie*>::iterator movieList_end = movieList.end();

        // Deleting all actors
        while (nodes_itr != nodes_end) {
            delete (*nodes_itr).second;
            nodes_itr++;
        }

        // Deleting all movies
        while (movieList_itr != movieList_end) {
            delete (*movieList_itr).second;
            movieList_itr++;
        }
        return false;
    }

    return true;
}

// Builds the output string
void returnOutput(ActorGraph::Actor* root, ActorGraph::Actor* end,
                  string& shortestPath) {
    // Adding goal actor to string
    string output = "(" + end->name + ")";
    ActorGraph::Actor* traverse = end;

    // Transversing backwards from end to root
    while (traverse != root) {
        ActorGraph::Movie* lastMovie = traverse->previousMovie;

        // Extracting title and year and adding movie to string
        int splitIndex = lastMovie->name.find('`');
        string title = lastMovie->name.substr(0, splitIndex);
        string year = lastMovie->name.substr(splitIndex + 1);
        output = "[" + title + "#@" + year + "]-->" + output;

        // Transversing backwards and adding actor to string
        traverse = lastMovie->previousActor;
        output = "(" + traverse->name + ")--" + output;
    }

    shortestPath = output;
}

// Breadth First Search
void ActorGraph::BFS(const string& fromActor, const string& toActor,
                     string& shortestPath) {
    // Checking if there's no graph
    if (nodes.empty()) {
        shortestPath = "";
        return;
    }

    unordered_map<string, Actor*>::iterator nodes_itr =
        nodes.begin();  // iterate through actor collection
    unordered_map<string, Actor*>::iterator nodes_end = nodes.end();

    unordered_map<string, Movie*>::iterator movieList_itr =
        movieList.begin();  // iterate through movie collection
    unordered_map<string, Movie*>::iterator movieList_end = movieList.end();

    // Setting all nodes to default state
    while (nodes_itr != nodes_end) {
        (*nodes_itr).second->visited = false;
        (*nodes_itr).second->previousMovie = NULL;
        nodes_itr++;
    }

    // Setting all movies to default state
    while (movieList_itr != movieList_end) {
        (*movieList_itr).second->visited = false;
        (*movieList_itr).second->previousActor = NULL;
        movieList_itr++;
    }

    vector<Actor*> cur_actors;  // Queue to do the BFS

    // Getting root to begin the BFS
    Actor* root = nodes.find(fromActor)->second;

    // If fromActor is not on the graph
    if (root == nullptr) {
        shortestPath = "";
        return;
    }
    // Adding root to the queue
    cur_actors.push_back(root);

    // Loop to iterate through the queue
    for (vector<Actor*>::iterator a_it = cur_actors.begin();
         a_it != cur_actors.end(); a_it = cur_actors.begin()) {
        // Popping head and setting node to visited
        Actor* a = *a_it;  // head of the queue
        a->visited = true;
        cur_actors.erase(a_it);

        // Looping through the edges (movies) of the node
        for (vector<Movie*>::iterator m_it = a->movies.begin();
             m_it != a->movies.end(); ++m_it) {
            Movie* m = *m_it;  // current movie

            if (!m->visited) {
                m->previousActor = a;
                // Iterating through actors of the movie
                for (vector<Actor*>::iterator a_it2 = m->actors.begin();
                     a_it2 != m->actors.end(); ++a_it2) {
                    Actor* a2 = *a_it2;  // current movie

                    // Success we found the node
                    if (a2->name == toActor) {
                        a2->previousMovie = m;
                        returnOutput(root, a2, shortestPath);
                        return;
                    }
                    // Adding the node the queue if not visited before
                    if (!a2->visited && a2->previousMovie == NULL) {
                        cur_actors.push_back(a2);
                        a2->previousMovie = m;
                    }
                }
                m->visited = true;
            }
        }
    }
}

ActorGraph::~ActorGraph() {
    unordered_map<string, Actor*>::iterator nodes_itr =
        nodes.begin();  // iterate through the actor collection
    unordered_map<string, Actor*>::iterator nodes_end = nodes.end();
    unordered_map<string, Movie*>::iterator movieList_itr =
        movieList.begin();  // iterate through the movie collection
    unordered_map<string, Movie*>::iterator movieList_end = movieList.end();

    // Deleting all actors
    while (nodes_itr != nodes_end) {
        delete (*nodes_itr).second;
        nodes_itr++;
    }

    // Deleting all movies
    while (movieList_itr != movieList_end) {
        delete (*movieList_itr).second;
        movieList_itr++;
    }
}