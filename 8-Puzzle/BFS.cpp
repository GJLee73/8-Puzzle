#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int puzzle[3][3];
	int depth;
	struct Node* parent;
	struct Node* next;
} node;

int get_cost (int puzzle[3][3]);
void move_puzzle (int puzzle[3][3], int row, int col, int direction);
void copy_puzzle (int puzzle[3][3], int puzzle_[3][3]);
void print_puzzle (int puzzle[3][3]);

void create_node (node** head,int puzzle[3][3],int depth,node* parent);
void delete_node (node** head);
void trace_node (node* head);

node* head = NULL;
node* f_head = NULL;
int duplicate[300][9];  // 중복 방지를 위해 이전 state들을 모아 두는 배열 
int dup = 0;  // duplicate 배열에 저장된 state 개수
bool is_duplicated (int puzzle[3][3]);

void find_solution (node* n) {
	delete_node (&head);
	
	int i, j, x, y;
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			if (n->puzzle[i][j]==9) {
				x = i;
				y = j;
			}
		}
	}
	
	int puzzle_[3][3];
	
	if (y!=2) {
		copy_puzzle (puzzle_, n->puzzle);
		move_puzzle (puzzle_, x, y, 1);
		if (is_duplicated (puzzle_) != true) {
			create_node (&head, puzzle_, n->depth, n);
		}
	}
	
	if (x!=0) {
		copy_puzzle (puzzle_, n->puzzle);
		move_puzzle (puzzle_, x, y, 2);
		if (is_duplicated (puzzle_) != true) {
			create_node (&head, puzzle_, n->depth, n);
		}
	}
	
	if (y!=0) {
		copy_puzzle (puzzle_, n->puzzle);
		move_puzzle (puzzle_, x, y, 3);
		if (is_duplicated (puzzle_) != true) {
			create_node (&head, puzzle_, n->depth, n);
		}
	}
	
	if (x!=2) {
		copy_puzzle (puzzle_, n->puzzle);
		move_puzzle (puzzle_, x, y, 4);
		if (is_duplicated (puzzle_) != true) {
			create_node (&head, puzzle_, n->depth, n);
		}
	}
	
	if (get_cost(head->puzzle)==0) {
		printf("Success!\n");
		trace_node (head);
		exit(1);
	}
	
	find_solution (head);
}

int main (void) {
	//int puzzle[3][3] = {{1,2,3},{4,5,6},{7,9,8}};  // Depth: 2
	int puzzle[3][3] = {{4,3,8},{7,1,2},{5,6,9}};  // Depth: 17
	//int puzzle[3][3] = {{7,2,4},{5,9,6},{8,3,1}};  // Depth: 21
	create_node (&head, puzzle, 0, NULL);
	find_solution(head);
	return 0;
}

int get_cost (int puzzle[3][3]) {
	int i, j, k = 1, cost = 0;
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			if (puzzle[i][j]!=9 && puzzle[i][j]!=k) cost++;
			k++;
		}
	}
	return cost;
}

void move_puzzle (int puzzle[3][3], int row, int col, int direction) { 
	switch (direction) {
		case 1:
			puzzle[row][col] = puzzle[row][col+1];
			puzzle[row][col+1] = 9;
			break;
		case 2:
			puzzle[row][col] = puzzle[row-1][col];
			puzzle[row-1][col] = 9;
			break;
		case 3:
			puzzle[row][col] = puzzle[row][col-1];
			puzzle[row][col-1] = 9;
			break;
		case 4:
			puzzle[row][col] = puzzle[row+1][col];
			puzzle[row+1][col] = 9;
			break;		
	}
}

void copy_puzzle (int puzzle[3][3], int puzzle_[3][3]) {
	int i, j;
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			puzzle[i][j] = puzzle_[i][j];
		}
	}
}

void print_puzzle (int puzzle[3][3]) {
	int i, j;
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			printf ("%d ", puzzle[i][j]);
		}
	}
}

void create_node (node** head, int puzzle[3][3], int depth, node* parent) {
	node* new_node = (node*)malloc(sizeof(node));
	copy_puzzle (new_node->puzzle, puzzle);
	new_node->depth = depth+1;
	new_node->parent = parent;
	new_node->next = NULL;
	
	// 새로 추가한 state를 duplicate 배열에 저장 
	
	int i, j, k = 0;
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			duplicate[dup][k] = puzzle[i][j];
			k++;
		}
	}
	
	if (dup == 299) {
		dup = 0;
	} else {
		dup++;
	}
	
	// Queue
	
	if (*head==NULL) {
		*head = new_node;
		return;
	}
	
	node* temp = *head;
	while (temp->next!=NULL) {
		temp = temp->next;
	}
	temp->next = new_node;
}

void delete_node (node** head) {
	if (*head == NULL) return;
	
	node* temp = *head;
	*head = (*head)->next;
	
	// head node를 free node에 추가
	
	if (f_head==NULL) {
		f_head = temp;
		return;
	}
	temp->next = f_head;
	f_head = temp;
}

void trace_node (node* head) {
	node* temp = head;
	while (temp!=NULL) {
		print_puzzle (temp->puzzle);
		printf(" depth: %d\n",temp->depth);
		temp = temp->parent;
	}
}

bool is_duplicated (int puzzle[3][3]) {
	int i, j, boolean;
	for (i=0; i<300; i++) {
		boolean = 1;
		for (j=0; j<9; j++) {
			if (duplicate[i][j]!=puzzle[j/3][j%3]) boolean = 0;
		}
		if (boolean == 1) return true;
	}
	return false;
}
