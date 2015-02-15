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
char* databasefile = NULL; //(char*)"data/3_667.fax";
char* queryfile = NULL; //(char*)"data/3_667.fax";
char* outfile = NULL;
char* algName = (char*)"sws";//Smith Waterman Score

char *HELP = (char*) "Parameters:\n"
" --query (-q),      path to a input query fasta file\n"
" --database (-d),   path to a input reference fasta file (if same then lower triangle is calculated)\n"
" --submatrix (-sm), e.g. data/substitution_matrices/BLOSUM62.txt\n"
" --gapopen (-go),   e.g. 10\n"
" --gapext (-ge),    e.g. 2\n"
" --gapext2len       Second affine gap length e.g. 10\n" 
" --gapext2          Second affing gap extension penalty e.g. 0\n"
" --help (-h),       print this help\n"
"\n";

bool readArguments(int argc, char** argv)
{
    char* argBuf;
    ArgumentsManager arguments(argc, argv);
    
    argBuf = (char*)arguments.getParam("help", "h");
    if (argBuf != NULL)
    {
        printf("%s\n", HELP);
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

    if(databasefile == NULL)
    {
        printf("%s\n", HELP);
        return false;
    }
    
    try
    {
        SubstitutionMatrix sm(submatrix);
        Sequences db(databasefile, &sm);
        db.load();
        db.sortSequences();

        SmithWatermanLocal swl[omp_get_max_threads()];

        if (queryfile == NULL || strcmp(databasefile, queryfile) == 0) {
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
