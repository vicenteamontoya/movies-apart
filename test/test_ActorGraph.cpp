#include <gtest/gtest.h>

#include <fstream>

#include "ActorGraph.hpp"

using namespace std;
using namespace testing;

TEST(ActorGraphTests, TwoActorsOneMovie) {
    string from = "Actor 1";
    string to = "Actor 2";
    string through = "Awesome Movie\t2020";

    stringstream ss;
    ss << "Actor/Actress	Movie	Year" << endl;
    ss << from << "\t" << through << endl;
    ss << to << "\t" << through << endl;

    ActorGraph graph;
    bool isBuilt = graph.buildGraph(ss);
    ASSERT_TRUE(isBuilt);
}

TEST(ActorGraphTests, UnformattedFile) {
    string from = "Actor 1";
    string to = "Actor 2";
    string through = "Awesome Movie\t2020";

    stringstream ss;
    ss << "Actor/Actress	Movie	Year" << endl;
    ss << from << " " << through << endl;
    ss << to << " " << through << endl;

    ActorGraph graph;
    bool isBuilt = graph.buildGraph(ss);
    ASSERT_TRUE(isBuilt);
}

TEST(ActorGraphTests, EmptyFile) {
    stringstream ss;
    string from = "Kevin Bacon";
    string to = "Robert Downey Jr.";
    string path;

    ActorGraph graph;
    bool isBuilt = graph.buildGraph(ss);
    graph.BFS(from, to, path);

    ASSERT_EQ(path, "");
}

TEST(ActorGraphTests, NonExistentFile) {
    string from = "Kevin Bacon";
    string to = "Robert Downey Jr.";
    string path;

    ActorGraph graph;

    char* graphFileName;
    ifstream graphFile(graphFileName);
    bool isBuilt = graph.buildGraph(graphFile);

    ASSERT_FALSE(isBuilt);
}

TEST(ActorGraphTests, SmallTest) {
    string from = "Kevin Bacon";
    string to = "Robert Downey Jr.";
    string path;
    string actualPath = "(Kevin Bacon)--[X-Men: First Class#@2011]-->(James ";
    actualPath = actualPath +
                 "McAvoy)--[Glass#@2019]-->(Samuel L. Jackson)--[Avengers: ";
    actualPath = actualPath + "Endgame#@2019]-->(Robert Downey Jr.)";

    stringstream ss;
    ss << "Actor/Actress	Movie	Year" << endl
       << "Kevin Bacon	X-Men: First Class	2011" << endl
       << "James McAvoy	X-Men: First Class	2011" << endl
       << "James McAvoy	X-Men: Apocalypse	2016" << endl
       << "James McAvoy	Glass	2019" << endl
       << "Michael Fassbender	X-Men: First Class	2011" << endl
       << "Michael Fassbender	X-Men: Apocalypse	2016" << endl
       << "Michael Fassbender	Alien: Covenant	2017" << endl
       << "Samuel L. Jackson	Glass	2019" << endl
       << "Samuel L. Jackson	Avengers: Endgame	2019" << endl
       << "Robert Downey Jr.	Avengers: Endgame	2019" << endl
       << "Robert Downey Jr.	Spider-Man: Homecoming	2017" << endl
       << "Tom Holland	Spider-Man: Homecoming	2017" << endl
       << "Tom Holland	The Current War	2017" << endl
       << "Katherine Waterston	Alien: Covenant	2017" << endl
       << "Katherine Waterston	The Current War	2017" << endl;

    ActorGraph graph;

    bool isBuilt = graph.buildGraph(ss);
    graph.BFS(from, to, path);

    ASSERT_EQ(path, actualPath);
}
// TODO: add more tests for actor graph
