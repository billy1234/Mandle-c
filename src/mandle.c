#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define False (char)0;
#define True (char)1;

//cached variables
double zR;
double zI;
double cR;
double cI;
double temp;
float currIterates;

int i; 
int j;

unsigned char *arr;

char* screenBuffer;
char* row;
int buffWidth = -1;
int buffHeight = -1;

char resize = True;
char printSettings = False;

struct zoomSettings{
    int width;
    int height;
    float iterates;
    double scale;
    double rCenter;
    double iCenter;
};

struct zoomSettings settings = {
    100,100,
    500.0,
    0.1,
    -1.315180982097868,0.073481649996795
};
char gradient[6][4] = {
    " ",
    ".",
    "░",
    "▒",
    "▓",
    "█"
    }; 
const int gradientLen = 4; //final elem id of ^

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

int bufferResized(){
    return((settings.height != buffHeight) || (settings.width != buffWidth));
}

void initBuffers(int width, int height) {
    if(screenBuffer) free(screenBuffer);
    if(row) free(row);

    screenBuffer = malloc(((width *4) + 1) * height * sizeof(char));
    row = malloc((settings.width *4) + 1 * sizeof(char)); //*3 as extended ascii may take up that much space
    buffWidth = width;
    buffHeight = height;
}

void displayArr(unsigned char* arr){
    if(bufferResized()) initBuffers(settings.width,settings.height);
    
    screenBuffer[0] = '\0';
    for(j=0; j < settings.height; j++){
        row[0] = '\0';
        for(i=0; i < settings.width; i++){
            strcat(row, numToStr(*(arr + i + (j*settings.width))));
        }
        strcat(row,"\n");
        strcat(screenBuffer,row);
    }
    printf("%s",screenBuffer);
}

const char* trC = "╗"; //top right corner ect
const char* tlC = "╔";
const char* brC = "╝";
const char* blC = "╚";
const char* horiz = "═";
const char* vert = "║";
const char* filler = " ";

char* makeRow(int n,const char* left,const char* right,const char* filler){
    strcat(row,left);
    for(int i = 1; i < n -1; i++){
        strcat(row,filler);
    }
    strcat(row,right);
    return row;
}

void previewScreen(){
    if(bufferResized()) initBuffers(settings.width,settings.height);
    screenBuffer[0] = '\0';
    for(j=0; j < settings.height; j++){
        row[0] = '\0';
        if(!j) row = makeRow(settings.width,tlC,trC,horiz);
        else if(j == settings.height -1) row = makeRow(settings.width,blC,brC,horiz);
        else {
            strcat(row,vert);
            for(i=1; i < settings.width -1; i++) {
                strcat(row, numToStr(*(arr + i + (j*settings.width))));
            }
            strcat(row, vert);
        }
        strcat(row,"\n");
        strcat(screenBuffer,row);
    }
    printf("\n%s",screenBuffer);
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
    printf("W: %d H: %d\nr: %f , i: %f\niterates: %d, scale: %.15lf\n",
        settings.width,
        settings.height,
        settings.rCenter,
        settings.iCenter,
        (int)settings.iterates,
        settings.scale);
}

void freeMem(){
    if(arr) free(arr);
    if(row) free(row);
    if(screenBuffer) free(screenBuffer);
}

char getInput(){
    char input = getchar();
    switch (input){
    case '\n':
        settings.scale *= 0.9;
        break;
    case ']':
        settings.scale *= 0.9;
        break;
    case '[':
        settings.scale *= 1.1;
        break;
    case '?':
        displaySettings();
        break;
    case 'p':
        printSettings = !printSettings;
        break;
    case 'r':
        resize = 0;
        break;
    default:
        displaySettings();
        return 0;
        break;
    }
    return 1;
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
        if(argv[3][0] == 'f' ||(argv[3][0] == '-' && argv[3][1] == 'f')) resize = 0;
        else settings.scale = atof(argv[3]);
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
    arr = (unsigned char *)malloc(settings.width * settings.height * sizeof(unsigned char));
    
    if(!arr){
        printf("memory allocation failed\n");
        return 1;
    }

    reCalcArr(arr);
    previewScreen();

    while (getInput()){
        if(settings.scale == 0.0) break;
        reCalcArr(arr);
        if(resize) previewScreen();
        else displayArr(arr);
        if(printSettings) displaySettings();
   }
    freeMem();
	return 0;
}