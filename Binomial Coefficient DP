#include <Rcpp.h>
#include <map>
#include <algorithm>

using namespace Rcpp;
using namespace std;

///////////////////////////////////////
// Skeleton code for Homework 3A
///////////////////////////////////////

// function to calculate factorial
int fac(int n) {
  if ( n < 2 ) return 1; // return 1 when n=0,1
  int ret = 1;
  for(int i=2; i <= n; ++i)
    ret *= i;  // calculate factorial
  return ret;
}

// compute choose(n,k) using factorial
// [[Rcpp::export]]
int choose_fac(int n, int k) {
  return fac(n)/fac(k)/fac(n-k);
}




// compute choose(n,k) using dynamic programming
// [[Rcpp::export]]
int choose_dp(int n, int k) {
    /*we will use the logic of the recursive rule
     * that professor provide us in the assignment
     */
    int the_coeff[n+1][k+1]; /*will hold the values
                                of the binomial coeff.Here is the dynamic programming
                              part*/
    
    int i,j; /* for the itterations*/
    
    for(i=0 ; i<=n ; i++) /* we will use double loop to construct 
      the dynamic programming binomial coeff. calculation*/
    {
      for(j=0; j<= i && j <=k ; j++)
      {
        if(j==0 || j==i) /*second condition of the recursive rule the professor gave us*/
                        /* C(n,0)=C(n,n)=1*/
          the_coeff[i][j] = 1; 
        
        else /* and here we implement the first recursive rule condition*/
            /* C(n,k) = C(n-1,k)+C(n-1,k-1)*/
          the_coeff[i][j] = the_coeff[i-1][j-1]+the_coeff[i-1][j];
      }     
    }
    
  return the_coeff[n][k]; /* finally we return the coefficient*/
  
}
