#ifndef _SMITH_WATERMAN_H_
#define _SMITH_WATERMAN_H_

#include "back_up_struct.h"
#include "sequences.h"
#include "substitution_matrix.h"
#include "similarity_algorithm_cpu.h"

namespace Algorithms
{
    namespace Sequential
    {
        class SmithWatermanLocal:public SimilarityAlgorithmCpu
        {
        public:
            SmithWatermanLocal() : SimilarityAlgorithmCpu(), gapEx2(2), gapEx2Len(10) { }
        protected:
            virtual void InitializeMatrices();
            virtual void FillMatrices();
            virtual void BackwardMoving();
            int maxX;
            int maxY;
            int maxVal;
            int gapEx2;
            int gapEx2Len;
        };
    }
}

#endif
