// Lance-Williams Algorithm for Hierarchical Agglomerative Clustering
// COMP2521 Assignment 2

#include <assert.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "LanceWilliamsHAC.h"

#define INFINITY DBL_MAX

/**
 * Generates  a Dendrogram using the Lance-Williams algorithm (discussed
 * in the spec) for the given graph  g  and  the  specified  method  for
 * agglomerative  clustering. The method can be either SINGLE_LINKAGE or
 * COMPLETE_LINKAGE (you only need to implement these two methods).
 * 
 * The function returns a 'Dendrogram' structure.
 */

//Global variables for array dimensions
int ROWS;
int COLS;

Dendrogram newMerge(Dendrogram left, Dendrogram right);
Dendrogram newNode(int x);
void getmin(int nV, double arr[nV][nV], int low[2]);
void newArray(int nV, double arr[nV][nV], int del_row, int del_col);
void updateArr(int nV, double arr[nV][nV], double merge[COLS]);
void del_cols(int nV, int del, double arr[nV][nV]);
void del_rows(int nV, int del, double arr[nV][nV]);
void merge(int nV, double arr[nV][nV], double merge[nV], int x, int y);
void merge_high(int nV, double arr[nV][nV], double merge[nV], int one, int two);
void updatedend(int nV, Dendrogram dend[nV], int x, int y, Dendrogram new);


Dendrogram LanceWilliamsHAC(Graph g, int method) {
	

	if (method == 1) {

	int nV = GraphNumVertices(g);
	double arr[nV][nV];
	for (int i = 0; i < nV; i++) {
		for (int x = 0; x < nV; x++) {
			if (i == x) {
				arr[i][x] = 0;
			}
			else {
				arr[i][x] = DBL_MAX;
			}
		}
	}
	
	for (int i = 0; i < nV; i++) {
		AdjList out = GraphOutIncident(g, i);

		while (out != NULL) {
			if (arr[i][out->v] < out->weight || arr[i][out->v] == DBL_MAX) {
				arr[i][out->v] = (double) 1/out->weight;
				arr[out->v][i] = (double) 1/out->weight;
			}
			out = out->next;
		}
	}
	
	Dendrogram dend[nV];
	for (int i = 0; i < nV; i++) {
		dend[i] = newNode(i);
	} 
	
	ROWS = nV;
	COLS = nV;

	for (int i = 0; i < nV-1; i++) {
	int low[2];
	getmin(nV, arr, low);
	double new[nV];
	merge(nV, arr, new, low[0], low[1]);
	newArray(nV, arr, low[0], low[1]);
	ROWS++;
	COLS++;
	updateArr(nV, arr, new);
	Dendrogram dendo = newMerge(dend[low[0]], dend[low[1]]);
	updatedend(nV-i, dend, low[0], low[1], dendo);
	} 

	return dend[0];
	
	}
	else if (method == 2) {

	int nV = GraphNumVertices(g);
	double arr[nV][nV];
	for (int i = 0; i < nV; i++) {
		for (int x = 0; x < nV; x++) {
			if (i == x) {
				arr[i][x] = 0;
			}
			else {
				arr[i][x] = DBL_MAX;
			}
		}
	}
	
	for (int i = 0; i < nV; i++) {
		AdjList out = GraphOutIncident(g, i);

		while (out != NULL) {
			if (arr[i][out->v] < out->weight || arr[i][out->v] == DBL_MAX) {
				arr[i][out->v] = (double) 1/out->weight;
				arr[out->v][i] = (double) 1/out->weight;
			}
			out = out->next;
		}
	}
	
	Dendrogram dend[nV];
	for (int i = 0; i < nV; i++) {
		dend[i] = newNode(i);
	} 
	
	ROWS = nV;
	COLS = nV;

	for (int i = 0; i < nV-1; i++) {
	int high[2];
	getmin(nV, arr, high);
	double new[nV];
	merge_high(nV, arr, new, high[0], high[1]);
	newArray(nV, arr, high[0], high[1]);
	ROWS++;
	COLS++;
	updateArr(nV, arr, new);
	Dendrogram dendo = newMerge(dend[high[0]], dend[high[1]]);
	updatedend(nV-i, dend, high[0], high[1], dendo);
	} 

	return dend[0];

	}

	return NULL;

}

//Function to remove two clusters from the dend array and insert the new cluster formed from them
void updatedend(int nV, Dendrogram dend[nV], int x, int y, Dendrogram new) {

	int min;
	int max;
	if (x < y)  {
		min = x;
		max = y;
	}
	else {
		min = y;
		max = x;
	}

	for (int i = max; i < nV; i++) {
		dend[i] = dend[i+1];
	}
	nV--;
	for (int i = min; i < nV; i++) {
		dend[i] = dend[i+1];
	}
	nV--;

	dend[nV] = new;
	nV++;

}

//Function that adds the newly created line to the bottom col and row in the array
void updateArr(int nV, double arr[nV][nV], double merge[COLS]) {


	for (int i = 0; i < COLS; i++) {
		arr[i][COLS-1] = merge[i];
	}
		
	for (int i = 0; i < ROWS; i++) {
		arr[ROWS-1][i] = merge[i];
	}

}

//Function that takes both lines to be merged together in a cluster and combines them for single linkage
//Ignore the items that are being removed, choose the minimum of the two to go into new line
void merge(int nV, double arr[nV][nV], double merge[nV], int one, int two) {

	
	int x = 0;
	int c = 0;
	while (x < nV) {
		if (x != one && x != two) {
			if (arr[one][x] < arr[two][x]) {
				merge[c] = arr[one][x];
				c++;
			}
			else { 
				merge[c] = arr[two][x];
				c++;
			}
		}
		x++;
	}
	
	merge[ROWS-2] = DBL_MAX;
	
}

//Function that takes both lines to be merged together in a cluster and combines them for complete linkage
//Ignore the items that are being removed, choose the maximum of the two to go into new line
void merge_high(int nV, double arr[nV][nV], double merge[nV], int one, int two) {

	int x = 0;
	int c = 0;
	while (x < nV) {
		if (x != one && x != two) {
			if (arr[one][x] > arr[two][x]) {
				merge[c] = arr[one][x];
				c++;
			}
			else {
				merge[c] = arr[two][x];
				c++;
			}
		}
		x++;
	}
			
	merge[ROWS-2] = 0;
}

//Function to get the minimum value in the array
void getmin(int nV, double arr[nV][nV], int low[2]) {

	double min = arr[0][0];
	int count = 1;
	for (int i = 0; i < ROWS; i++) {
		for (int x = 0; x < count; x++) {
			if ((arr[i][x] < min || min == 0) && arr[i][x] != 0) {
				min = arr[i][x];
				low[0] = i;
				low[1] = x;
			}
		}
		count++;
	
	}
}


//Function just calls other function to delete cols and rows from the array
void newArray(int nV, double arr[nV][nV], int del_row, int del_col) {
	
	del_cols(nV, del_row, arr);
	del_rows(nV, del_row, arr);
	del_cols(nV, del_col, arr);
	del_rows(nV, del_col, arr);

}

//Shifts the cols left at deletion point
void del_cols(int nV, int del, double arr[nV][nV]) {

	COLS--;
	int tmp = del;
	for (int i = 0; i < ROWS; i++) {
		tmp = del;
		while (tmp < COLS) {
			arr[i][tmp] = arr[i][tmp+1];
			tmp++;
		}

	}
}

//Shifts rows up at deletion point
void del_rows(int nV, int del, double arr[nV][nV]) {

	ROWS--;
		for (int x = 0; x < COLS; x++) {
			arr[del][x] = arr[del+1][x];
		}
		
	
}

//Creates a new dendnode with no vertex value and clusters being merged
Dendrogram newMerge(Dendrogram left, Dendrogram right) {

	Dendrogram d = malloc(sizeof(*d));
	d->left = left;
	d->right = right;

	return d;
}

//Helper to create new dendnode
Dendrogram newNode(int x) {
	
	Dendrogram d = malloc(sizeof(*d));
	d->left = NULL;
	d->right = NULL;
	d->vertex = x;

	return d;

}

