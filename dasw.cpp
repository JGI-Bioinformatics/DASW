#include <omp.h>
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
char gapExt2Len = 10;
char gapExt2 = 0;
char* submatrix = (char*)"data/substitution_matrices/EBLOSUM30";
char* databasefile = (char*)"data/3_667.fax";
char* queryfile = (char*)"data/3_667.fax";
char* outfile = NULL;
char* algName = (char*)"sws";//Smith Waterman Score

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

    argBuf = (char*)arguments.getParam("database", "d");
    if( (argBuf != NULL) && (strcmp(argBuf,"true")) )
        databasefile = strdup(argBuf);

    argBuf = (char*)arguments.getParam("query", "q");
    if( (argBuf != NULL) && (strcmp(argBuf,"true")) )
        queryfile = strdup(argBuf);

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

    argBuf = (char*)arguments.getParam("gapext2len", "ge2l");
    if ((argBuf != NULL) && (atoi(argBuf)>0))
        gapExt2Len = atoi(argBuf);

    argBuf = (char*)arguments.getParam("gapext2", "ge2");
    if ((argBuf != NULL) && (atoi(argBuf)>0))
        gapExt2 = atoi(argBuf);

    return true;
    
}

int main(int argc, char** argv)
{
    
    if(!readArguments(argc, argv))
        return 1;
    
    try
    {
        SubstitutionMatrix sm(submatrix);
        Sequences db(databasefile, &sm);
        db.load();
        db.sortSequences();

        SmithWatermanLocal swl[omp_get_max_threads()];

        if (strcmp(databasefile, queryfile) == 0) {
            printf("query and database are the same, calculating lower triangle of database\n");

          #pragma omp parallel for schedule(dynamic, 1)
          for(int i = 0; i < db.getSequenceNumber(); i++) {
            for(int j = 0; j <= i; j++) {
              swl[omp_get_thread_num()].Run(&db, i, j, gapOpen, gapExtension, gapExt2Len, gapExt2);
            }
          }
        } else {

          Sequences query(queryfile, &sm);
          query.load();
          query.sortSequences();

          #pragma omp parallel for schedule(dynamic, 1)
          for(int i = 0; i < query.getSequenceNumber(); i++) {
             for(int j = 0; j < db.getSequenceNumber(); j++) {
               swl[omp_get_thread_num()].RunSeqs(&query, i, &db, j, gapOpen, gapExtension, gapExt2Len, gapExt2);
             }
          }
       }

    }
    catch (Exception* ex)
    {
        printf("%s\n",ex->getMessage());
        return 1;
    }
    return 0;
}
