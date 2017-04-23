/*--------------------------------------------------------------------------*/
/*-------------------------- File MSArbor.h --------------------------------*/
/*--------------------------------------------------------------------------*/
/** @file
 * Solves the Minimal Spanning Arborescence problem, with a C++
 * implementation of the ARBOR algorithm [M.Fischetti and P.Toth,
 * ORSA J.on Computing 5(4), 1993] for MSA on complete graphs.
 *
 * \version 1.13
 *
 * \date 01 - 09 - 2009
 *
 * \author Antonio Frangioni \n
 *         Operations Research Group \n
 *         Dipartimento di Informatica \n
 *         Universita' di Pisa \n
 *
 * \author Daniele Pretolani \n
 *         Dipartimento di Matematica \n
 *         Universita' di Camerino \n
 *
 * \author Marisa Traini \n
 *         Dipartimento di Matematica \n
 *         Universita' di Camerino \n
 *
 * Copyright &copy 2004 - 2009 A. Frangioni, D. Pretolani
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 * or check www.gnu.org.
 */

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*----------------------------- DEFINITIONS --------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

#ifndef __MSArbor
 #define __MSArbor  /* self-identification - #endif at the end of the file */

/*--------------------------------------------------------------------------*/
/*------------------------- NAMESPACE and USING ----------------------------*/
/*--------------------------------------------------------------------------*/

namespace MSA_di_unipi_it
{
 /** @namespace MSA_di_unipi_it
     The namespace MSA_di_unipi_it is defined to hold the MSArbor class. */

/*--------------------------------------------------------------------------*/
/*--------------------------- CLASS MSArbor --------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------- GENERAL NOTES --------------------------------*/
/*--------------------------------------------------------------------------*/
/** Given a (complete) directed graph G = ( N , A ), with n = |N| nodes and
    m = |A| (= n ( n - 1 )) arcs, and arc costs C[ i , j ], the Minimal
    Spanning Arborescence problem (MSA) with root r requires to find a
    directed spanning tree (arborescence) of G rooted in r with minimal total
    cost, where the cost is the sum of the costs of the arcs belonging to the
    tree.

    This class solves MSA for r = n - 1 (the node with largest index) with a
    C++ implementation of the ARBOR algorithm [M. Fischetti and P.Toth,
    ORSA J. on Computing 5(4), 1993] for complete graphs.

    Nodes and arc indices are defined to be of the public type
    MSArbor::Index, and arc costs are defined to be of the public type
    MSArbor::CNumber. These types, with the accompanying constants InINF and
    C_INF, are defined in the public types part of the class. By changing
    these definitions one can a) solve compilation problems due to different
    type or constant names in differend compilers, and b) use "small" data
    types, just large enough to fit the numbers in his/her instances, thereby
    (possibly) reducing the memory footprint of the object and increasing its
    efficiency. */

class MSArbor
{

/*--------------------------------------------------------------------------*/
/*----------------------- PUBLIC PART OF THE CLASS -------------------------*/
/*--------------------------------------------------------------------------*/

 public:

/*--------------------------------------------------------------------------*/
/*---------------------------- PUBLIC TYPES --------------------------------*/
/*--------------------------------------------------------------------------*/

  typedef unsigned short  Index;          ///< arc or node index ( >= 0 )
  typedef Index          *Index_Set;      ///< set (array) of indices
  typedef const Index    cIndex;          ///< a read-only Index
  typedef cIndex        *cIndex_Set;      ///< read-only array
  static cIndex InINF = 65535;            ///< the largest Index

  typedef short           CNumber;        /**< type of arc costs: since the
					     cost matrix is re-used to store
					     node names, it not should be
					     (too) "smaller" than Index */
  typedef CNumber        *CRow;           ///< vector of costs
  typedef const CNumber  cCNumber;        ///< a read-only cost
  typedef cCNumber      *cCRow;           ///< read-only cost array
  static cCNumber C_INF = 32767;          /**< the largest arc cost is
                                             C_INF - 2, as C_INF - 1 means
                                             "no arc is here" and C_INF is
                                             reserved */

  typedef int             FONumber;       /**< type of objective function
					     values; should be able to hold
					     something like (max arc cost)
					     times (max number of nodes) */

/*--------------------------------------------------------------------------*/
/*--------------------- PUBLIC METHODS OF THE CLASS ------------------------*/
/*--------------------------------------------------------------------------*/
/*---------------------------- CONSTRUCTOR ---------------------------------*/
/*--------------------------------------------------------------------------*/

   MSArbor( Index nds );

/**< Constructor of the class: takes as parameter the number of nodes in the
   graph G. The actual graph is passed in Solve() [see below], and different
   instances can be solved by calling Solve() multiple times on the same
   MSArbor object, as long as al the instances have the same number of nodes.
   */

/*--------------------------------------------------------------------------*/
/*-------------------- METHODS FOR SOLVING THE PROBLEM ---------------------*/
/*--------------------------------------------------------------------------*/

   FONumber Solve( cCRow C , CRow RC = 0 );

/**< This method solves the Minimum Spanning Arborescence problem on the
   complete graph described in the vector C[], which is a vector of arc
   costs, ordered as follows:

   - a sequence of n - 1 Backward Stars: BS[ 0 ], BS[ 1 ], .., BS[ n - 2 ];

   - arcs in each BS[ i ] are ordered in increasing index of tail node;

   Since nodes are numbered from 0 to n - 1, and the root is node n - 1,
   C[ i + n * j ] is the cost of ( i , j ). BS[ root ] does not exist.

   Although the algorithm is developed with complete graphs in mind, it is
   possible to try solve non-complete instances by giving not-existent arcs
   the "plus infinity" cost C_INF - 1. If the costs of all other nodes are
   suitably smaller than that, then the optimal solution will avoid these
   arcs. However, if there is *no* feasible solution *not* using at least
   one arc with cost C_INF - 1, then the best possible unfeasible solution
   will be reported instead with no warning, so it is the user's
   responsibility to check feasibility if that is in doubt. Also, note that
   arcs should *never* be given cost C_INF, since that value is reserved for
   special use by the code.

   If RC != 0 (= NULL), after that the Minimum Spanning Arborescence has been
   found its optimal arc reduced costs are written in RC, in the same format
   as the arc cost vector C.

   \warning the current implementation of the reduced cost computation is a
            "naive" one, which has an O(n^3) worst case complexity; a smarter 
            O(n^2) implementation is possible, but it is not implemented as 
            yet. Note that the MSA computation has an O(n^2) cost. */

/*--------------------------------------------------------------------------*/
/*------------------------ METHODS FOR READING RESULTS ---------------------*/
/*--------------------------------------------------------------------------*/

   inline cIndex_Set ReadPred( void ) const;

/**< Returns a read-only vector describing the primal optimal solution of the
   problem, i.e., the "predecessor" function of the optimal MSA.

   For each node i = 0, ..., n - 2, j = ReadPred()[ i ] is the predecessor of
   i in the spanning tree, and therefore arc (i, j) belongs to the optimal
   solution. The root has no predecessor, hence the entry n - 1 of the
   returned vector should not be checked. */

/*--------------------------------------------------------------------------*/

   inline Index GetM( void ) const;

   inline cIndex_Set ReadAux( void ) const;

   inline cCRow GetU( void ) const;

/**< These three methods describe the dual optimal solution of the problem
   (a good knowledge of the algorithm is required for understending their
   meaning).

   The dual optimal solution is described in terms of at most 2 n - 2 sets
   of nodes, arranged in a tree structure (called the "auxiliary tree") and
   with a dual variable attached to each. GetM() and ReadAux() describe the
   topology of the auxiliary tree: GetM() returns the number of its nodes,
   while ReadAux()[ i ] is the predecessor function of the tree. Note that 
   the tree does not include a dummy root node, thus the sons of the dummy 
   root have predecessor  InINF. Finally, GetU()[ i ] is the value of the
   optimal dual variable associated with the set (node of the auxiliary
   tree) i. */

/*--------------------------------------------------------------------------*/
/*-------------- METHODS FOR READING THE DATA OF THE PROBLEM ---------------*/
/*--------------------------------------------------------------------------*/

   inline Index GetN( void ) const;

/**< Returns the size of the problem (number of nodes). */

/*--------------------------------------------------------------------------*/
/*------------------------------ DESTRUCTOR --------------------------------*/
/*--------------------------------------------------------------------------*/

   ~MSArbor();

/*--------------------------------------------------------------------------*/
/*-------------------- PROTECTED PART OF THE CLASS -------------------------*/
/*--------------------------------------------------------------------------*/

 protected:

/*--------------------------------------------------------------------------*/
/*--------------------- PROTECTED DATA STRUCTURES --------------------------*/
/*--------------------------------------------------------------------------*/

  Index  n;          // Total number of nodes of the graph
  Index  m;          // size of the dual tree (<= 2 n - 2)
  Index  root;       // n - 1          
  Index  csize;      // ( n - 1 ) * n : size of the cost array
  Index  gsize;      // 2 * n - 3 : max number of nodes in the auxiliary tree,
                     // i.e. original nodes plus supernodes representing 
                     // strongly connected components

  Index_Set P;       // n array, predecessor function of the optimal tree

  CRow c;            // ( n - 1 ) * n  matrix: initially, row j gives the
                     // costs of arcs in BS( j ), c( i , i ) == +INF, and the
                     // root has no BS

  CRow rU;           // ( 2 n - 2 ) array. rU[ j ] == dual variable associated
                     // with node j

  Index_Set LABEL;   // n array. LABEL[ j ] == stage where node j is labeled
  Index_Set PARENT;  // ( 2 n - 2 ) array. PARENT[ j ] == predecessor of j in
                     // the auxiliary tree
  Index_Set ARCT;    // ( 2 n - 2 ) arrays: ARC*[ j ] == Tail and Head of
  Index_Set ARCH;    // original arc entering node j (when j is visited)
  Index_Set LINE;    // ( 2 n - 2 ) array. LINE[ j ] == (cost) line index
                     // associated with node j
  Index_Set SHADOW;  // n array: SHADOW[ h ] == (arc) line index associated
                     // with node j with LINE[ j ] == h
  Index_Set STACK;   // ( n - 1 ) array: stack of labeled nodes
  Index_Set ACTIVE;  // ( 2 n - 2 ) array: set of nodes in the current graph;
                     // original nodes are in cells [ Abot .. n - 1 ];
                     // shrunken nodes in cells [ n .. Atop ]
  Index_Set POS;     // ( 2 n - 2 ) array. POS[ j ] == position of node j in
                     // ACTIVE; if j is not in the current graph, then
                     // POS[ j ] == UNDEFINED

/*--------------------------------------------------------------------------*/

 };  // end( class MSArbor )

/*--------------------------------------------------------------------------*/
/*------------------- inline methods implementation ------------------------*/
/*--------------------------------------------------------------------------*/

inline MSArbor::cIndex_Set MSArbor::ReadPred( void ) const
{
 return( P );
 }

/*--------------------------------------------------------------------------*/

inline MSArbor::cIndex_Set MSArbor::ReadAux( void ) const
{
 return( PARENT );
 }

/*--------------------------------------------------------------------------*/

inline MSArbor::Index MSArbor::GetM( void ) const
{
 return( m );
 }

/*--------------------------------------------------------------------------*/

inline MSArbor::cCRow MSArbor::GetU( void ) const
{
 return ( rU );
 }

/*--------------------------------------------------------------------------*/

inline MSArbor::Index MSArbor::GetN( void ) const
{
 return( n );
 }

/*--------------------------------------------------------------------------*/

 };  // end( namespace MSA_di_unipi_it )

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

#endif  /* MSArbor included */

/*--------------------------------------------------------------------------*/
/*----------------------- End File MSArbor.h -------------------------------*/
/*--------------------------------------------------------------------------*/
