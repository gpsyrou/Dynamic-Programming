
/*Homework 3 / Biostat 615 / Georgios Spyrou*/

#include <Rcpp.h>
#include <iostream>
#include <string>

using namespace Rcpp;
using namespace std;

/*we will construct a dynamic programmic algorithm for the edit distance problem*/

/* we are going to follow the logic of the lecture notes and try to implement
* the edit distance problem
*/
int editDistance(string& s1, string& s2, IntegerMatrix& cost, IntegerMatrix& move, int r, int c, int mcost, int icost) {
  
  if ( cost(r,c) < 0 ) {
    if ( r == 0 ) {
      if ( c == 0 ) cost(r,c) = 0;
      else {
        cost(r,c) = editDistance(s1, s2, cost, move, r, c-1,mcost,icost) + icost;
        move(r,c) = 1; /* number 1  - insert*/
      }
    }
    else if ( c == 0 ) {
      cost(r,c) = editDistance(s1, s2, cost,move, r-1, c,mcost,icost) + icost;
      move(r,c) = 2; /* number 2  - delete*/
    }
    else { /*this is if both r,c > 0 */
      int iDist = editDistance(s1, s2, cost,move, r, c-1,mcost,icost) + icost;
      int dDist = editDistance(s1, s2, cost,move, r-1, c,mcost,icost) + icost;
      int mDist = editDistance(s1,s2,cost,move,r-1,c-1,mcost,icost)+((s1[r-1]!=s2[c-1])*mcost);
      
      if ( iDist < dDist ) {
        move(r,c) = ( iDist < mDist) ? 1:3; cost(r,c) = ( iDist < mDist) ? iDist:mDist;
        /* if iDist the less, then insert (will be defined by number 1)*/
        /* if mDist the less, then we have missmatch( number 3 for this)*/
        
      }
      else {
        move(r,c) = ( dDist < mDist) ? 2:3; cost(r,c) = ( dDist < mDist) ? dDist:mDist;
        /* finally if dDist less, then deletion ( number 2 representing this)*/
        /* already defined above*/
      }
    }
  }
  return cost(r,c); /* returning the optimal cost for the edit distance*/
}


/* now we will implement the alignWords function
* and try to find if we can construct the results asked
* at homework ( edit distance + allignment at the words)
*/

// [[Rcpp::export]]
List alignWords(string s1, string s2, int mcost, int icost) {
  int r = (int)s1.size();
  int c = (int)s2.size(); /*defining the sizes of the two words*/

  string first_word, second_word, the_allignment; /*we have to define 3 strings that will represent the two "changed" words
  (i.e. with the '-' in them) and also the allignment*/
  
  IntegerMatrix cost(r+1,c+1); /* we define the cost matrix and fill it with -1 everywhere at the beginning*/
  fill(cost.begin(), cost.end(), -1);
  
  IntegerMatrix move(r+1, c+1);
  fill(move.begin(), move.end(), -1); /*same for the move matrix, we start with negaritive(-1) values , as asked
  from the proffesor*/
  
  /* hence now we can call the function we construct previously in the problem*/
  double distance = editDistance(s1, s2, cost, move, r, c, mcost, icost);
  
  /* thus , now we can reconstruct the words with the proper modifications*/
  while( (r >= 0 && c >= 0) && (move(r,c) >= 0)) {
    if(move(r,c) == 1) { /*we will add '-' if needed and also add 'I' if ins*/
  first_word = "-" + first_word; second_word = s2[c-1] + second_word;
  the_allignment = "I" + the_allignment;
  --c;
    }
    else if (move(r,c) == 2) {
      first_word = s1[r-1] + first_word; second_word = "-" + second_word; /*we will add '-' if needed and also add 'D' if del*/
  the_allignment = "D" + the_allignment;
  --r;
    }
    else if (move(r,c) == 3) {
      first_word = s1[r-1] + first_word;
      second_word = s2[c-1] + second_word; /* we add "." if we had a match and "*" if mismatch*/
  the_allignment = (s1[r-1] == s2[c-1] ? ".": "*") + the_allignment;
  --c;--r;
    }
  }
  
  /*finally we have to return the results as a Rcpp List as requested*/
  return List::create(Named("cost") = distance,
                      Named("first") = first_word,
                      Named("alignment") = the_allignment, 
                      Named("second") = second_word);


} /* end of question C for HW3*/







