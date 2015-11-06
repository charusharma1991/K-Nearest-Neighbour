#include"header.h"
void func_main(int **matrix,int **unobserved_set,int user,int movie_count,int col,int unobserved_row,int knn)
{
    srand(time(NULL));
    int i,j,k,n,flag,row,index,row_test,*a,error=0,rating,count=0;
    int **train_set,**test_set,*recommended,*kmin_pos,*rates;
    double percent=0.0,*dist;
    row=(int)(0.75*(double)movie_count);
    train_set=(int **)malloc(sizeof(int*)*row);
    for(i=0;i<row;i++)
       train_set[i]=(int*)malloc(sizeof(int)*col);
    test_set=(int **)malloc(sizeof(int*)*(movie_count-row));
    for(i=0;i<(movie_count-row);i++) 
	test_set[i]=(int*)malloc(sizeof(int)*col);
    a=(int *)malloc(sizeof(int)*movie_count);
    rates=(int *)malloc(sizeof(int)*knn);
    recommended=(int *)malloc(sizeof(int)*TOP);
    i=0;
    while(i<movie_count){
	n=rand()%movie_count;
	for(j=0;j<movie_count;j++){
	    if(n==a[i]){
		flag=0;
		break;
	    }
	    else{
		flag=1;
	    }
	}
	if(flag==1){
	    a[i]=n;
	    i++;
	}
    }

    for(i=0;i<row;i++){							//store 75% data from observed matrix to training set
	index=a[i];
	for(j=0;j<col;j++){
	    train_set[i][j]=matrix[index][j];
	}
    }
    row_test=0;
    for(i=row;i<movie_count;i++){					//store 25% data from observed matrix to test set
	index=a[i];
	for(j=0;j<col;j++){
	    test_set[row_test][j]=matrix[index][j];
	}
	row_test++;
    }
    for(i=0;i<unobserved_row;i++){					//predict ratings for unobserved data set
	dist=distance(train_set,unobserved_set,row,i,col,knn);
	kmin_pos=kmin(dist,row,knn);
	for(j=0;j<knn;j++){
	    k=kmin_pos[j];
	    rates[j]=train_set[k][col-1];
	}
	rating=majority(rates,knn);
	unobserved_set[i][col-1]=rating;
    }
    for(i=0;i<row_test;i++){						//calculate error for test set
	dist=distance(train_set,unobserved_set,row,i,col,knn);
	kmin_pos=kmin(dist,row,knn);
	for(j=0;j<knn;j++){
	    k=kmin_pos[j];
	    rates[j]=train_set[k][col-1];
	}
	rating=majority(rates,knn);
	if(rating!=test_set[i][col-1]){
	    error++;
	}
    }
    percent=((double)error/(double)row_test)*100.0;
    printf("Error percentage for user %d is :%f\n",user,percent);
    i=0,count=0;
    int max=MAX_RATING;
    while(count<TOP){							//find TOP 10 rated movies
	if(i<row_test && test_set[i][col-1]==max && count<TOP){
	    recommended[count++]=test_set[i][0];
	}
	if(i<unobserved_row && unobserved_set[i][col-1]==max && count<TOP){
	    recommended[count++]=unobserved_set[i][0];
	}
	i++;
	if(i>=row_test && i>=unobserved_row && count<TOP){
	    i=0;
	    max--;
	}
	
    }
    printf("Top ten recommended movies from user %d are:\n",user);
    for(i=0;i<count;i++){
	printf("%d movie id is %d\n",i+1,recommended[i]);
    }
}
double* distance(int **train_set,int **test_set,int row,int row_test,int col,int knn)
{
    int j,k;
    double sum,*dist;
    dist=(double *)malloc(sizeof(double)*row);
    for(j=0;j<row;j++){							//calculate euclidean distance
	sum=0.0;
	for(k=1;k<col-1;k++){
	    sum+=pow((test_set[row_test][k]-train_set[j][k]),2);
	}
	dist[j]=sqrt(sum);
    }
    return dist;
}
int* kmin(double *distance,int row,int knn)				//find k nearest neighbours
{
    int i,j,*kmin_pos,a,b,*index;
    kmin_pos=(int *)malloc(sizeof(int)*knn);
    index=(int *)malloc(sizeof(int)*row);
    for(i=0;i<row;i++){
	index[i]=i;
    }
    for(i=0;i<row;i++){
	for(j=i+1;j<row;j++){
	    if (distance[i]>distance[j])
            {
                a =distance[i];
		b=index[i];
                distance[i]=distance[j];
		index[i]=index[j];
                distance[j]=a;
		index[j]=b;
            }
	}
    }
    for(i=0;i<knn;i++){
	kmin_pos[i]=index[i];
    }
    return kmin_pos;
}
int majority(int *rates,int knn)					//find majority rating from k nearest neighbour
{
    int i,*count,max,pos;
    count=(int*)malloc(sizeof(int)*(MAX_RATING+1));
    for(i=0;i<=MAX_RATING;i++){
	count[i]=0;
    }
    for(i=0;i<knn;i++){
	count[rates[i]]+=1;
    }
    max=count[0];
    for(i=0;i<=MAX_RATING;i++){
	if(max<count[i]){
	    max=count[i];
	    pos=i;
	}
    }
    return pos;
}
