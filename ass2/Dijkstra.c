// Dijkstra API implementation
// COMP2521 Assignment 2
//Fion Wells z53549092

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"


PredNode *insertNode(NodeData n, int U, int dist);
PredNode *newPred(int U);
PredNode *updateList(NodeData n, int U);

//Initialise new array with dist values of infinity and pred empty preds
//Add all these nodes to the new PQ
//Go through the PQ, for each item in PQ finding shortest distances and updating PQ
NodeData *dijkstra(Graph g, Vertex src) {
	
	int V = GraphNumVertices(g);
	PQ pq = PQNew();
	
	NodeData *new = malloc(V * sizeof(*new));
	
	for (int i = 0; i < V; i++) {
		new[i].dist = INFINITY;
		new[i].pred = NULL;
		if (i != src) {
			PQInsert(pq, i, new[i].dist);
		}
	}
	
	new[src].dist = 0;
	PQInsert(pq, src, 0);
	
	while (!PQIsEmpty(pq)) {

		int U = PQDequeue(pq);
		AdjList l = GraphOutIncident(g, U);
		while (l != NULL) {
			int tmp_dist = new[U].dist + l->weight;
			if (new[U].dist == INFINITY) {
				break;
			}
			else if (tmp_dist < new[l->v].dist) {
				new[l->v].dist = tmp_dist;
				PQUpdate(pq, l->v, tmp_dist);
				new[l->v].pred = updateList(new[l->v], U);
			} 
			else if (tmp_dist == new[l->v].dist) {
				new[l->v].pred = insertNode(new[l->v], U, tmp_dist);
			}
			l = l->next;
		}

	}
	PQFree(pq);
	
	return new;
}

//Cycles through each item
//Create tmp variable pointing to head of linked list, cycle though the list with tmp trailing, free tmp after each iteration
//after freeing all the pred lists, free data itself
void freeNodeData(NodeData *data, int nV) {

	for (int i = 0; i < nV; i++) {
		PredNode *tmp;
		while (data[i].pred != NULL) {
			tmp = data[i].pred;
			data[i].pred = data[i].pred->next;
			free(tmp);
		}
	}
	free(data);

}

//Helper function to initialise a new prednode
PredNode *newPred(int U) {

	PredNode *p = malloc(sizeof(PredNode));
	p->v = U;
	p->next = NULL;

	return p;
}


//Function to insert a prednode into the list in order
//Checks for if newpred is to be inserted first
//Otherwise loop though list till insertion point and insert
PredNode *insertNode(NodeData n, int U, int dist) {

	PredNode *new =  newPred(U);
	if (n.pred->v > U) {
	new->next = n.pred;
	n.pred = new;		
	}
	else {
		PredNode *curr = n.pred;
		while (curr->next != NULL && U > n.pred->v) {
			curr = curr->next;
		}
		new->next = curr->next;
		curr->next = new;
	}

	return n.pred;
}


PredNode *updateList(NodeData n, int U) {

	PredNode *curr = n.pred;
	PredNode *next;
	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}
	
	return newPred(U);
}
	
	