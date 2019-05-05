// STL implementation of Prim's algorithm for MST
#include "Graph.h"
#include <fstream>
#include <vector>
#include <string>

using namespace std;

typedef vector<vector<int>> TwoDiVec;

void print2DVector(TwoDiVec, string); //takes a 2 dimensional vector and prints it
TwoDiVec createMSTGraph(vector<int>, TwoDiVec); //takes an mst vector and its original graph and returns an MST graph

int main()
{
	int V = 0, edgeCount = 0;
		
	ifstream input;	
	input.open("GraphA.txt");
	
	input >> V;//first thing in txt should be size of graph
	TwoDiVec myGraph(V, vector<int>(V,0));//Create 2d vector of all 0's
	Graph g(V); //new graph of size V, undirected
	for (int i = 0; i < V; i++) {	//can't use range based for loop because I need row and column itself	
		for (int j = 0; j < V; j++) {
			input >> myGraph[i][j];	//get weight from file				
			if (myGraph[i][j] > 0) {
				g.addEdge(i, j, myGraph[i][j]); //only add edge if its weight is nonzero
				edgeCount++; //count the number of nonzero edges
			}
		}		
	}	

	cout << "\nNumber of Vertexes: " << V << endl;
	cout << "Number of Edges: " << edgeCount/2 << endl << endl; //since this is an undirected graph, divide by 2

	print2DVector(myGraph, "Original.txt");//Print the original Graph

	print2DVector(createMSTGraph(g.MST(0), myGraph), "MSTat0.txt");		//Print the MST graph created from the mst vector

	print2DVector(createMSTGraph(g.MST(4) , myGraph), "MSTat4.txt");

	print2DVector(createMSTGraph(g.MST(2), myGraph), "MSTat2.txt");

	system("pause"); 
	return 0;
}

void print2DVector(TwoDiVec my2DVector, string name) {
	ofstream output;
	output.open(name);
	output << 5 << endl;
	
	for (auto i : my2DVector) {
		for (auto j : i) {
			output << j << " ";
			cout << j << " ";
		}
		output << endl;
		cout << endl;
	}
	cout << endl;
	output.close();
}

TwoDiVec createMSTGraph(vector<int> source, TwoDiVec original) {
	TwoDiVec newGraph(original.size(), vector<int>(original.size(), 0));//Create 2d vector of all 0's
	for (int i = 0; i < 6; i++) {
		if (source[i] == -1) { //mst vector will have -1 for its source node, cant have that!
			source[i] = 0;
		}		
		newGraph[i][source[i]] = original[i][source[i]]; //do this twice because its undirected graph
		newGraph[source[i]][i] = original[source[i]][i];		
	}
	return newGraph;
}