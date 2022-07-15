#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum maze { //duvar, boþluk, elma, hedef gibi itemler enum ile tanýmlandý.
	empty,
	wall,
	goal,
	apple
};

char** allocate_maze_matrix (int , int );
int** allocate_visited_matrix (int , int );
void get_maze (char* , char** , int , int , int *, int *);
void add_apples (char **, int , int , int);
void init_visited (char **, int **, int , int  );
void print_maze (char **, int , int );
int dfs (char **, int **, int , int , int , int , int *, int *);
void mark (char **maze, int row, int col);
void remove_mark (char **maze, int row, int col);


int main(){
	
	char **maze; 
	int **visited; 
	
	int rows, cols;
	
	int *start_row = malloc (sizeof(int)); 
	int *start_col = malloc (sizeof(int));
	int *point = malloc (sizeof(int));
	int *flag = malloc (sizeof(int));  //duvara çarpma durumunun tutulduðu flag
	
	int count;
	
	(*point) = 0;
	(*flag) = 0;

	//srand(time(NULL));
	
	printf("\n	Enter the number of rows of the maze -> ");
	scanf("%d",&rows);
	
	printf("\n	Enter the number of columns of the maze -> ");
	scanf("%d",&cols);
	
	printf("\n	Number of apples to add to the maze - > ");
	scanf("%d",&count);
	
	maze = allocate_maze_matrix(rows,cols); //labirent matrisi allocate edilir.
	
	visited = allocate_visited_matrix(rows,cols); //enum deðerlerinin tutuludðu matris allocate edilir.
	  
	//dosya ismi buraya yazýlacak!!
	get_maze("maze.txt", maze, rows, cols, start_row, start_col); //matris okunur.
	
	add_apples(maze, rows, cols, count); //elmalar eklenir
	
	init_visited(maze, visited, rows, cols); //enum deðerleri atanýr.
	
	if(!dfs(maze, visited, rows, cols, *start_row, *start_col, point, flag)){
		
		printf("\n	Maze exit not found");
		
	}else{
		
		printf("\n	Found the exit of the maze\n	Total Point --> %d\n",*point);
		print_maze(maze, rows, cols);
		
	}
	
	return 0;
}


char** allocate_maze_matrix (int rows, int cols){ //labirentin tutulduðu matris dinamik bir þekilde oluþturuluyor.
	
	int i;
	char **maze;
	
	maze = malloc(rows * sizeof(char*));
	
	
	for (i = 0; i < rows; ++i){
		
		maze[i] = malloc(cols * sizeof(char*));
		
	}
	
	return maze;
}

int** allocate_visited_matrix (int rows, int cols) { //enumlarýn tutulduðu matris dinamik bir þekilde oluþturuluyor.
	
	int i;
	int **visited;
	
	visited = malloc(rows * sizeof(int*));
	
	for (i = 0; i < rows; ++i){
		
		visited[i] = malloc(cols * sizeof(int*));
		
	}
	
	return visited;
}

void get_maze (char* file_name, char** maze, int rows, int cols, int *start_row, int *start_col) { //matris dosyadan okunuyor.
	
	int i,j;
	char c;

	FILE* maze_file = fopen(file_name, "r");

	if (!maze_file) {
		
		printf("No such file!");
		return;
		
	} 
	
	for (i = 0; i < rows; ++i) {
		
		for (j = 0; j < cols; ++j) {
			
			do{
				c = getc(maze_file);
					maze[i][j] = c;

				if (c =='b') { //baþlangýç hücresi bulunuyor.
				
					(*start_row) = i;
					(*start_col) = j;
				
				}
				
			}while(c == '\n');

		
		}
	}

	fclose(maze_file);
}

void add_apples (char **maze, int rows, int cols, int count) { //random bir þekilde elma ekleniyor.
	
	int i;
	int row, col;
	
	for(i = 0 ; i < count ; i++){ //10 adet elma eklemek tercih edildi.
		
		do{
			
			row  = rand()%rows;
			col  = rand()%rows;
			
		}while(maze[row][col] != ' ');
		
		maze[row][col] = 'o';

	}
}

void init_visited (char **maze, int **visited, int rows, int cols ) { //enum deðerleri atanýyor.

	int i, j;
	
	for (i = 0; i < rows; ++i) {
		
		for (j = 0; j < cols; ++j) {
			
			if (maze[i][j] == '-' || maze[i][j] == '|' || maze[i][j] == '+') { 
				
				visited[i][j] = wall;
				
			} else if (maze[i][j] == 'c') {
				
				visited[i][j] = goal;
				
			} else if(maze[i][j] == 'o'){
				
				visited[i][j] = apple;
				
			}else if(maze[i][j] == ' '){
				
				visited[i][j] = empty;
				
			}
		}
	}
}

void print_maze (char **maze, int rows, int cols) { //matris ekrana yazdýrýlýyor

	int i, j;
	
	for (i = 0; i < rows; ++i) {
		
		printf("	");
		
		for (j = 0; j < cols; ++j) {
			
			printf("%c", maze[i][j]);
			
		}
		
		printf("\n");
	}	
	
	printf("\n");
}

void mark (char **maze, int row, int col) {
	
	maze[row][col] = '*'; //gezilen yollar * eklenerek görselleþtirilir.
	
}

void remove_mark (char **maze, int row, int col) {

	maze[row][col] = ' '; //duvara çarptýktan sonra dönülen yol boyunca eklenen *lar silinir.

}

int dfs (char **maze, int **visited, int rows, int cols, int current_row, int current_col, int *point, int *flag) { // depth first searh algoritmasý iþleniyor.
	
	int *current = & visited[current_row][current_col];
	
	if (*current == goal) { // eðer hedefe ulaþýldýysa 
		
		return 1;
		
	}

	if (*current == empty  || *current == apple){ //elma veya boþ yol varsa
	
		if(*current == apple){ //bir elma ile karþýlaþýldýysa
		
			printf("\n	An apple was found. + 10 points !!\n");
			(*point) += 10; //puan 10 artýrýlýr.
		
		}
		
		if((*flag) == 1){ //duvara çarpýldýysa
			
			printf("\n	Hit the wall ! - 5 points !!\n");
			(*point) -= 5; //puan 5 azaltýlýr.
			
		}
		
		(*flag) = 0; 
		
		printf("\n	Total Point --> %d\n",*point);
		print_maze(maze, rows, cols);
		
		//sleep(1);
		usleep(100);
		system("cls");
		
		mark(maze, current_row, current_col);
		
 		*current = wall;	
		
		if (dfs(maze, visited, rows, cols, current_row, current_col + 1, point, flag)){ //aþaðý
			
			(*current) = wall;
			return 1;
					
		} 
		if (dfs(maze, visited, rows, cols,current_row, current_col - 1, point, flag)){ //yukarý
			
			(*current) = wall;
			return 1;
			
		} 
		if (dfs(maze, visited, rows, cols,current_row + 1, current_col, point, flag)){  //sol
		
			(*current) = wall;
			return 1;
			
		} 
		if (dfs(maze, visited, rows, cols,current_row - 1, current_col, point, flag)){ //sað
			
			(*current) = wall;
			return 1;
			
		}

		//gidilebilecek tüm yollara gittikten sonra if bloklarýndan çýkarýz ve bu duvara çarptýðmýz anlamýna gelir.
		
		(*flag) = 1; 
		
		printf("\n	Total Point --> %d\n",*point);
		print_maze(maze, rows, cols);
		
	
		//sleep(1);
		usleep(100);
		system("cls");
		
		remove_mark(maze, current_row, current_col);
	
	}

	return 0;
}


 
