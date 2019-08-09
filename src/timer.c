#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int passes = 3;
int n = 10000; //10k
int steps = 4;
struct desc_stats {
    int n;
    float min;
    float max;
    float mean;
    float stdDev;
};

struct desc_stats getStats(int len, float* data){
    struct desc_stats stats;
    stats.n = len;
    stats.min = 100000.0;
    stats.max = -100000.0;
    float total = 0.0;
    float stdDev = 0.0;

    for(int i =0; i < len; i++){
        stats.min = (data[i] < stats.min)? data[i] : stats.min;
        stats.max = (data[i] > stats.max)? data[i] : stats.max;
        total += data[i];
    }
    stats.mean = total/len;

    for(int i =0; i < len; i++){
        stdDev += pow(data[i] - stats.mean,2);
    }
    stats.stdDev = sqrt(stdDev/stats.mean);
    return stats;
}

int main(int argc, char **argv){
    if(argc < 2){
        printf("Error please input valid params\n");
        return 1;
    }
    int command = 0;
    char cBuff[100] = "";
    char* name = argv[argc -1];
    for(int i =1; i < argc; i++){
        if(command) {
            strcat(cBuff,argv[i]);
            if(i < argc -1){ 
                strcat(cBuff, " ");
            }
        }else if(!strncmp(argv[i], "-c",4)){
            command = 1;
        } else {
            passes = atoi(argv[i]);
        }
    }
    if(!command){
        printf("please input -c followed by a run command");
        return 1;
    }

    clock_t end;     
    clock_t begin;
    float* values = malloc(passes * sizeof(float));
    int currN;
    char str[100];

    for(int p = steps; p > 0; p--){
        int i =0;
        currN = n/p;
        for(i=0; i < passes; i++){
            sprintf(str,"%s %d %d\n",cBuff,currN,currN);
            begin = time(NULL);
            system(str);
            end = time(NULL);
            printf("time is: %f\n",(float)difftime(end,begin));
            values[i] = (float)difftime(end,begin);
        }

        struct desc_stats stats = getStats(passes,values);
        printf("\n%s [N:%d] [Mean:%f]\n\n", name,currN,stats.mean);

        if(passes > 1){
            printf(" min: %f\n", stats.min);
            printf(" max: %f\n", stats.max);
            printf(" stdDev: %f\n", stats.stdDev);
        }
    }

    free(values);
}