/*--------------------------------------------------------------------------*/
/*----------------------------- File Main.C --------------------------------*/
/*--------------------------------------------------------------------------*/
/*--                                                                      --*/
/*-- Simple main() for testing various the MSArbor class.                 --*/
/*--                                                                      --*/
/*--                            VERSION 1.00                              --*/
/*--                           08 - 10 - 2004                             --*/
/*--                                                                      --*/
/*--                 Original Idea and Implementation by:                 --*/
/*--                                                                      --*/
/*--                          Antonio Frangioni                           --*/
/*--                                                                      --*/
/*--                       Operations Research Group                      --*/
/*--                      Dipartimento di Informatica                     --*/
/*--                          Universita' di Pisa                         --*/
/*--                                                                      --*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*------------------------------ INCLUDES ----------------------------------*/
/*--------------------------------------------------------------------------*/

#include "MSArbor.h"

#include <iostream>
#include <fstream>

/*--------------------------------------------------------------------------*/
/*-------------------------------- USING -----------------------------------*/
/*--------------------------------------------------------------------------*/

using namespace MSA_di_unipi_it;
using namespace std;

/*--------------------------------------------------------------------------*/
/*-------------------------------- main() ----------------------------------*/
/*--------------------------------------------------------------------------*/

int main( int argc , char **argv )
{
 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 // read data file and construct the MSArbor object - - - - - - - - - - - - -
 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 /* The format of the file is

    n = number of nodes: note that nodes are numbered from 0 to n - 1, and
        n - 1 is taken to be the root

    for( i = 0 ; i < n ; i++ )
     for( j = 0 ; j < n - 1 ; j++ )
      < cost of arc ( i , j ) >

    Note that arcs (i, n - 1), i.e., entering the root, will never be part
    of an optimal solution, so they are not even present in the file. */

 if( argc < 1 )
 {
  cerr << "Usage: MSArbor <file_name>" << endl;
  return( 1 );
  }

 // open the file - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 ifstream inFile( argv[ 1 ] );
 if( ! inFile )
 {
  cerr << "Error: cannot open file " << argv[ 1 ] << endl;
  return( 1 );
  }

 // read number of nodes- - - - - - - - - - - - - - - - - - - - - - - - - - -

 MSArbor::Index n;
 inFile >> n;

 // prepare & read costs- - - - - - - - - - - - - - - - - - - - - - - - - - -

 MSArbor::CRow csts = new MSArbor::CNumber[ n * ( n - 1 ) ];

 for( MSArbor::Index i = 0 ; i < n ; i++ )
 {
  register MSArbor::Index j = 0;
  for( ; j < i ; j++ )             // j < i
   inFile >> csts[ n * j + i ];

  if( i < n - 1 )                  // skip j == i and set c[i,i] = C_INF
  {                                // but only if i < n - 1
   MSArbor::CNumber foo;
   inFile >> foo;
   csts[ n * (j++) + i ] = MSArbor::C_INF;

   for( ; j < n - 1 ; j++ )        // i < j < n - 1
    inFile >> csts[ n * j + i ];
   }
  }  // end( for( i ) )

 // close file- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 inFile.close();

 // construct the MSArbor object- - - - - - - - - - - - - - - - - - - - - - -

 MSArbor MSA( n );

 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 // solve the problem and print out results - - - - - - - - - - - - - - - - -
 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 clog << MSA.GetN() << "\t" << MSA.Solve( csts ) << endl;

 for( int i = 0 ; i < n - 1 ; i++ )
  clog << MSA.ReadPred()[ i ] << " ";

 clog << endl;

 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 // clean up- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 delete[] csts;

 return( 0 );

 }  // end( main )

/*--------------------------------------------------------------------------*/
/*-------------------------- End File Main.C -------------------------------*/
/*--------------------------------------------------------------------------*/
