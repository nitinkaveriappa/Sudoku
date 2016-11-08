#include "Sudoku.h"


/* proper run time command */
void Usage()
{
	cout << "./sudoku a1/a2/a3/a4/a5 input_file output_file\n";

}

/* parse command line */
int ParseCommandLine(int argc, char *argv[])
{
	if (argc == 4)
	{
		if (strcmp(argv[1], "a1") == 0)
			choice = 1;
		else if (strcmp(argv[1], "a2") == 0)
			choice = 2;
		else if (strcmp(argv[1], "a3") == 0)
			choice = 3;
		else if (strcmp(argv[1], "a4") == 0)
			choice = 4;
		else if (strcmp(argv[1], "a5") == 0)
			choice = 5;

		strcpy_s(input_file, argv[2]);
		strcpy_s(output_file, argv[3]);
		return 1;
	}
	else
	{
		Usage();
		return 0;
	}

}


/* read sudoku input from file and store it in cell list */
int sudoku::read_input()
{
	int i;
	int j;
	ifstream myReadFile;
	myReadFile.open(input_file);
	if (myReadFile.is_open()) {
		for (i = 0; i<9; i++)
		{
			for (j = 0; j<9; j++)
			{
				if (!myReadFile.eof())
					myReadFile >> cell_list[i][j].value;
				else
				{
					cout << "Invalid file";
					return 0;
				}
			}
		}
		myReadFile.close();
		return 1;
	}
	else
		return 0;
}

/* write output to file */
void sudoku::write_output()
{
	int i;
	int j;
	ofstream myWriteFile(output_file);
	if (myWriteFile.is_open()) {
		for (i = 0; i<9; i++)
		{
			for (j = 0; j<9; j++)
			{
				myWriteFile << cell_list[i][j].value;
				myWriteFile << " ";
			}
			myWriteFile << "\n";
		}
		myWriteFile.close();
	}
	else
	{
		cout << "Invalid output file\n";
	}

}
/* update cell list based on input */
void sudoku::process_input()
{
	int i;
	int j;
	int arr[] = { 1,2,3,4,5,6,7,8,9 };
	for (i = 0; i<9; i++)
	{
		for (j = 0; j<9; j++)
		{
			if (cell_list[i][j].value == 0)
			{
				cell_list[i][j].domain.assign(arr, arr + 9);
				cell_list[i][j].state = EMPTY;
			}
			else
			{
				cell_list[i][j].state = FIXED;
			}

		}
	}
}


/* backtracking algorithm */
int sudoku::backtrack()
{
	/* do the stuff */
	int row;
	int col;
	int flag = 0;
	int i;
	int j;

	flag = 0;
	/* find empty cell */
	for (i = 0; i<9; i++)
	{
		for (j = 0; j<9; j++)
		{
			if (cell_list[i][j].state == EMPTY) {
				row = i;
				col = j;
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			break;
	}
	/* check if sudoku is full */
	if (flag == 0)
		return 1;

	/* try 1-9 values on empty cell */
	for (i = 1; i < 10; i++)
	{
		if (check_grid(row, col, i) && check_row(row, col, i) && check_col(row, col, i))
		{
			cell_list[row][col].value = i;
			cell_list[row][col].state = ASSIGNED;
			if (backtrack())
				return 1;
			cell_list[row][col].state = EMPTY;
			cell_list[row][col].value = 0;
		}

	}
	return 0;
}

int sudoku::check_grid(int row, int col, int val)
{
	int grid_row = row / 3;
	int grid_col = col / 3;
	int i;
	int j;
	for (i = grid_row * 3; i<grid_row * 3 + 3; i++)
	{
		for (j = grid_col * 3; j<grid_col * 3 + 3; j++)
		{
			if (cell_list[i][j].value == val)
				return 0;
		}
	}
	return 1;

}

int sudoku::check_row(int row, int col, int val)
{
	int i;
	for (i = 0; i<9; i++)
	{
		if (cell_list[row][i].value == val)
			return 0;
	}
	return 1;

}

int sudoku::check_col(int row, int col, int val)
{
	int i;
	for (i = 0; i<9; i++)
	{
		if (cell_list[i][col].value == val)
			return 0;
	}
	return 1;

}

/* backtracking with forward checking */
int sudoku::backtrack_fwd()
{
	int row;
	int col;
	int flag = 0;
	int i;
	int j;
	list<int>::iterator it;

	flag = 0;
	/* find empty cell */
	for (i = 0; i<9; i++)
	{
		for (j = 0; j<9; j++)
		{
			if (cell_list[i][j].state == EMPTY) {
				row = i;
				col = j;
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			break;
	}
	/* check if sudoku is full */
	if (flag == 0)
		return 1;

	if (cell_list[row][col].domain.empty())
	{
		return 0;
	}

	for (it = cell_list[row][col].domain.begin(); it != cell_list[row][col].domain.end(); ++it)
	{
		if (check_grid(row, col, *it) && check_row(row, col, *it) && check_col(row, col, *it))
		{
			cell_list[row][col].value = *it;
			cell_list[row][col].state = ASSIGNED;

			update_row(row, col, *it);
			update_col(row, col, *it);
			update_grid(row, col, *it);

			if (backtrack_fwd())
				return 1;

			cell_list[row][col].state = EMPTY;
			cell_list[row][col].value = 0;

			update_back_row(row, col);
			update_back_col(row, col);
			update_back_grid(row, col);
		}

	}
	return 0;

}

void sudoku::update_row(int row, int col, int val)
{
	int i;
	for (i = 0; i<9; i++)
	{
		if (i == col || cell_list[row][i].state == FIXED || cell_list[row][i].state == ASSIGNED)
			continue;
		else
		{
			cell_list[row][i].domain.remove(val);
			cell_list[row][i].removed_list.push(val);
		}
	}

}

void sudoku::update_col(int row, int col, int val)
{
	int i;
	for (i = 0; i<9; i++)
	{
		if (i == row || cell_list[i][col].state == FIXED || cell_list[i][col].state == ASSIGNED)
			continue;
		else
		{
			cell_list[i][col].domain.remove(val);
			cell_list[i][col].removed_list.push(val);
		}
	}
}

void sudoku::update_grid(int row, int col, int val)
{
	int grid_row = row / 3;
	int grid_col = col / 3;
	int i;
	int j;
	for (i = grid_row * 3; i<grid_row * 3 + 3; i++)
	{
		for (j = grid_col * 3; j<grid_col * 3 + 3; j++)
		{
			if (i == row && j == col || cell_list[i][j].state == FIXED || cell_list[i][j].state == ASSIGNED)
				continue;
			else
			{
				cell_list[i][j].domain.remove(val);
				cell_list[i][j].removed_list.push(val);
			}
		}
	}

}

void sudoku::update_back_row(int row, int col)
{
	int i;
	int val;
	for (i = 0; i<9; i++)
	{
		if (i == col || cell_list[row][i].state == FIXED || cell_list[row][i].state == ASSIGNED)
			continue;
		else
		{
			val = cell_list[row][i].removed_list.top();
			cell_list[row][i].removed_list.pop();
			cell_list[row][i].domain.push_back(val);
		}
	}
}

void sudoku::update_back_col(int row, int col)
{
	int i;
	int val;
	for (i = 0; i<9; i++)
	{
		if (i == row || cell_list[i][col].state == FIXED || cell_list[i][col].state == ASSIGNED)
			continue;
		else
		{
			val = cell_list[i][col].removed_list.top();
			cell_list[i][col].removed_list.pop();
			cell_list[i][col].domain.push_back(val);
		}
	}
}

void sudoku::update_back_grid(int row, int col)
{
	int grid_row = row / 3;
	int grid_col = col / 3;
	int i;
	int j;
	int val;
	for (i = grid_row * 3; i<grid_row * 3 + 3; i++)
	{
		for (j = grid_col * 3; j<grid_col * 3 + 3; j++)
		{
			if (i == row && j == col || cell_list[i][j].state == FIXED || cell_list[i][j].state == ASSIGNED)
				continue;
			else
			{
				val = cell_list[i][j].removed_list.top();
				cell_list[i][j].removed_list.pop();
				cell_list[i][j].domain.push_back(val);
			}
		}
	}
}

/* ac3 backtracking */
int sudoku::ac3_backtrack()
{
	int row;
	int col;
	int flag = 0;
	int i;
	int j;
	int change = 0;
	
	queue<Node> myqueue;

	//add every row pair
	for (int row = 0; row < 9; row++)
		for (int i = 0; i < 9; i++)
			for (int j = i + 1; j < 9; j++)
				myqueue.push(Node(row,i,row,j));
	// add every col pair
	for (int col = 0; col < 9; col++)
		for (int i = 0; i < 9; i++)
			for (int j = i + 1; j < 9; j++)
				myqueue.push(Node(i,col,j,col));
	// add every grid
	for (int gridx = 0; gridx < 3; gridx++)
		for (int gridy = 0; gridy < 3; gridy++)
			// loop over guts of grid to create constraints
			for (int comp1 = 0; comp1 < 9; comp1++)
				for (int comp2 = comp1 + 1; comp2 < 9; comp2++) {
					int x1 = (comp1 % 3) + gridx * 3;
					int y1 = (comp1 / 3) + gridy * 3;
					int x2 = (comp2 % 3) + gridx * 3;
					int y2 = (comp2 / 3) + gridy * 3;
					myqueue.push(Node(x1,y1,x2,y2));
				}


	while (!myqueue.empty())
	{
		Node a = myqueue.front();
		myqueue.pop();
		if (arc_reduce(a)) {
			change = 1;
			//myqueue.push(Node(a.x1,a.y1,a.x2,a.y2));
		}

	}
	/*
	Node a = myqueue.front();
	myqueue.pop();
	change = arc_reduce(a);
	*/
	for (i = 0; i<9; i++)
	{
		for (j = 0; j<9; j++)
		{
			if (cell_list[i][j].state == EMPTY) {
				row = i;
				col = j;
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			break;
	}
	/* check if sudoku is full */
	if (flag == 0)
		return 1;

	/* try 1-9 values on empty cell */
	for (i = 1; i < 10; i++)
	{
		if (check_grid(row, col, i) && check_row(row, col, i) && check_col(row, col, i))
		{
			cell_list[row][col].value = i;
			cell_list[row][col].state = ASSIGNED;
			if (ac3_backtrack())
				return 1;
			cell_list[row][col].state = EMPTY;
			cell_list[row][col].value = 0;
		}

	}
	/*
		if (backtrack())
		return 1;
		*/

	return 0;
}

int sudoku::arc_reduce(Node a) 
{
	int one, two;
	int found = 0, change = 0;
	list<int>::iterator it;
	list<int>::iterator ti;

	list<int> d1 = cell_list[a.x1][a.y1].domain;
	list<int> d2 = cell_list[a.x2][a.y2].domain;

	for (it = cell_list[a.x1][a.y1].domain.begin(); it != cell_list[a.x1][a.y1].domain.end(); ++it)
	{
		one = *it;
		found = false;
		
		for (ti = cell_list[a.x2][a.y2].domain.begin(); ti != cell_list[a.x2][a.y2].domain.end(); ++ti)
		{
			two = *ti;

			if (one == two && a.x1 == a.x2) //same value same line
				found = 1;
			if (one == two && a.y1 == a.y2) //same value same column
				found = 1;

		}
		if (found == 0) {
			update_row(a.x1, a.y1, *it);
			update_col(a.x1, a.y1, *it);
			update_grid(a.x1, a.y1, *it);
			change = 1;
		}

	}
	return change;
}

/* back tracking with vertex order */
int sudoku::vertex_order()
{
	/* do the stuff */
	int row;
	int col;
	int flag = 0;
	int i;
	int j;
	list<int>::iterator it;

	flag = 0;
	int min = 9 + 1;
	
	/*find empty cell with least domain value*/
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if ((cell_list[i][j].state == FIXED) || (cell_list[i][j].state == ASSIGNED))
			{
				update_row(i, j, cell_list[i][j].value);
				update_col(i, j, cell_list[i][j].value);
				update_grid(i, j, cell_list[i][j].value);
			}
		}
	}

	/*find empty cell with least domain value*/
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if ((cell_list[i][j].state == EMPTY) && (cell_list[i][j].domain.size() < min))
			{
				row = i;
				col = j;
				flag = 1;
				min = cell_list[row][col].domain.size();
			}
		}
		if (flag == 1)
			break;
	}
	/* check if sudoku is full */
	if (flag == 0)
		return 1;
	
	
	
	for (i = 1; i < 10; i++)
	{
		if (check_grid(row, col, i) && check_row(row, col, i) && check_col(row, col, i))
		{
			cell_list[row][col].value = i;
			cell_list[row][col].state = ASSIGNED;
			if (vertex_order())
				return 1;
			cell_list[row][col].state = EMPTY;
			cell_list[row][col].value = 0;
		}

	}
	return 0;

}

/* simulated annealing algorithm */
int sudoku::annealing()
{
	/* start timer */
	//std::chrono::time_point<std::chrono::system_clock> start, end;
	//start = std::chrono::system_clock::now();

	/* simulated annealing */
	int i;
	int j;
	int k;

	/* new board configuration */
	class cell new_board[9][9];

	/* randomization parameters */
	volatile int rand_grid_row;
	volatile int rand_grid_col;
	volatile int rand_cell_1_row;
	volatile int rand_cell_1_col;
	volatile int rand_cell_2_row;
	volatile int rand_cell_2_col;
	volatile int cell_1_row;
	volatile int cell_1_col;
	volatile int cell_2_row;
	volatile int cell_2_col;

	/* old board and new board scores */
	double old_score;
	double new_score;

	/* Best board and best board's score */
	double best_score = 100000;
	class cell best_board[9][9];

	/* Initial global temperature */
	long double T = 2.5;

	int count = 0;

	volatile double delta;
	volatile double random_value;

	srand(time(0));

	int arr[] = { 1,2,3,4,5,6,7,8,9 };
	list<int> domain_1;
	list<int> domain_2;
	list<int> domain_3;
	list<int>::iterator it_1;
	list<int>::iterator it_2;
	list<int>::iterator it_3;
	int pos;
	int value;
	/* randomly fill empty cells */
	for (k = 0; k < 3; k++)
	{
		domain_1.clear();
		domain_2.clear();
		domain_3.clear();
		domain_1.assign(arr, arr + 9);
		domain_2.assign(arr, arr + 9);
		domain_3.assign(arr, arr + 9);
		for (i = k * 3 + 0; i < (k * 3 + 3); i++)
		{
			for (j = 0; j < 9; j++)
			{
				if (cell_list[i][j].value != 0) {
					if (j >= 0 && j<3)
						domain_1.remove(cell_list[i][j].value);
					else if (j >= 3 && j<6)
						domain_2.remove(cell_list[i][j].value);
					else
						domain_3.remove(cell_list[i][j].value);
				}
			}
		}
		for (i = k * 3 + 0; i < (k * 3 + 3); i++)
		{
			for (j = 0; j < 9; j++)
			{
				if (cell_list[i][j].value == 0) {
					if (j >= 0 && j<3) {
						pos = rand() % domain_1.size();
						it_1 = domain_1.begin();
						advance(it_1, pos);
						value = *it_1;
						cell_list[i][j].value = value;
						domain_1.remove(value);
					}
					else if (j >= 3 && j<6) {
						pos = rand() % domain_2.size();
						it_2 = domain_2.begin();
						advance(it_2, pos);
						value = *it_2;
						cell_list[i][j].value = value;
						domain_2.remove(value);
					}
					else {
						pos = rand() % domain_3.size();
						it_3 = domain_3.begin();
						advance(it_3, pos);
						value = *it_3;
						cell_list[i][j].value = value;
						domain_3.remove(value);
					}
				}
			}
		}
	}


	/* repeat until best score is reached */
	while (1)
	{

		/* check if initial state is desired final state */
		old_score = calculate_score(cell_list);
		if (old_score == -162) {
			best_score = -162;
			break;
		}
		do
		{
			/* select a grid randomly */
			rand_grid_row = rand() % 3;
			rand_grid_col = rand() % 3;

			/* choose random squares in the grid */
			rand_cell_1_row = rand() % 3;
			rand_cell_1_col = rand() % 3;
			rand_cell_2_row = rand() % 3;
			rand_cell_2_col = rand() % 3;
			while (rand_cell_1_row == rand_cell_2_row && rand_cell_1_col == rand_cell_2_col) {
				rand_cell_2_row = rand() % 3;
				rand_cell_2_row = rand() % 3;
			}
			cell_1_row = rand_grid_row * 3 + rand_cell_1_row;
			cell_1_col = rand_grid_col * 3 + rand_cell_1_col;
			cell_2_row = rand_grid_row * 3 + rand_cell_2_row;
			cell_2_col = rand_grid_col * 3 + rand_cell_2_col;

		} while (cell_list[cell_1_row][cell_1_col].state == FIXED || cell_list[cell_2_row][cell_2_col].state == FIXED);

		/* copy board and flip selected cells */
		this->copy_board(new_board, cell_list);
		this->filp_selected_cell(new_board, cell_1_row, cell_1_col, cell_2_row, cell_2_col);

		old_score = calculate_score(cell_list);
		new_score = calculate_score(new_board);


		delta = old_score - new_score;
		random_value = rand_double(0.0, 1.0);

		if (new_score < old_score)
		{
			cout << "Flipping 2 cells at position <" << cell_1_row << "," << cell_1_col << ">  <" << cell_2_row << "," << cell_2_col << ">\n";
			this->copy_board(cell_list, new_board);
			old_score = new_score;
		}
		else if ((exp(double(delta / T)) - random_value) > 0)
		{
			cout << "Flipping 2 cells at position <" << cell_1_row << "," << cell_1_col << ">  <" << cell_2_row << "," << cell_2_col << ">\n";
			this->copy_board(cell_list, new_board);

			old_score = new_score;

		}
		if (new_score < best_score)
		{
			this->copy_board(best_board, new_board);
			best_score = new_score;
		}
		if (new_score == -162)
			break;
		if (T <= 0)
			break;

		T *= 0.99999;
		count++;

	}

	/* end timer and update the duration */
	//end = std::chrono::system_clock::now();
	//std::chrono::duration<double> elapsed_seconds = end - start;
	//total_duration = elapsed_seconds.count();

	if (best_score == -162)
		return 1;

	return 0;
}

/* copy old board on new board */
void sudoku::copy_board(class cell new_board[9][9], class cell old_board[9][9])
{
	int i, j;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
			new_board[i][j].value = old_board[i][j].value;
	}
}

/* flip selected cells */
void sudoku::filp_selected_cell(class cell new_board[9][9], int row_1, int col_1, int row_2, int col_2)
{
	int temp;
	temp = new_board[row_1][col_1].value;
	new_board[row_1][col_1].value = new_board[row_2][col_2].value;
	new_board[row_2][col_2].value = temp;
}

/* Calculate score */
double sudoku::calculate_score(class cell board[9][9])
{
	int i;
	int j;

	double score = 0;
	map<int, int> num_map;
	map<int, int>::iterator it;

	/* number scores in every row */
	for (i = 0; i<9; i++)
	{
		for (j = 0; j<9; j++)
		{
			it = num_map.find(board[i][j].value);
			if (it == num_map.end())
			{
				num_map.insert(pair<int, int>(board[i][j].value, 1));
			}
			else
			{
				it->second++;
			}
		}
		for (it = num_map.begin(); it != num_map.end(); it++)
		{
			if (it->second == 1) {
				score--;
			}
		}
		num_map.clear();
	}
	num_map.clear();

	/* number scores in every column */
	for (i = 0; i<9; i++)
	{
		for (j = 0; j<9; j++)
		{
			it = num_map.find(board[j][i].value);
			if (it == num_map.end())
			{
				num_map.insert(pair<int, int>(board[j][i].value, 1));
			}
			else
			{
				it->second++;
			}
		}
		for (it = num_map.begin(); it != num_map.end(); it++)
		{
			if (it->second == 1)
			{
				score--;
			}
		}
		num_map.clear();
	}
	return score;
}

/* generates random float values within the given range */
double rand_double(double low, double high) {
	return ((double)rand() * (high - low)) / (double)RAND_MAX + low;
}
/* main function */
int main(int argc, char *argv[])
{
	int result;
	sudoku s;
	if (ParseCommandLine(argc, argv) == 0)
	{
		return 0;
	}
	if (s.read_input() == 0)
		return 0;
	s.process_input();
	switch (choice)
	{
	case 1:
		result = s.backtrack();
		break;
	case 2:
		result = s.backtrack_fwd();
		break;
	case 3:
		result = s.ac3_backtrack();
		break;
	case 4:
		result = s.vertex_order();
		break;
	case 5:
		result = s.annealing();
		break;
	}
	if (result == 0)
	{
		cout << "sudoku couldn't be solved\n";
		s.write_output();
		return 0;
	}
	/* write output to file */
	s.write_output();
	cout << "Sudoku successfully solved!Check the output file\n";
	cout << "It took" << total_duration << "seconds\n";
	return 0;
}

