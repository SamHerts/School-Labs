#pragma once
#include <vector>		
#include <utility>		//for using pairs
#include <queue>		//for using priority queue
#include <functional>	//for using greater than comparison of pairs
#include <iostream>		

using std::vector;
using std::cout;
using std::endl;

typedef std::pair<int, int> Pair; 

class Graph
{
public:
	Graph(int nodeCount) {	// takes in number of nodes and creates a vector of that size, full of pairs	
		V = nodeCount;
		testGrid = vector<vector<Pair>>(V); //2d vector of pairs, size VxV
	}

	~Graph() {};

	void addEdge(int left, int right, int weight) { //vector at position left is connected to right by weight		
		testGrid[left].push_back(Pair{ right,weight });
	}
	
	vector<int> MST(int);

private:
	int V;//Number of Vertices	
	vector<vector<Pair>> testGrid; //2d vector of pairs	
};

vector<int> Graph::MST(int start)
{	
	cout << "Begin MST starting at Vertex: " << start << std::endl;	
	std::priority_queue< Pair, vector <Pair>, std::greater<Pair> > myQueue; //Queue of pairs (weight, node), so the compare works properly
	vector<int> key(V, 9999); //vector of size v, all set to a huge number
	vector<int> parent(V, -1);//Stores parent array which actually stores mst, value of parent[i] = node which i is connected to
	vector<int> otherP(V, -1);
	vector<bool> Mark(V, false);// To keep track of vertices included in MST

	myQueue.push(Pair{ 0, start });// Insert source itself in priority queue and initialize
	key[start] = 0;
	int vertex = myQueue.top().second;	
	Mark[vertex] = true;

	/* Looping till priority queue becomes empty */
	while (myQueue.empty() == false)
	{		
		for(auto i : testGrid[vertex]) //for every node attached to smallest node
		{					
			int v = i.first; //save the node
			int w = i.second;//save the weight
			
			if (Mark[v] == false && key[v] > w) // if node has not been added to MST, and there is not already a shorter path
			{				
				key[v] = w; //update node as being short path
				myQueue.push(Pair{ key[v], v }); //add node to queue with weight
				//cout << "Push to queue Node: " << v << " - " << key[v] << endl;
				parent[v] = vertex; //jump to the new node				
			}
		}	
		vertex = myQueue.top().second; //store the smallest node on the queue
										   //cout << "Pop from queue Node: " << myQueue.top().second << " - " << myQueue.top().first << endl;
		myQueue.pop(); //remove the smallest node on the queue		

		Mark[vertex] = true;  //smallestNode has been visited
	}
	
	// print edges of mst using parent array
	
	for (int i = 0; i < V; ++i) { //for all nodes
		if (parent[i] >= 0) { //as long as the node its connected to exists
			cout << "Add edge " << parent[i] << " to " << i << std::endl; //cout the node and its connecting node
		}
	}
	cout << std::endl;
	return parent;
}