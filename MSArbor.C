/*--------------------------------------------------------------------------*/
/*--------------------------- File MSArbor.C -------------------------------*/
/*--------------------------------------------------------------------------*/
/*--                                                                      --*/
/*--  Solves the Minimal Spanning Arborescence problem.                   --*/
/*--                                                                      --*/
/*--                            VERSION 1.13                              --*/
/*--                	       09 - 08 - 2009			       	  --*/
/*--                                                                      --*/
/*--                          Implementation by:                          --*/
/*--                                                                      --*/
/*--                          Antonio Frangioni(*)                        --*/
/*--                          Daniele Pretolani(#)                        --*/
/*--                           Marisa Traini(#)                           --*/
/*--                                                                      --*/
/*--   (*)  Dipartimento di Informatica -  Universita' di Pisa            --*/
/*--   (#)  Dipartimento di Matematica  -  Universita' di Camerino        --*/
/*--                                                                      --*/
/*--   Copyright (C) 2004 - 2009 A. Frangioni, D. Pretolani               --*/
/*--                                                                      --*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------- IMPLEMENTATION -------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*---------------------------- MACROS --------------------------------------*/
/*--------------------------------------------------------------------------*/

/*----------------------------- LOG FILE -----------------------------------*/

#define LOG_MSA 0   // 0 = no log , 1 = debug

/*--------------------------------------------------------------------------*/
/*--------------------------- INCLUDES -------------------------------------*/
/*--------------------------------------------------------------------------*/

#include "MSArbor.h"

#if( LOG_MSA )
 #include <iostream>
#endif 

/*--------------------------------------------------------------------------*/
/*-------------------------------- USING -----------------------------------*/
/*--------------------------------------------------------------------------*/

using namespace MSA_di_unipi_it;

#if( LOG_MSA )
 using namespace std;
#endif 

/*--------------------------------------------------------------------------*/
/*----------------------------- CONSTANTS ----------------------------------*/
/*--------------------------------------------------------------------------*/

static MSArbor::cIndex SCAN = 0;
static MSArbor::cIndex LOOP = 1;
static MSArbor::cIndex RCON = 2;

/*--------------------------------------------------------------------------*/
/*--------------------- PUBLIC METHODS OF THE CLASS ------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*---------------------------- CONSTRUCTOR ---------------------------------*/
/*--------------------------------------------------------------------------*/
 
MSArbor::MSArbor( Index nds )
{
 // define dimensions - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 n = nds;
 root = n - 1;
 csize = root * n;
 gsize = 2 * n - 2;

 // allocate memory - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 rU     = new CNumber[ gsize ];
 c      = new CNumber[ csize ];
 P      = new Index[ n ];
 SHADOW = new Index[ n ];
 STACK  = new Index[ n ];
 ACTIVE = new Index[ gsize ];
 POS    = new Index[ gsize ];
 LABEL  = new Index[ gsize ];
 PARENT = new Index[ gsize ];
 ARCT   = new Index[ gsize ];
 ARCH   = new Index[ gsize ];
 LINE   = new Index[ gsize ];

 }  // end( MSArbor )

/*--------------------------------------------------------------------------*/
/*-------------------- METHODS FOR SOLVING THE PROBLEM ---------------------*/
/*--------------------------------------------------------------------------*/

MSArbor::FONumber MSArbor::Solve( cCRow C , CRow RC )
{
 m = root;

 CRow tc = c + csize;
 for( C += csize ; tc-- > c ; )
  *tc = *(--C);

 Index i = 0;
 for( ; i <= root ; ) {
  PARENT[ i ] = ARCT[ i ] = ARCH[ i ] = SHADOW[ i ] = P[ i ] = InINF; 
  LINE[ i ] = ACTIVE[ i ] = POS[ i ] = i;
  rU[ i ] = 0;
  LABEL[ i++ ] = 0;
  }

 for( ; i <= 2 * n - 3 ; ) {
  PARENT[ i ] = ARCT[ i ] = ARCH[ i ] = InINF;
  LINE[ i ] = i;
  rU[ i ] = 0;
  POS[ i ] = 0;
  LABEL[ i++ ] = 0;
  }

 Index Atop = root;
 Index Abot = 0;
 Index Ubot = 0;
 Index Stage = LABEL[ root ] = 1;
 Index UnLabeled = root;

 FONumber Z = 0;

 // Start of Phase 1 --------------------------------------------------------

 while( UnLabeled > 0 ) {
  Stage++;  // a new stage begins

  // find first unlabeled active node

  if( Ubot < Abot )  // some nodes have been deleted
   Ubot = Abot;      // note: Ubot is non-decreasing 

  while( LABEL[ ACTIVE[ Ubot ] ] )
   Ubot++;

  Index h = STACK[ 0 ] = ACTIVE[ Ubot ];
  Index Stop = 0;
  Index status;

  do {
   // process node v  at the top of the STACK

   Index v = STACK[ Stop ];
   LABEL[ v ] = Stage;

   if( v < root )
    UnLabeled--; 

   Index k1 = LINE[ v ];
   Index kmin , vmin , t;

   // find minimum arc ( t , v ) entering v

   CNumber lmin = C_INF;
   status = LOOP; 

   Index j = Abot;
   for( ; j <= Atop ; ) {
    // try arc ( i , v ): cost of ( i , v ) = c[ k1 , k2 ]

    i = ACTIVE[ j++ ];
    Index k2 = LINE[ i ];
    Index label2 = LABEL[ i ];

    CNumber ctv = c[ k1 * n + k2 ];

    if( ctv < lmin ) {  // a new min cost arc is found
     lmin = ctv; 
     kmin = k2; 
     t = i;

     if( label2 == Stage )
      status = LOOP;   // loop detected
     else
      if( label2 == 0 )
       status = SCAN;  // h unlabeled 
      else
       status = RCON;  // h r-linked  
      }
    else
     if( ctv == lmin )  // WARNING: some epsilon might be needed here
      if( ( label2 < Stage ) && ( label2 > 0 ) ) {
       // i is a (currently) minimum r-connected node
       // choose i ONLY IF v is not already connected to the root;
       // this gives a solution with MAXIMUM root degree

       if( t != root ) {
        kmin = k2;
        t = i;
        status = RCON;
        }
       }
      else
       if( ( status == LOOP ) && ( label2 == 0 ) ) {
        // i is a minimum unlabeled node
        kmin = k2;
        t = i;
        status = SCAN;
        }
    }  // end for( j ): processing active node i

   // now, ( t , v ) is the minimum arc entering v: LINE[ t ] = kmin;
   // lmin = cost( t , v ); find the corresponding original arc
   // ( Tail , Head )

   if( k1 == v )  // v is an original node
    ARCH[ v ] = v;
   else           // v is a shrunken component
    ARCH[ v ] = Index( c[ SHADOW[ k1 ] * n + kmin ] );

   if( t == kmin )  // t is an original node
    ARCT[ v ] = t;
   else             // t is a shrunken component
    ARCT[ v ] = Index( c[ k1 * n + SHADOW[ kmin ] ] );

   // update dual variables and objective function

   rU[ v ] = lmin;
   Z += FONumber( lmin );

   if( status == SCAN )  // node t unlabeled; PUSH it on stack 
    STACK[ ++Stop ] = t; 
   else
    if( status == LOOP ) {  // a loop is detected; shrink a new component 
     m++;  // new component name

     Index h1 = 0;

     while( STACK[ h1 ] != t )
      h1++;

     // now, the nodes in the component are STACK[ h1 ] , STACK[ h1 + 1 ] ,
     // ... , STACK[ Stop ]: remove them from ACTIVE set

     for( j = h1 ; j <= Stop ; ) {
      i = STACK[ j++ ];
      Index k = POS[ i ];

      if( i > root ) {
       ACTIVE[ k ] = ACTIVE[ Atop ];
       POS[ ACTIVE[ Atop-- ] ] = k;
       }
      else {
       ACTIVE[ k ] = ACTIVE[ Abot ];
       POS[ ACTIVE[ Abot++ ] ] = k;
       }

      PARENT[ i ] = m;

      }  // end for( j )

     // select new LINE and SHADOW: bottom and second nodes in STACK; 

     Index Lm = LINE[ STACK[ h1 ] ];
     Index Sm = LINE[ STACK[ h1 + 1 ] ];

     // - - - - - compute BS[ m ] - - - - - - - - - - - - - - - - - - - - - -

     for( j = Abot ; j <= Atop ; ) {
      // process tail node i

      i = ACTIVE[ j++ ];
      Index k2 = LINE[ i ];

      // process nodes in m

      lmin = C_INF;
      for( Index k = h1 ; k <= Stop ; ) {
       // try head node v; cost of ( i , v ) = c[ k1 , k2 ]

       v = STACK[ k++ ];
       k1 = LINE[ v ];

       CNumber civ_prime = c[ k1 * n + k2 ] - rU[ v ];

       if( civ_prime < lmin ) {  // a new min cost arc is found
        lmin = civ_prime; 
        vmin = v; 
        kmin = k1;
        }
       }  // end for( k )

      v = vmin;

      // now, ( i , v ) is the minimum arc from i to m; LINE[ v ] = kmin; 

      c[ Lm * n + k2 ] = lmin; 

      // store the corresponding original arc in SHADOW

      if( v == kmin )         // v is an original node
       c[ Sm * n + k2 ] = v;  // put v in ROW Sm[ k2 ]
      else                    // v is a shrunken component
       c[ Sm * n + k2 ] = c[ SHADOW[ kmin ] * n + k2 ];

      if( i != k2 ) {        // i is a shrunken component
       Index S2 = SHADOW[ k2 ];
       c[ Lm * n + S2 ] = c[ kmin * n + S2 ];
       } 

      // else i == k2 is an original node: do nothing

      }  // end for( j ): processing tail node i

     // - - - - - - - - compute FS[ m ] - - - - - - - - - - - - - - - - - - -

     j = Abot;
     if( j == root )
      j++;

     for( ; j <= Atop ; ( j == ( root - 1 ) ? j += 2 : j++ ) ) {
      // process head node i

      i = ACTIVE[ j ];
      Index k2 = LINE[ i ];

      // process nodes in m

      lmin = C_INF;
      for( Index k = h1 ; k <= Stop ; ) {
       // try tail node v; cost of ( v , i ) = c[ k2 , k1 ] 

       v = STACK[ k++ ];
       k1 = LINE[ v ];

       CNumber civ = c[ k2 * n + k1 ];

       if( civ < lmin ) {  // a new min cost arc is found
        lmin = civ; 
        vmin = v; 
        kmin = k1;
        }
       }  // end for( k )

      v = vmin;
      // now, ( v , i ) is the minimum arc from m to i: LINE[ v ] = kmin; 

      c[ k2 * n + Lm ] = lmin; 
      // store the corresponding original arc in SHADOW

      if( v == kmin )         // v is an original node
       c[ k2 * n + Sm ] = v;  // put it in COLUMN Sm[ k2 ]
      else                    // v is a shrunken component; put c[ k2 , S1 ]
                              // in COLUMN Sm[ k2 ]
       c[ k2 * n + Sm ] = c[ k2 * n + SHADOW[ kmin ] ];

      if( i != k2 ) {
       // i is a shrunken component; put c[ S2 , k1 ] in ROW S2[ Lm ]
       Index S2 = SHADOW[ k2 ];
       c[ S2 * n + Lm ] = c[ S2 * n + kmin ];
       }

      // else i == k2 is an original node: do nothing

      }  // end for( j ): processing active node i, computation of FS( m )

     // insert m in the stack and in the ACTIVE set

     STACK[ Stop = h1 ] = ACTIVE[ ++Atop ] = m;

     // update LINE and SHADOW for new component

     SHADOW[ LINE[ m ] = Lm ] = Sm;

     }  // end if( Status == LOOP ) - End of Shrinking

  } while( ( status == LOOP ) || ( status == SCAN ) );   // End of the Stage

 }  // end while( UnLabeled > 0 )

 // end of Phase 1 - Phase 2 begins -----------------------------------------

 Index j = 0;
 for( ; j <= m ; ) 
  POS[ j++ ] = 0;     // note: POS is re-used to replace "REMOVED"

 POS[ root ] = 1;

 for( Index Node = m + 1 ; Node-- ; )
  if( ! POS[ Node ] ) {
   Index Head = ARCH[ Node ];
   Index Tail = ARCT[ Node ];
   P[ Head ] = Tail;
   i = Head;

   while( i != Node ) {
    POS[ i ] = 1;
    i = PARENT[ i ];
    }
   }

 // End of Phase 2 - possibly, Phase 3 begins -------------------------------

 if( RC ) {
  // Compute reduced costs -- output on array RC with  n - 1 rows and n
  // columns (row == backward star, same format as the cost array).
  // For each arc ( i , j ), RC[ j , i ] gives the cost of ( i , j ) minus
  // the sum of the dual variables U( k ) for each r-cut k containing (i,j);
  // note that U( k ) is stored in U[ k ], where k is an index of a node in
  // the auxiliary tree, i.e. either an original node or a node representing
  // a Strongly Connected Component.
  //
  // ASSUMPTION: for each son k of the root of the auxiliary tree, it is
  //             PARENT[ k ] == InINF

  for( i = 0 ; i < 2 * n - 3 ; i++ ) {  // re-use arrays POS and ACTIVE
   ACTIVE[ i ] = InINF;
   POS[ i ] = 0;
   }

  // l1 points to the first entry in RC (arc ( 0 , 0 )) - - - - - - - - - - -

  CRow l1 = RC;  
  cCRow c1 = C;  
  for( Index Head = 0 ; Head < root ; Head++ )
   for( Index Tail = 0 ; Tail <= root ; Tail++ )
    if( ( Tail != Head ) && ( *c1 < C_INF ) ) {
     // compute sum of dual variables for arc ( Tail , Head ) - - - - - - - -

     // (1) stack onto ACTIVE the nodes in the path from the root to node
     //     Head in the auxiliary tree; mark nodes setting POS = 1

     Index k = 1;

     ACTIVE[ k ] = j = Head;
     POS[ j ] = k;

     while( PARENT[ j ] != InINF ) {
      j = PARENT[ j ];  // climb up the tree
      ACTIVE[ ++k ] = j;
      POS[ j ] = k;
      }

     // (2) search the common ancestor of Tail and Head

     i = Tail;

     while( ( PARENT[ i ] != InINF ) && ( ! POS[ i ] ) )
      i = PARENT[ i ];

     if( POS[ i ] )      // Tail and Head have common ancestor i != root
      j = POS[ i ] - 1;
     else                // the common ancestor of Tail and Head is the root
      j = k;

     // (3) output in RC[ Head , Tail ] the cost of ( Tail , Head ) minus
     // the sum of the dual values for the ancestors of Head that are  
     // proper descendant of the common ancestor

     FONumber RcHT = 0;
     for( ; j ; )
      RcHT += FONumber( rU[ ACTIVE[ j-- ] ] );

     *(l1++) = CNumber( FONumber( *(c1++) ) - RcHT );

     // (4) clear POS

     for( ; k ; )
      POS[ ACTIVE[ k-- ] ] = 0;

     }      // end if( process arc ( Tail , Head ) )
    else {  // either Tail == Head, or c( Tail , Head ) == C_INF
     c1++;             // skip cost
     *(l1++) = C_INF;  // and set reduced cost to C_INF
     }

  }  // end if( RC != NULL )

 // End of Phase 3 ----------------------------------------------------------

 #if( LOG_MSA )
  // correctness controls - - - - - - - - - - - - - - - - - - - - - - - - - -

  FONumber NewZ = 0;       

  for( i = 0 ; i < root ; i++ ) {
   j = P[ i ];
   Index h = i * ( n - 1 ) + j;
   NewZ += FONumber( lambda[ h ] );
   }

  for( i = 0 ; i < root ; i++ ) {
   Index k = 0;
   j = i;

   while( j != root ) {
    j = P[ j ];
    if( k++ > n )
     j = root;
    }

   if( k > n )
    cerr << "ERROR: cycle on node " << i << endl;
   }

  if( Z != NewZ )
   cerr << "ERROR in the objective calculation (OPT = " << Z
        << ", NewOPT = " << NewZ << ")" << endl;
 #endif

 return( Z );

 }  // end( Solve )

/*--------------------------------------------------------------------------*/
/*------------------------------ DESTRUCTOR --------------------------------*/
/*--------------------------------------------------------------------------*/

MSArbor::~MSArbor()
{
 delete[] LINE;
 delete[] ARCH;
 delete[] ARCT;
 delete[] PARENT;
 delete[] LABEL;
 delete[] POS;
 delete[] ACTIVE;
 delete[] STACK;
 delete[] SHADOW;
 delete[] P;
 delete[] c;
 delete[] rU;

 }  // end( ~MSArbor )

/*--------------------------------------------------------------------------*/
/*----------------------- End File MSArbor.C -------------------------------*/
/*--------------------------------------------------------------------------*/
