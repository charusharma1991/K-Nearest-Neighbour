#include"header.h"
int main(int argc, char *argv[])
{
    srand(time(NULL));
    if(argc>4){
	printf("Too many arguements provided\n");
	return -1;
    }
    else if(argc<4){
	printf("Insufficient data: 4 arguements expected\n");
	return -1;
    }
    else{
	int knn;
	FILE *file1=fopen(argv[1],"r");
	FILE *file2=fopen(argv[2],"r");
	sscanf(argv[3],"%d",&knn);
	if(knn>15 || knn<1){
	    printf("Enter K value less than 16 and greater than 1 as some user have rated only 20 movies\n");
	    return -1;
	}
	if(file1==NULL || file2==NULL){
	    printf("\n file opening failed ");
	    return -1 ;
	}
	else{
	    int row=0,col=0,i=0,j=0,row2=0,col2=0,p,k,m,index;
	    char buffer[1024];
	    char *record,*line;
	    int **user_matrix,*m_count,**movie_matrix;
	    m_count=(int *)malloc(sizeof(int)*USER);
    	    while((line=fgets(buffer,sizeof(buffer),file1))!=NULL){			//count rows and cols of first file "u.data"
		col=0;
		record = strtok(line,"\t");
		while(record != NULL){
		    col++;
		    record = strtok(NULL,"\t");
		}
		row++;
	    }
	    rewind(file1);
	    user_matrix=(int **)malloc(sizeof(int*)*row);
	    for(k=0;k<row;k++) 
		user_matrix[k]=(int*)malloc(sizeof(int)*col);
	    while((line=fgets(buffer,sizeof(buffer),file1))!=NULL){			//store values from file "u.data" into user_matrix
		j=0;
		record = strtok(line,"\t");
		while(record != NULL){
		    sscanf(record,"%d",&p);
		    user_matrix[i][j]=p;
		    j++;
		    record = strtok(NULL,"\t");
		}
		i++;
	    }
	    for(m=0;m<USER;m++){
		m_count[m]=0;
	    }
	    for(m=0;m<row;m++){
		index=user_matrix[m][0];
		m_count[index]++;
	    }
	    while((line=fgets(buffer,sizeof(buffer),file2))!=NULL){			//count rows and cols of second file "u.item"
		col2=0;
		record = strtok(line,"|");
		while(record != NULL){
		    col2++;
		    record = strtok(NULL,"|");
		}
		row2++;
	    }
	    rewind(file2);
	    movie_matrix=(int **)malloc(sizeof(int*)*row2);
	    for(k=0;k<row2;k++) 
		movie_matrix[k]=(int*)malloc(sizeof(int)*col2);
	    i=0;
	    while((line=fgets(buffer,sizeof(buffer),file2))!=NULL){			//store values from file "u.item" into movie_matrix
		j=0;
		record = strtok(line,"|");
		while(record != NULL){
		    sscanf(record,"%d",&p);
		    movie_matrix[i][j]=p;
		    j++;
		    record = strtok(NULL,"|");
		}
		i++;
	    }
	    int movie_count,column,user,id;
	    int **matrix;
	    for(i=0;i<NO_OF_USER;i++){							//find TOP 10 recommended movies for 50 users
		movie_count=0;
		column=0;
		user=rand()%USER;
		matrix=(int **)malloc(sizeof(int*)*m_count[user+1]);
		for(k=0;k<m_count[user+1];k++)						//generate matrix of rated movies for each user
		    matrix[k]=(int*)malloc(sizeof(int)*USER_COLUMN);
		for(j=0;j<row;j++){
		    if(user_matrix[j][0]==user+1){
			matrix[movie_count][column]=user_matrix[j][column+1];
			movie_count++;
		    }
		}
		for(j=0;j<movie_count;j++){
		    column=0;
		    id=matrix[j][column++];
		    for(k=4;k<col2;k++){
			matrix[j][column++]=movie_matrix[id-1][k];
		    }
		}
		movie_count=0;
		for(j=0;j<row;j++){
		    if(user_matrix[j][0]==user+1){
			matrix[movie_count++][column]=user_matrix[j][col-2];
		    }
		}
		column++;
		int unobserved_row=0,flag=0,col_count=0;
		int **unobserved_set;
		unobserved_set=(int **)malloc(sizeof(int*)*(MOVIE-movie_count));	//unobserved matrix for each user needs to be predicted
		for(k=0;k<(MOVIE-movie_count);k++)
		    unobserved_set[k]=(int*)malloc(sizeof(int)*USER_COLUMN);
		for(j=0;j<MOVIE;j++){
		    flag=0;
		    for(k=0;k<movie_count;k++){
			if(matrix[k][0]==movie_matrix[j][0]){
			    flag=1;
			}
		    }
		    if(flag!=1){
			col_count=0;
			unobserved_set[unobserved_row][col_count++]=movie_matrix[j][0];
			for(k=4;k<col2;k++)
			    unobserved_set[unobserved_row][col_count++]=movie_matrix[j][k];
			unobserved_row++;
		    }
    		}
		printf("Details for %d User is:\n",i+1);
		func_main(matrix,unobserved_set,user,movie_count,column,unobserved_row,knn);
	    }
	}
    }
    return 0;
}
