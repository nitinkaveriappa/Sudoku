#include <iostream>
#include <list>
#include <stack>
#include <fstream>
#include <chrono>
#include <queue>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <map>

using namespace std;
using namespace std::chrono;

/* state value */
#define FIXED 0
#define ASSIGNED 1
#define EMPTY 2

/* best score for simulated annealing */
#define BEST_SCORE -162

/* timing parameters */
double total_duration;

/* class datastructure */
class cell {
private:
	int state;               /* state: FIXED, ASSIGNED or EMPTY */
	int value;               /* 0-9 */
	list<int> domain;        /* domain values */
	stack<int> removed_list; /* removed domain values */
public:
	friend class sudoku;

};

struct Node {
	int x1;
	int y1;
	int x2;
	int y2;
	Node(int a, int b, int c, int d) : x1(a), y1(b), x2(c), y2(d) {}
};

struct Link {
	Node a;
	Link *next;
};


class sudoku {

private:
	/* read file */
	class cell cell_list[9][9];

public:
	int read_input();
	void write_output();
	void process_input();
	int backtrack();
	int backtrack_fwd();
	int ac3_backtrack();
	int vertex_order();
	int annealing();

	int arc_reduce(Node a);

	/* copy boards */
	void copy_board(class cell new_board[9][9], class cell old_board[9][9]);

	/* flip two cells of a grid */
	void filp_selected_cell(class cell new_board[9][9], int row_1, int col_1, int row_2, int col_2);

	/* calculate score of the board */
	double calculate_score(class cell board[9][9]);

	int check_grid(int row, int col, int val);
	int check_row(int row, int col, int val);
	int check_col(int row, int col, int val);
	void update_row(int row, int col, int val);
	void update_col(int row, int col, int val);
	void update_grid(int row, int col, int val);
	void update_back_row(int row, int col);
	void update_back_col(int row, int col);
	void update_back_grid(int row, int col);
};

int get_grid_number();

/* selected algorithm */
int choice;

/* input and output text files */
char input_file[256];
char output_file[256];

/* time related data */
double start_time;
//struct timeval T;
long long start;

/* proper run command */
void Usage();

/* parse command line */
int ParseCommandLine(int argc, char *argv[]);

/* calculate random number */
double rand_double(double low, double high);
#pragma once
