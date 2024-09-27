#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h> 
#include <sys/time.h>
#include <ctype.h>

int row1=0;
int col1=0;
int row2=0;
int col2=0;
FILE *matrix1;
FILE *matrix2;
int matrix1_data[20][20];
int matrix2_data[20][20];
int matrix3_data[20][20];

struct thread_parameter
{
    int row;
    int col;
};

void read_matrix_data(int num){

    if(num==1){
    fscanf(matrix1, "row=%d col=%d\n",&row1,&col1);
    for(int i=0;i<row1;i++){
        for(int j=0;j<col1;j++){
            fscanf(matrix1,"%d",&matrix1_data[i][j]);
        }
    }
        fclose(matrix1);
    }
    else{
        fscanf(matrix2, "row=%d col=%d\n",&row2,&col2);
        for(int i=0;i<row2;i++){
        for(int j=0;j<col2;j++){
            fscanf(matrix2,"%d",&matrix2_data[i][j]);
        }
    }
        fclose(matrix2);
    }
   
}

void write_to_file(FILE* pointer,int i){
    if(i==1)
        fprintf(pointer,"Method: A thread per matrix\n");
    else if(i==2)
        fprintf(pointer,"Method: A thread per row\n");
    else
        fprintf(pointer,"Method: A thread per element\n");  
    fprintf(pointer,"row=%i col=%i\n",row1,col2);
    for(int i=0 ;i<row1;i++){
        for(int j=0; j<col2;j++){
           fprintf(pointer,"%d ",matrix3_data[i][j]);
        }
        fprintf(pointer,"\n");
    }
    fclose(pointer);
}

void* multiplication(void* ptr){
    int pointer=*((int*)ptr);
    if(pointer==-1){
        for(int i=0;i<row1;i++){
            for(int j=0;j<col2;j++){
                for(int k=0;k<row2;k++){
                    matrix3_data[i][j]+=matrix1_data[i][k]*matrix2_data[k][j];
                }
            }
        }
    }
   else {
        for(int j=0;j<col2;j++){
            for(int k=0;k<row2;k++){
                matrix3_data[pointer][j]+=matrix1_data[pointer][k]*matrix2_data[k][j];
            }
        }
  }
    free(ptr);
    return NULL;
}

void* multiplication_per_element(void* ptr){
    struct thread_parameter* arg=(struct thread_parameter*)ptr;
    for(int k=0;k<row2;k++){
        matrix3_data[arg->row][arg->col]+=matrix1_data[arg->row][k]*matrix2_data[k][arg->col];
    }
   free(arg);
}

void matrix_multiply_method(int method_num){
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    // clock_t t; 
    // t = clock();
    if(method_num==1){
        int *arg = malloc(sizeof(*arg));
        *arg = -1;
        multiplication(arg);
        //free(arg);
    }
    else if(method_num==2){
    pthread_t threads[row1];
        for(int i=0;i<row1;i++){
         int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&threads[i],NULL,multiplication,arg);
    }
    for(int i=0;i<row1;i++){
        pthread_join(threads[i],NULL);
    }
   // free(arg);
   }
   else{
        pthread_t threads[row1][col2];
        for(int i=0;i<row1;i++){
            for(int j=0;j<col2;j++){ 
                struct thread_parameter *arg = malloc(sizeof(struct thread_parameter));               
                arg->row=i;
                arg->col=j;
                pthread_create(&threads[i][j],NULL,multiplication_per_element,arg);
            }
        }
        for(int i=0;i<row1;i++){
            for(int j=0;j<col2;j++){
                pthread_join(threads[i][j],NULL);
            }
        }
       // free(arg);
   }
        // for(int i=0;i<row1;i++){
        //     for(int j=0;j<col2;j++){
        //         printf("%d",matrix3_data[i][j]);
        //     }
        // }
    gettimeofday(&stop, NULL);
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
    //   t = clock() - t; 
    // double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
 
    // printf("fun() took %f seconds to execute \n", time_taken); 
}

void main(){
    char input[300];
    fgets(input, sizeof(input), stdin);
    char matrix1_path_default[6]="a.txt\0";
    char matrix2_path_default[6]="b.txt\0";
    char output1_matrix_path_default[17]="c_per_matrix.txt\0";
    char output2_matrix_path_default[14]="c_per_row.txt\0";
    char output3_matrix_path_default[18]="c_per_element.txt\0";
    char matrix1_path[100]="a.txt";
    char matrix2_path[100]="b.txt";
    char output1_matrix_path[100]="c_per_matrix.txt";
    char output2_matrix_path[100]="c_per_row.txt";
    char output3_matrix_path[100]="c_per_element.txt";
    int start=11;
    for(int i=start;i<strlen(input);i++){
       if(isspace(input[i])){
        strcat(matrix1_path,".txt");
        matrix1_path[i-start+4]='\0';
        start=i+1;
        break;
       }
       matrix1_path[i-start]=input[i];
       if(i==strlen(input)-1){
        strcat(matrix1_path,".txt");
        matrix1_path[i-start+5]='\0';
        start=i+1;
        break;
       }
    }
    for(int i=start;i<strlen(input);i++){
        if(isspace(input[i])){
        strcat(matrix2_path,".txt");
        matrix2_path[i-start+4]='\0';
        start=i+1;
        break;
       }
       matrix2_path[i-start]=input[i];
       if(i==strlen(input)-1){
        strcat(matrix2_path,".txt");
        matrix2_path[i-start+5]='\0';
        start=i+1;
        break;
       }
    }

       for(int i=start;i<strlen(input);i++){
        if(isspace(input[i])){
        strcat(output1_matrix_path,"_per_matrix.txt");
        output1_matrix_path[i-start+15]='\0';
        strcat(output2_matrix_path,"_per_row.txt");
        output2_matrix_path[i-start+12]='\0';
        strcat(output3_matrix_path,"_per_element.txt");
        output3_matrix_path[i-start+16]='\0';
        start=i+1;
        break;
       }
       output1_matrix_path[i-start]=input[i];
       output2_matrix_path[i-start]=input[i];
       output3_matrix_path[i-start]=input[i];
       if(i==strlen(input)-1){
        strcat(output1_matrix_path,"_per_matrix.txt");
        output1_matrix_path[i-start+16]='\0';
        strcat(output2_matrix_path,"_per_row.txt");
        output2_matrix_path[i-start+13]='\0';
        strcat(output2_matrix_path,"_per_element.txt");
        output2_matrix_path[i-start+17]='\0'; 
        start=i+1;
        break;
       }
       }

       FILE *output1_matrix; 
       FILE *output2_matrix; 
       FILE *output3_matrix; 
       matrix1=fopen(matrix1_path,"r");
       if(matrix1==NULL) matrix1=fopen(matrix1_path_default,"r");
       matrix2=fopen(matrix2_path,"r");
       if(matrix2==NULL) matrix2=fopen(matrix2_path_default,"r");
       output1_matrix=fopen(output1_matrix_path,"a");
       if(output1_matrix==NULL) output1_matrix=fopen(output1_matrix_path_default,"a");
       output2_matrix=fopen(output2_matrix_path,"a");
       if(output2_matrix==NULL) output2_matrix=fopen(output2_matrix_path_default,"a");
        output3_matrix=fopen(output3_matrix_path,"a");
       if(output3_matrix==NULL) output3_matrix=fopen(output3_matrix_path_default,"a");
       read_matrix_data(1);
       read_matrix_data(2);
       if(col1!=row2){
        printf("Not valid!! number of columns of first matrix must equal to number of rows of second matrix\n");
        return;
       }
       printf("Matrix Multiplication by matrix\n");
       matrix_multiply_method(1);
       printf("Number of threads is 1\n\n");
       write_to_file(output1_matrix,1);
       for(int i=0;i<row1;i++){
          for(int j=0;j<col2;j++){
            matrix3_data[i][j]=0;
          }
       }
       printf("Matrix Multiplication by row\n");
       matrix_multiply_method(2);
       printf("Number of threads is %d\n\n",row1);
        write_to_file(output2_matrix,2);
        for(int i=0;i<row1;i++){
          for(int j=0;j<col2;j++){
            matrix3_data[i][j]=0;
          }
       }
      printf("Matrix Multiplication by element\n");
      matrix_multiply_method(3);
      printf("Number of threads is %d\n",row1*col2);
      write_to_file(output3_matrix,3);
}
