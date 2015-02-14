#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "sequences.h"
#include "substitution_matrix.h"
#include "smith_waterman_local.h"
#include "arguments_manager.h"

using Exceptions::Exception;
using namespace Data;
using namespace Algorithms::Sequential;
using std::map;
using namespace std;

//GLOBAL DATA
char gapOpen = 5;
char gapExtension = 2;
char* submatrix = (char*)"data/substitution_matrices/EBLOSUM30";
char* infile = (char*)"data/3_667.fax";
char* outfile = NULL;
char* algName = (char*)"sws";//Smith Waterman Score
int seqX = -1;
int seqY = -1;

bool readArguments(int argc, char** argv)
{
    char* argBuf;
    ArgumentsManager arguments(argc, argv);
    
    argBuf = (char*)arguments.getParam("help", "h");
    if (argBuf != NULL)
    {
        ifstream fin("HELP");
        string temp;
        while(getline(fin, temp))
            cout << temp << endl;
        fin.close();
        return false;
    }

    argBuf = (char*)arguments.getParam("infile", "i");
    if( (argBuf != NULL) && (strcmp(argBuf,"true")) )
        infile = strdup(argBuf);

    argBuf = (char*)arguments.getParam("outfile", "o");
    if( (argBuf != NULL) && (strcmp(argBuf,"true")) )
        outfile = strdup(argBuf);

    argBuf = (char*)arguments.getParam("submatrix", "sm");
    if( (argBuf != NULL) && (strcmp(argBuf,"true")) )
        submatrix = strdup(argBuf);
    
    argBuf = (char*)arguments.getParam("gapopen", "go");
    if ((argBuf != NULL) && (atoi(argBuf)>0))
        gapOpen = atoi(argBuf);

    argBuf = (char*)arguments.getParam("gapext", "ge");
    if ((argBuf != NULL) && (atoi(argBuf)>0))
        gapExtension = atoi(argBuf);

    argBuf = (char*)arguments.getParam("seqX", "x");
    if ((argBuf != NULL) && (atoi(argBuf) > 0))
        seqX = atoi(argBuf) - 1;

    argBuf = (char*)arguments.getParam("seqY", "y");
    if ((argBuf != NULL) && (atoi(argBuf) > 0))
        seqY = atoi(argBuf) - 1;

    return true;
    
}

int main(int argc, char** argv)
{
    
    if(!readArguments(argc, argv))
        return 1;
    
    try
    {
//        SubstitutionMatrix sm(submatrix);
//        Sequences s(infile, &sm);
//        s.load();
//        s.sortSequences();

        //Homo_sapiens.GRCh37.55.pep.all.fa
        SubstitutionMatrix sm(submatrix);
        Sequences s(infile, &sm);
        s.load();
        s.sortSequences();

//        s.writeToFile("data/5_xxx.fax",100,420);
        
//        char fileName[512];
//        for(int i=16; i<1025; i+=16)
//        {
//            sprintf(fileName, "data/test/16-1024/5_%d.fax", i);
//            s.writeToFile(fileName, i);
//        }

//        char fileName[512];
//        for(int i=0; i<6; i++)
//        {
//            sprintf(fileName, "data/test/5_4000_%d.fax", i);
//            s.writeToFile(fileName, 4000);
//        }



//        s.writeToFile("data/3_xxx.fax", 100, 420);

        SmithWatermanLocal swl;
        swl.Run(&s, 234, 660, gapOpen, gapExtension);
        swl.PrintResults("results/2.txt");

      
    }
    catch (Exception* ex)
    {
        printf("%s\n",ex->getMessage());
        return 1;
    }
    return 0;
}
