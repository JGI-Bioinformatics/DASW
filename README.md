# DASW
Double Affine Smith-Waterman on CPU

The software may be freely used for academic and research purposes. 

This implementation is based on the CPU reference C++ code in G-PAS 1.0
 http://gpualign.cs.put.poznan.pl/project-gpu-pairAlign.html
 
"Protein alignment algorithms with an efficient backtracking routine on multiple GPUs" 
Jacek Blazewicz, Wojciech Frohmberg, Michal Kierzynka, Erwin Pesch and Pawel Wojciechowski 
BMC Bioinformatics 2011, 12:181

Thanks to Jacek Blazewicz, Wojciech Frohmberg, Michal Kierzynka, Erwin Pesch and Pawel Wojciechowski 
for providing the original code.

The original smith-waterman code from G-PAS was modified to include a second affine gap 
penalty after a specified length.  And then the inner loop was somewhat optimized and
the outer loop was parallelized with OpenMP.

Parameters:
 --query (-q),      path to a input query fasta file
 --database (-d),   path to a input reference fasta file (if same then lower triangle is calculated)
 --submatrix (-sm), e.g. data/substitution_matrices/BLOSUM62.txt
 --gapopen (-go),   e.g. 10
 --gapext (-ge),    e.g. 2
 --gapext2len       Second affine gap length e.g. 10
 --gapext2          Second affing gap extension penalty e.g. 0
 --help (-h),       print this help

