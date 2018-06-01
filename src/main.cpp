//CS780-37 Project4 Distance Transform Algm.
//Shawn Yang

#include <iostream>
#include <fstream>
using namespace std;

class DistanceTransform {
public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newMinVal;
    int newMaxVal;
    int ** zeroFramdAry;
    int NeighborAry[5];
    ifstream *infile;
    ofstream *outfile1;
    ofstream *outfile2;
    
    DistanceTransform(ifstream * in_infile, ofstream *in_outfile1, ofstream *in_outfile2){
        infile=in_infile;
        outfile1=in_outfile1;
        outfile2=in_outfile2;
        //step0
        *infile >> numRows >> numCols >>minVal >>maxVal;
        zeroFramdAry = new int* [numRows+2];
        for (int i=0; i<numRows+2; i++){
            zeroFramdAry[i]=new int[numCols+2];
            //step1
            for (int j=0; j<numCols+2;j++)
                zeroFramdAry[i][j]=0;
        }
    }
    void run (){
        //todo
        loadImage();
        *outfile2<<"First pass:"<<endl;
        firstPassDistance();
        prettyPrint(outfile2);
        *outfile2<<"Second pass:"<<endl;
        secondPassDistance();
        prettyPrint(outfile2);
        *outfile1<<numRows<<" "<<numCols<<" "<<newMinVal<<" "<<newMaxVal<<endl;
        printOutputImage(outfile1);
        
    }
    void loadImage(){
        for (int r=1; r<numRows+1; r++){
            for (int c=1; c<numCols+1;c++){
                *infile >> zeroFramdAry[r][c];
            }
        }
    }
    void loadNeighbors(int r, int c, int selector){//selector 1:for 1st pass, 2 for 2nd pass
        if (selector==1){
            NeighborAry[0]=zeroFramdAry[r][c];
            NeighborAry[1]=zeroFramdAry[r-1][c-1];
            NeighborAry[2]=zeroFramdAry[r-1][c];
            NeighborAry[3]=zeroFramdAry[r-1][c+1];
            NeighborAry[4]=zeroFramdAry[r][c-1];
        }
        else if (selector==2){
            NeighborAry[0]=zeroFramdAry[r][c];
            NeighborAry[1]=zeroFramdAry[r][c+1];
            NeighborAry[2]=zeroFramdAry[r+1][c-1];
            NeighborAry[3]=zeroFramdAry[r+1][c];
            NeighborAry[4]=zeroFramdAry[r+1][c+1];
        }
        else
            exit(1);
    }
    void firstPassDistance(){
        for (int r=1; r<numRows+1; r++){
            for (int c=1; c<numCols+1; c++){
                if (zeroFramdAry[r][c]>0){
                    loadNeighbors(r, c, 1);
                    zeroFramdAry[r][c]=minOfNeighbors()+1;
                }
            }
        }
    }
    void secondPassDistance(){
        newMinVal=zeroFramdAry[1][1];
        newMaxVal=zeroFramdAry[1][1];
        for (int r=numRows; r>0; r--){
            for (int c=numCols; c>0; c--){
                if (zeroFramdAry[r][c]>0){
                    loadNeighbors(r, c, 2);
                    zeroFramdAry[r][c]=minOfTwoNumber(NeighborAry[0], (minOfNeighbors()+1));
                }
                if (zeroFramdAry[r][c]<newMinVal)
                    newMinVal=zeroFramdAry[r][c];
                if (zeroFramdAry[r][c]>newMaxVal)
                    newMaxVal=zeroFramdAry[r][c];
            }
        }
    }
    void prettyPrint(ofstream *outfile){
        for (int r=0; r<numRows+2; r++){
            for (int c=0; c<numCols+2; c++){
                if (zeroFramdAry[r][c]==0)
                    *outfile<<"  ";
                else
                    *outfile << zeroFramdAry[r][c] << " ";
            }
            *outfile<< endl;
        }
    }
    void printOutputImage(ofstream* outfile){
        for (int r=1; r< numRows+1; r++){
            for (int c=1; c<numCols+1; c++){
                *outfile <<zeroFramdAry[r][c]<<" ";
            }
            *outfile<<endl;
        }
    }
    int minOfNeighbors (){
        int index = 1;
        for (int i=1; i<5; i++){
            if (NeighborAry[index]>NeighborAry[i])
                index=i;
        }
        return NeighborAry[index];
    }
    int minOfTwoNumber(int a, int b){
        if (a>b)
            return b;
        else return a;
    }
    
};

int main(int argc, const char * argv[]) {
    ifstream *infile = new ifstream;
    ofstream *outfile1 = new ofstream;
    ofstream *outfile2 = new ofstream;
    
    if (argc<4) {
        cout<< "Please provide enough 1 input and 2 output files."<<endl;
        return 1;
    }
    
    try {
        infile->open(argv[1]);
        outfile1->open(argv[2]);
        outfile2->open(argv[3]);
    } catch (exception *e) {
        cout<< "Problem opening files."<<endl;
        return 1;
    }
    
    DistanceTransform *dt = new DistanceTransform(infile, outfile1, outfile2);
    dt->run();
    
    infile->close();
    outfile1->close();
    outfile2->close();
    
    return 0;
}
