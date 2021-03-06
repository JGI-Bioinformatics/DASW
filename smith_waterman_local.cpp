#include "smith_waterman_local.h"

using namespace Algorithms::Sequential;
using Data::Sequences;
using Data::SubstitutionMatrix;

void SmithWatermanLocal::InitializeMatrices()
{
    //printf("gapOpen %d, gapExt %d, gapExt2Len %d, gapEx2 %d\n", gapOp, gapEx, gapEx2Len, gapEx2);
    for (int i = 0; i < seq1Length + 1; i++)
    {
        A[i][0] = 0;
        E[i][0] = INT_MIN + gapEx; //jednak musi być bardzo niska wartość zamiast i*gapEx;
        F[i][0] = INT_MIN + gapEx; //(INT_MIN - gapEx) MINUS JEST CELOWY!!!
        B[i][0].backDirection = stop;
    }
    for (int j = 0; j < seq2Length + 1; j++)
    {
        A[0][j] = 0;
        E[0][j] = INT_MIN + gapEx;
        F[0][j] = INT_MIN + gapEx;
        B[0][j].backDirection = stop;
    }
}

void SmithWatermanLocal::FillMatrices()
{
    /*
     *   s e q 2
     * s
     * e
     * q
     * 1
     */
    //E - responsible for left direction
    //F - responsible for up   direction

    maxVal = INT_MIN;
    int Elen, Flen[seq2Length+1];
    for (int j = 1; j <= seq2Length; j++)
      Flen[j] = 0;
    printf("gapOpen %d, gapExt %d, gapExt2Len %d, gapExt2 %d\n", gapOp, gapEx, gapExt2Len, gapExt2);
    
    int *Ai = A[0], *Ai1, *Ei = E[0], *Ei1, *Fi = F[0], *Fi1;
    BackUpStruct *Bi = B[0], *Bi1;
    for (int i = 1; i <= seq1Length; i++)
    {
        Elen = 0;
        Ai1 = Ai;
        Ei1 = Ei;
        Fi1 = Fi;
        Bi1 = Bi;
        Ai = A[i];
        Ei = E[i];
        Fi = F[i];
        Bi = B[i];
        for (int j = 1; j <= seq2Length; j++)
        {
            int Aij, Eij, Fij;
            int e1 = Ei[j - 1] - ((Elen >= gapExt2Len) ? gapExt2 : gapEx), e2 = Ai[j - 1] - gapOp;;
            if (e1 >= e2) {
               Eij = e1;
               Bi[j - 1].continueLeft = 1;
               Elen++;
            } else {
               Eij = e2;
               Bi[j - 1].continueLeft = 0;
               Elen = 0;
            }

            int f1 = Fi1[j] - ((Flen[j] >= gapExt2Len) ? gapExt2 : gapEx), f2 = Ai1[j] - gapOp;
            if (f1 >= f2) {
                Fij = f1;
                Bi1[j].continueUp = 1;
                Flen[j]++;
            } else {
                Fij = f2;
                Bi1[j].continueUp = 0;
                Flen[j] = 0;
            }

            int a1 = Ai1[j - 1] + sm->getScore(seq1[i], seq2[j]);
            Aij = MAX(a1, 0);

            Aij = MAX3(Aij, Eij, Fij);
             
            if (Aij == 0)
                Bi[j].backDirection = stop; //SPECYFIC FOR SMITH WATERMAN
            else if(Aij == a1)
                Bi[j].backDirection = crosswise;
            else if(Aij == Eij)
                Bi[j].backDirection = left;
            else //if(A[i][j] == F[i][j])
                Bi[j].backDirection = up;

            if(Aij > maxVal)
            {
                maxX = j;
                maxY = i;
                maxVal = Aij;
            }
            Ai[j] = Aij;
            Ei[j] = Eij;
            Fi[j] = Fij;

        }
    }
}

void SmithWatermanLocal::BackwardMoving()
{
    //BACKWARD MOVING
    int carret = 0;

    int y = maxY;
    int x = maxX;
    score = maxVal;

    BackDirection prev = crosswise;
    while(B[y][x].backDirection != stop)
    {
        if (prev == up && B[y][x].continueUp) //CONTINUE GOING UP
        {                                          //GAP EXTENSION
            result1[carret] = sm->revConvert(seq1[y]);
            result2[carret] = '-';
            carret++;
            y--;
        }
        else if (prev == left && B[y][x].continueLeft) //CONTINUE GOING LEFT
        {                                         //GAP EXTENSION
            result1[carret] = '-';
            result2[carret] = sm->revConvert(seq2[x]);
            carret++;
            x--;
        }
        else
        {
            prev = B[y][x].backDirection;
            if(prev == up)
            {
                result1[carret] = sm->revConvert(seq1[y]);
                result2[carret] = '-';
                carret++;
                y--;
            }
            else if(prev == left)
            {
                result1[carret] = '-';
                result2[carret] = sm->revConvert(seq2[x]);
                carret++;
                x--;
            }
            else //prev == crosswise
            {
                result1[carret] = sm->revConvert(seq1[y]);
                result2[carret] = sm->revConvert(seq2[x]);
                carret++;
                x--;
                y--;
            }
        }
    }
    result1[carret] = 0;
    result2[carret] = 0;
}
