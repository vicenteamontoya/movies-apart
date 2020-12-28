/**
 * CSE 100 PA4 Pathfinder in Actor Graph
 */
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ActorGraph.hpp"

using namespace std;

/* Print the usage of the program */
void usage(char* program_name) {
    cerr << program_name << " called with incorrect arguments." << endl;
    cerr << "Usage: " << program_name
         << " movie_cast_file actor_pairs_file shortest_paths_file" << endl;
}

/* Main program that drives the pathfinder */
int main(int argc, char* argv[]) {
    const int NUM_ARG = 4, INPUT_IDX = 1, PAIR_IDX = 2, OUT_IDX = 3,
              PAIR_SIZE = 2;

    if (argc != NUM_ARG) {
        usage(argv[0]);
        return 1;
    }

    char* graphFileName = argv[INPUT_IDX];
    char* pairs = argv[PAIR_IDX];
    char* output = argv[OUT_IDX];

    // build the actor graph from the input file
    ActorGraph* graph = new ActorGraph();
    cout << "Reading " << graphFileName << " ..." << endl;
    ifstream graphFile(graphFileName);
    if (!graph->buildGraph(graphFile)) {
        delete graph;
        cerr << "Failed to read " << graphFileName << endl;
        return 1;
    }
    graphFile.close();
    cout << "Done." << endl;

    // write the shorest path of each given pair to the output file
    ifstream infile(pairs);
    ofstream outfile(output);
    bool haveHeader = false;

    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        // skip reading the header in inFile and output the header in outFile
        if (!haveHeader) {
            outfile << "(actor)--[movie#@year]-->(actor)--..." << endl;
            haveHeader = true;
            continue;
        }

        // read the pair from each line
        istringstream ss(s);
        vector<string> actorPair;
        while (ss) {
            string str;
            if (!getline(ss, str, '\t')) break;
            actorPair.push_back(str);
        }

        // skip the incorrectly formatted line in input file
        if (actorPair.size() != PAIR_SIZE) {
            continue;
        }

        string actor1(actorPair[0]);
        string actor2(actorPair[1]);

        // output the shorest path for each line
        string shortestPath = "";
        graph->BFS(actor1, actor2, shortestPath);
        if (shortestPath.length() > 0) {
            outfile << shortestPath;
        }
        outfile << endl;
    }
    outfile.close();
    infile.close();
    delete graph;
    return 0;
}
