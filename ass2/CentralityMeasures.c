// Centrality Measures API implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "Graph.h"

void getbetween(double *between, NodeData d, NodeData *n, int src, int nV);

double *closenessCentrality(Graph g) {
	
	int nV = GraphNumVertices(g);
	double *res = malloc(nV * sizeof(double));
	for (int i = 0; i < nV; i++) {
		NodeData *d = dijkstra(g, i);
		int close = 0;
		int r = 0;
		for (int x = 0; x < nV; x++) {
			if (d[x].dist != INFINITY) {
				close += d[x].dist;
				r++;
			}
		}
		double n_1 = r-1;
		double N_1 = nV-1;
		if (close > 0) {
			res[i] = (n_1/N_1) * (n_1/close);
		}

	freeNodeData(d, nV);
	}
	return res;
}


double *betweennessCentrality(Graph g) {
	
	int nV = GraphNumVertices(g);
	double *between = malloc(nV * sizeof(double));
	for (int i = 0; i < nV; i++) {
		between[i] = 0;
	}

	for (int i = 0; i < nV; i++) {
		NodeData *d = dijkstra(g, i);
		
		for (int x = 0; x < nV; x++) {
			if (d[x].dist != INFINITY && d[x].pred != NULL && d[x].dist != 0) {
				getbetween(between, d[x], d, i, nV);	
			}
		}
		freeNodeData(d, nV);
	}
	
	return between;
}

void getbetween(double *between, NodeData d, NodeData *n, int src, int nV) {

	if (d.pred == NULL) {
		return;
	}

	int count = 0;
	int betw[nV];
	for (int i = 0; i < nV; i++) {
		betw[i] = 0;
	}

	for (PredNode *curr = d.pred; curr != NULL; curr = curr->next) {
		if (curr->v != src) {
			betw[curr->v] += 1;
			getbetween(between, n[d.pred->v], n, src, nV);
		}
		count++;
	}
	for (int i = 0; i < nV; i++) {
		between[i] += (double) betw[i] / count;
	}
	
	return;
}
