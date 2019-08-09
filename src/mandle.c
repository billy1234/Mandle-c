#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//cached variables
double zR;
double zI;
double cR;
double cI;
double temp;
float currIterates;


int i; 
int j;

struct zoomSettings{
    int width;
    int height;
    float iterates;
    double scale;
    double rCenter;
    double iCenter;
};

struct zoomSettings settings = {100,100,255.0,0.1,-0.75,0.1};
char gradient[6][4] = {
    " ",
    ".",
    "░",
    "▒",
    "▓",
    "█"
    }; 
int gradientLen = 4; //final elem id of ^

char *numToStr(unsigned char c){
    return ((c == (unsigned char)255)?
        gradient[gradientLen] : 
        gradient[(int)(((float)c/255.0) * gradientLen)]);
}

unsigned char getDivergeAt(double r,double i){
	cR = (r * settings.scale) + settings.rCenter;
    cI = (i * settings.scale) + settings.iCenter;
    zR = 0.0;
    zI = 0.0;
    currIterates = 0;
	while((currIterates < settings.iterates) && ((zR * zR + zI * zI) < 4.0)){
		temp = zR * zR - zI * zI + cR;
		zI = 2.0 * zR * zI + cI;
		zR = temp;
		currIterates++;
	}
    return (char)(255.0 * (currIterates/settings.iterates));
}

void displayArr(unsigned char* arr){
    char row[(settings.width *4) + 1]; //*3 as extended ascii may take up that much space
    for(j=0; j < settings.height; j++){
        row[0] = '\0';
        for(i=0; i < settings.width; i++){
            strcat(row, numToStr(*(arr + i + (j*settings.width))));
        }
        printf("%s\n",row);
    }
}

char getInput(){
    char input[4];
    fgets(input,1,stdin);
    switch (input[0]){

    case ']':
        settings.scale *= 1.1;
        break;
    case '[':
        settings.scale *= 0.9;
        break;
    default:
        return 0;
        break;
    }
    return 1;
}

void reCalcArr(unsigned char* arr){
    for(j=0; j < settings.height; j++){
        for(i=0; i < settings.width; i++){
            *(arr + i + (j*settings.width)) = getDivergeAt(
                (i-((double)settings.width/2.0)),
                (j-((double)settings.height/2.0))
            );
        }
    }
}

void displaySettings(){
    printf("W: %d H: %d\nr: %f , i: %f\niterates: %d, scale: %f\n",
        settings.width,
        settings.height,
        settings.rCenter,
        settings.iCenter,
        (int)settings.iterates,
        settings.scale);
}

int main(int argc, char **argv){
    if(argc < 2){
        printf("no width arg\n");
        return 1;
    }
    if(argc < 3){
        printf("no height arg\n");
        return 1;
    }

    settings.width = atoi(argv[1]);
    settings.height = atoi(argv[2]);

    if(argc >= 4){
        settings.scale = atof(argv[3]);
    }
    if(argc >= 5){
        settings.rCenter = atof(argv[4]);
    }
    if(argc >= 6){
        settings.iCenter = atof(argv[5]);
    }
    if(argc >= 7){
        settings.iterates = (float)atoi(argv[6]);
    }
    unsigned char *arr = (unsigned char *)malloc(settings.width * settings.height * sizeof(unsigned char));
    
    if(!arr){
        printf("memory allocation failed\n");
        return 1;
    }

    reCalcArr(arr);
    displayArr(arr);

   while (getInput()){
        reCalcArr(arr);
        displayArr(arr);
   }
   
    free(arr);
	return 0;
}