Readme file for Version 1.13 of MSArbor, a C++ implementation of the ARBOR
algorithm [M. Fischetti and P.Toth, ORSA J. on Computing 5(4), 1993] for the
Minimal Spanning Arborescence problem on complete graphs.

This distribution comes with the following files:

- doc/refman.pdf: Short manual for MSArbor

- doxygen/:   input files for producing the manual

- MSArbor.h:  Header file for MSArbor

- MSArbor.C:  Implementation of MSArbor

- Main.C:     Sample Main

- makefile:   makefile

- msa15:      File containing an example of MSA instance (with 15 nodes)

- Readme.md:  This file

- License.md: License information

The code is pretty much pure C++ with no bells and whistles, so it should
compile straight away on most systems. It has been primarily tested on g++.
Testing it on your system involves the following steps:

- edit MSArbor.h and check the "PUBLIC TYPES" section for relevant type
  definitions and the corresponding constants;

- edit the makefile for compiler name, switches and so on;

- type "make" in the directory to build the executable;

- type "MSArbor msa15" to compute the optimal solution of the instance
  described in the file msa15: the output should be
  
  15	  290
  5 9 6 11 14 3 9 6 7 0 0 4 10 2 
  
  indicating that the instance has 15 nodes, that the optimal spanning
  arborescence with root 14 is
  
  14 --> 4 --> 11 --> 3 --> 5 --> 0 -->  9 --> 1
                                  |      |
                                  |      ----> 6 --> 2 --> 13
                                  |            |
                                  |            ----> 7 -->  8
                                  |
                                  ----> 10 --> 12
  
  and that its cost is 290.
  (tip: if you don't see the tree, switch to "raw" display)

This program comes with NO WARRANTY at all, as described in the license file.
Nor we can guarantee you any support in case you find bugs or other problems.
That does not mean that we won't try, if we can, so for every
comment/request/bug report please contact

     	    frangio@di.unipi.it.

Good luck.
