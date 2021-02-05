
#include <Rcpp.h>
#include <cmath>
#include <algorithm>

using namespace Rcpp;
using namespace std;

// [[Rcpp::export]]
NumericVector partiallyShuffleVector(NumericVector& x, int k) {
  
  for(int i = 1; i <= k; i++){
    
    double swap_tm,a=0,b=1;        // declare temporary variable for the swap and a,b for the uniform
    double randm = R::runif(a,b); //randomly sample number from uniform(0,1)
    int loc_t = randm * x.size(); // here we are getting it back as integer-so we take advantage of the "randomness" of unif(0,1)
    swap_tm = x[loc_t];
    x[loc_t] = x[i-1];
    x[i-1] = swap_tm;
  }
  
  NumericVector final_lis(x.begin(),x.begin()+k);
  return final_lis;
  
}


//  use this function in order to calculate the standard deviation at the second algorithm and for some part of the first one
double find_sd(NumericVector& x){
  
  int n = x.size();
  double value_of_mean = (std::accumulate(x.begin(),x.end(),0.0) / n);
  
  std::vector<double> support_df(n);
  std::transform(x.begin(), x.end(), support_df.begin(),std::bind2nd(std::minus<double>(), value_of_mean));
  
  
  double sq_sum = std::inner_product(support_df.begin(), support_df.end(), support_df.begin(), 0.0);
  double value_of_sd = std::sqrt(sq_sum / (n-1));
  
  return  value_of_sd; // returns the standard deviation
}



//  e-m algorithm for the case of equal variances

// [[Rcpp::export]]
List gmm615emFull(NumericVector x, int k, int maxiter=1000, double tol=1e-8) {
  NumericVector pis(k);
  NumericVector mus(k);  
  NumericVector sds(k);
  double maxllk = 0;
  int iter = 0;
  
  int n = x.size();
  fill(pis.begin(),pis.end(),1/double(k));
  std::fill(sds.begin(),sds.end(),find_sd(x)); 
  mus = partiallyShuffleVector(x,k);
  
  for(int i = 0; i < maxiter; i++){
    
    double llk = 0;
    double s = 0;
    NumericVector wsum(k),wxsum(k),wxxsum(k),w(k);
    std::fill(wsum.begin(),wsum.end(),0);
    std::fill(wxsum.begin(),wxsum.end(),0);
    std::fill(wxxsum.begin(),wxxsum.end(),0);
    
    for(int j = 0; j < n ; j++){
      for(int z = 0; z < k ; z++){ // O(N*K) algorithm per itteration
       
        w[z] = pis[z] * R::dnorm(x[j],mus[z],sds[z],0); // draw from normal
        s = std::accumulate(w.begin(),w.end(),0.0);
        std::for_each(w.begin(),w.end(), w = w/s);
        llk = llk + log(s); 
        wsum = wsum + w;
        wxsum = wxsum + w * x[j];
        wxxsum = wxxsum + w * x[j] * x[j];
      }
    }
    
    if(abs(llk - maxllk) < tol){break;}
    maxllk = llk;
       
    pis = wsum / n;
    mus = wxsum / n / pis;
    sds = sqrt(wxxsum / n / pis - mus * mus);
    iter = i;
  } 
  
  return( List::create(Named("maxllk")=maxllk, 
                       Named("pis")=pis,
                       Named("mus")=mus,
                       Named("sds")=sds,
                       Named("iter")=iter
  ));
}


// Algorithm 2
// second algorithm to implement  - Uniform to each component (i.e. the same variances )

// [[Rcpp::export]]
List gmm615emUniVar(NumericVector x, int k, int maxiter=1000, double tol=1e-8) {
  NumericVector pis(k);
  NumericVector mus(k);  
  double sd;
  double maxllk = 0;
  int iter = 0;
  
  int n = x.size();
  fill(pis.begin(),pis.end(),1/double(k));
  mus = partiallyShuffleVector(x,k);
  sd = find_sd(x); // use the function we have created above to calculate sd 

  for(int i = 0; i < maxiter ; i++){
    
    double llk = 0;double sum_s = 0;double wxxxsum = 0;double final_sum = 0;
    NumericVector wsum(k),wxsum(k),wxxsum(k),w(k),sds(k),find_mus(k),find_pis(k),mpis_total(k);
    std::fill(wsum.begin(),wsum.end(),0);
    std::fill(wxsum.begin(),wxsum.end(),0);
    std::fill(wxxsum.begin(),wxxsum.end(),0);
    
    for(int j = 0; j < n ; j++){
      for(int z = 0; z < k ; z++){ // O(N*K) algorithm per itteration
        
        w[z] = pis[z] * R::dnorm(x[j],mus[z],sd,0); // sd uniform for all k per each itteration
        
        sum_s = std::accumulate(w.begin(),w.end(),0.0);
        
        std::for_each(w.begin(),w.end(), w = w/sum_s);
        llk = llk + log(sum_s); 
        wsum = wsum + w;
        wxsum = wxsum + w * x[j];
        wxxsum = wxxsum + w * x[j] * x[j];

        find_mus = wxsum  / wsum;
        find_pis = wsum / n;
        mpis_total = find_mus * find_mus * find_pis;
        final_sum = std::accumulate(mpis_total.begin(),mpis_total.end(),0.0);
        
      }
        
    }
    
    wxxxsum = std::accumulate(wxxsum.begin(),wxxsum.end(),0.0);

    
    if(abs(llk - maxllk) < tol){break;}
    maxllk = llk;
    
    // Here is not the same as before as we have same variances and so the 
    // standard deviations are going to be updated once per iteration
    // and remain the same for the whole-same itteration
    // i.e. we only update the sd's at the M - step as follows:
    
    pis = wsum / n;
    mus = wxsum / n / pis;
    sd = sqrt(wxxxsum / n - final_sum);
    iter = i;
    
  } 
  
  
  return( List::create(Named("maxllk")=maxllk, 
                       Named("pis")=pis,
                       Named("mus")=mus,
                       Named("sd")=sd,
                       Named("iter")=iter
  ));
}

