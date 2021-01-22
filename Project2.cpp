#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

/*
    Author: Katriana O'Hara
    References : Utilized lecture slides, Stepik implementations, Slack posts from other students, 
    conceptual discussion with another student, and some websites (referenced material has a comment 
    either within the function code or before the function to indicate where resources were utilized)
 */

class Graph {
private:
    int urlNum = 1;
    vector<string> urlNames;
    map<string, int> urls;
    map<int, vector<pair<int, int>>> inGraph;
    map<int, vector<pair<int, int>>> outGraph;
    
public:
    void insertEdge(string from, string to, int weight);
    void printGraph(vector<double> r);
    void pageRank(int iterations);
};

// Used implementation from Stepik 7.1.2 with some minor changes
void Graph::insertEdge(string from, string to, int weight) {
    // Reused previous reference to http://www.cplusplus.com/reference/algorithm/find/ for figuring out how to deal with dulpicate url names in my vector
    // Push back URL names into a vector to use later when printing out the page ranks
    auto iter = find(urlNames.begin(), urlNames.end(), from);
    if (iter == urlNames.end()) {
        urlNames.push_back(from);
    }
    iter = find(urlNames.begin(), urlNames.end(), to);
    if (iter == urlNames.end()) {
        urlNames.push_back(to);
    }
    // Use urls map to store the names of the URLs and assign them numbers
    if (urls.find(from) == urls.end())
        urls[from] = urlNum++;
    if (urls.find(to) == urls.end())
        urls[to] = urlNum++;

    // Use the urls map to create the adjacency list with regards to the in degrees
    inGraph[urls[to]].push_back(make_pair(urls[from], weight));
    if (inGraph.find(urls[from]) == inGraph.end()) {
        inGraph[urls[from]] = {};
    }

    // Use the urls map to create the adjacency list with regards to the out degrees
    outGraph[urls[from]].push_back(make_pair(urls[to], weight));
    if (outGraph.find(urls[to]) == outGraph.end()) {
        outGraph[urls[to]] = {};
    }
}

// Referenced http://www.cplusplus.com/reference/iomanip/setprecision/ to learn how to round page rank to only 2 decimal places
// Based on my printGraph implementation from Stepik 7.1.2 
void Graph::printGraph(vector<double> r) {
    sort(urlNames.begin(), urlNames.begin() + urlNames.size());
    for (int i = 0; i < urlNames.size(); i++) {
        cout << urlNames.at(i) << " " << fixed << setprecision(2) << r.at(urls[urlNames.at(i)] - 1) << endl;
    }
 }

// This function was discussed conceptually with Yelaine Linares. We drew out the different graphs and discussed how the "matrix" multiplication would work.
void Graph::pageRank(int iterations) {
    vector<double> temp;
    vector<double> r;

    // First populating temp rank vector with best guess : 1 / |V|
    // Populating rank vector with zeros
    for (int i = 0; i < urlNum - 1; i++) {
        temp.push_back((1.0 / ((double)urlNum - 1)));
        r.push_back((1.0 / ((double)urlNum - 1)));
    }

    // Calculate the rank for iterations - 1 iterations by doing matrix multiplication
    for (int i = 0; i < iterations - 1; i++) {
        for (int j = 1; j <= urlNum - 1; j++) {
            double sum = 0;
            for (int k = 0; k < inGraph[j].size(); k++) {
                // First use outdegree version of adjacency list to get the size of the vector which represents the outdegree of that element
                double outDegree = 1.0 / (outGraph[inGraph[j].at(k).first].size());
                // Next multiply the computed outdegree by the first number in the pair of the inGraph[j].at(k) element of the temp rank vector
                // This allows for multiplying only the corresponding elements in the adjacency list instead of the zeros and the element like 
                // you normally would in an adjacency matrix
                sum += (outDegree * temp.at((inGraph[j].at(k).first) - 1));
            }
            // Replace the j-1th element of the resultant rank vector with the sum from the "matrix" multiplication
            r.at(j - 1) = sum;
        }
        // Set the rank vector (r) equal to the temp vector at the end of the iteration so it can be used for the next iteration
        temp = r;
    }
    printGraph(r);
}

int main() {
    int noOfLines, powerIterations;
    string from, to;
    Graph adjacencyList;
    cin >> noOfLines;
    cin >> powerIterations;
    for (int i = 0; i < noOfLines; i++) {
        cin >> from;
        cin >> to;
        adjacencyList.insertEdge(from, to, 0);
    }
    adjacencyList.pageRank(powerIterations);
}