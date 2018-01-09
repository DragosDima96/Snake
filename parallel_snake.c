#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


void set_rightHEAD(struct snake  *s1){

	node * nou = s1->left;
	
	while(1){	
		
		if(nou->x == s1->head.line)
			if(nou->y == s1->head.col){
			s1->right = nou;
			break;
			}
		nou = nou->next;
	}
	s1->right->next = NULL;
}



void initialize_matrix(int num_lines,int num_cols,int **dp){


int i,j;
for( i = 0 ; i < num_lines; i++)
	for(j = 0 ; j < num_cols; j++)
		dp[i][j] = 0;
}



void remove_left(struct snake * s1,int num_lines,int num_cols,int **dp){
	node * a = s1->left;
	s1->lastx = a->x;
	s1->lasty = a->y;
	dp[a->x][a->y] -= s1->encoding;
	s1->left = s1->left->next;
	free(a);

}
void reverse_remove_left(struct snake * s1,int num_lines,int num_cols,int **dp){
	dp[s1->lastx][s1->lasty] += s1->encoding;
}


void update_values(int *a,int *b,char dir,int num_lines,int num_cols){

	int x = * a;
	int y = * b;

	if(dir == 'E') {
		if(y == num_cols -1)
			y = 0;
		else
			y = y + 1;

	}

	if(dir == 'V') {
		if(y == 0)
			y = num_cols-1;
		else
			y = y -1;
	}
	
	if(dir == 'N'){
		if(x == 0)
			x = num_lines - 1;
		else 
			x = x - 1;
	}


	if(dir == 'S'){
		if(x == num_lines -1 )
			x = 0;
		else 
			x = x + 1;
	}

	*a = x;
	*b = y;

}


void add_right(struct snake  *s1,int x,int y,char dir,int num_lines,int num_cols,int **dp) {
	
	node * b = malloc(sizeof(node));
	update_values(&x,&y,dir,num_lines,num_cols);
	b->next = NULL;

	b->x = x;
	b->y = y;

	s1->right->next = b;
	s1->right = b;
	s1->right->next = NULL;
}
void add_right_dp(struct  snake *s1,int num_lines,int num_cols,int **dp){
	int x = s1->right->x;
	int y = s1->right->y;
	dp[x][y] += s1->encoding;

}

void reverse_add_right(struct snake * s1,int x,int y,int num_lines,int num_cols,int **dp){
	dp[x][y] -= s1->encoding;
}


void add_left(struct snake  *s1,int x,int y) {
	node * b = malloc(sizeof(node));
	
	b->x = x;
	b->y = y;
	
	b->next = s1->left;
	s1->left = b;

}

int check_left(struct snake  *s1,int num_lines,int num_cols,int ** world,int **dp,int *x,int *y,int encoding){
	
	int a = *x;
	int b = *y;

	if(b == 0)
		b = num_cols-1;
	else
		b = b-1;

	if(dp[a][b] == 0)
		if(world[a][b] == encoding) {
			add_left(s1,a,b);
			*x = a;
			*y = b;
			dp[a][b] += encoding;
			return 1;
		
		}

	return 0;
}

int check_right(struct snake  *s1,int num_lines,int num_cols,int ** world,int **dp,int *x,int *y,int encoding){


	int a = *x;
	int b = *y;
	if(b == num_cols-1)
		b = 0;
	else 
		b = b+1;

	if(dp[a][b] == 0)
		if(world[a][b] == encoding) {
			add_left(s1,a,b);
			*x = a;
			*y = b;
			dp[a][b] += encoding;
			return 1;
		}

	return 0;
}

int check_top(struct snake  *s1,int num_lines,int num_cols,int ** world,int **dp,int *x,int *y,int encoding){


	int a = *x;
	int b = *y;

	if(a == 0)
		a = num_lines-1;
	else
		a = a-1;

	if(dp[a][b] == 0)
		if(world[a][b] == encoding) {
			add_left(s1,a,b);
			*x = a;
			*y = b;
			dp[a][b] += encoding;
			return 1;
		}

	return 0;
}

int check_bot(struct snake  *s1,int num_lines,int num_cols,int ** world,int **dp,int *x,int *y,int encoding) {
	
	int a = *x;
	int b = *y;


	if(a == num_lines-1)
		a = 0;
	else
		a = a +1;



	if(dp[a][b] == 0)
		if(world[a][b] == encoding) {
			add_left(s1,a,b);
			*x = a;
			*y = b;
			dp[a][b] += encoding;
			return 1;
		}
	
	return 0;

	
	}


void update_snake(struct snake  *s1,int num_lines,int num_cols,int ** world,int **dp){
	

	add_right(s1,s1->right->x,s1->right->y,s1->direction,num_lines,num_cols,dp);
	remove_left(s1,num_lines,num_cols,dp);

}

int check_intersect(struct snake * snakes,int num_lines,int num_cols,int **dp,int num_snakes){
	int i = 0;
	int x = 0;
	int y = 0;
	int ok = 0;
	for(i = 0 ; i < num_snakes; i++){
		x = snakes[i].right->x;
		y = snakes[i].right->y;
		if(dp[x][y]!= snakes[i].encoding){
			ok = 1;
		}
	}
	if(ok == 1){
		for(i = 0 ; i < num_snakes; i++) {
			reverse_add_right(&snakes[i],snakes[i].right->x,snakes[i].right->y,num_lines,num_cols,dp);
			reverse_remove_left(&snakes[i],num_lines,num_cols,dp);
			}
	}

	return ok;
}

void update_snake_heads(struct snake *s1,int num_lines,int num_cols){
		
		if(s1->direction == 'E'){
			if(s1->right->y == 0)
				s1->head.col = num_cols-1;
			else
				s1->head.col = s1->right->y - 1;

			s1->head.line = s1->right->x;
		
		}
		if(s1->direction == 'V'){

			if(s1->right->y == num_cols-1)
				s1->head.col = 0;
			else
				s1->head.col = s1->right->y + 1;

			s1->head.line = s1->right->x;
		}
	
		if(s1->direction == 'N'){
			if(s1->right->x == num_lines-1)
				s1->head.line = 0;
			else
				s1->head.line = s1->right->x + 1;
			
			s1->head.col = s1->right->y;
		
		}
		if(s1->direction == 'S'){
			if(s1->right->x == 0)
				s1->head.line = num_lines-1;
			else
				s1->head.line = s1->right->x - 1;
			
			s1->head.col = s1->right->y;	

		}

}



void create_snake(struct snake  *s1,int num_lines,int num_cols,int ** world,int **dp){
	
	


	int x = 0;
	int y = 0;
	s1->left = malloc(sizeof(node));
	s1->right = malloc(sizeof(node));

	
	s1->left->next = NULL;
	s1->left->x = s1->head.line;
	s1->left->y = s1->head.col;


	x = s1->head.line;
	y = s1->head.col;
	dp[x][y] += s1->encoding;
	int neighboor_found = 1;

	while(neighboor_found == 1) {
		if(check_left(s1,num_lines,num_cols,world,dp,&x,&y,s1->encoding) == 0)
			if(check_right(s1,num_lines,num_cols,world,dp,&x,&y,s1->encoding) == 0)
				if(check_top(s1,num_lines,num_cols,world,dp,&x,&y,s1->encoding) == 0)
					if(check_bot(s1,num_lines,num_cols,world,dp,&x,&y,s1->encoding) == 0)
						neighboor_found = 0;
	}
	
	set_rightHEAD(s1);
}


void run_simulation(int num_lines, int num_cols, int **world, int num_snakes,
	struct snake *snakes, int step_count, char *file_name) {
	int i,j;
	int **dp;
	dp = (int **)malloc(num_lines * sizeof(int *));
	for (i = 0; i < num_lines; i++)
		dp[i] = (int *)calloc(num_cols, sizeof(int));
	
	for(i = 0 ; i < num_snakes;i++)
		create_snake(&snakes[i],num_lines,num_cols,world,dp);
	
	
	while(step_count > 0){
		
		#pragma omp parallel for
		for(i = 0 ; i < num_snakes;i++){
			update_snake(&snakes[i],num_lines,num_cols,world,dp);	
		}
		
		for(i = 0 ; i < num_snakes;i++)
			add_right_dp(&snakes[i],num_lines,num_cols,dp);


		if(check_intersect(snakes,num_lines,num_cols,dp,num_snakes) == 1){
			#pragma omp parallel for
			for(i = 0 ; i < num_snakes; i++){
				update_snake_heads(&snakes[i],num_lines,num_cols);
			}
			break;
		}
		
		step_count--;
		if(step_count == 0)
			#pragma omp parallel for
			for(i = 0 ; i < num_snakes;i++){
				snakes[i].head.line = snakes[i].right->x;
				snakes[i].head.col = snakes[i].right->y;
			}

	}
		for(i = 0 ; i < num_lines ; i++)
			for(j = 0 ;j < num_cols; j++)
				world[i][j] = dp[i][j];

}
