# Dynamic-Programming Algorithms 

The implementation of the algorithms is in C++ but we are able to run the programs
through R ( Rcpp ). For further information considering these algorithms please
follow the links provided below:

   1) Money(Coin) change problem - Implemented in Python
        reference link: https://www.geeksforgeeks.org/dynamic-programming-set-7-coin-change/

   2) Edit distance - C++
        reference link: https://www.geeksforgeeks.org/dynamic-programming-set-5-edit-distance/

   3) Binomial Coefficients calculation using DP ( + 2  R methods) - C++
       reference link: https://www.geeksforgeeks.org/dynamic-programming-set-9-binomial-coefficient/
       
       
       Output:
       <br><br/>
       ![alt text](https://github.com/gpsyrou/Dynamic-Programming/blob/master/fac_pc.PNG)
       
       <br><br/>
Now we have to explain why the results at the table above, are not consistent, even though they should do the same job. I will separate these three different implementations and discuss our findings for each of them, as follows:
<br><br/>

**choose_fac**

The first case, choose_fac overflows really fast (from the second case already-only the first one was successful).This is happening as from the way this function is defined, makes it impossible to store big number (we already have problem for n = 15) and we have overflow of the integer making it unable to give the proper answer. Hence, these big numbers cannot be stored in the bits provided with this implementation of finding factorials .We can have higher values only by using another libraries, but with the standard library of C++ , we have limits on the values).

<br><br/>
**choose_dp**

The second case seems to work fine and give us the proper results for all the cases except the last one. At the last one we have a really big pair (n,k), and thus we end up again in an overflow for huge numbers ( that’s why we get a negative value). We may say that it might be a precision error due to the range of the numbers and using factorials bigger than 20 in C++ is impossible with built in functions.

We saw that its working properly for the other cases, and it doesn’t "stuck" like the first one (choose_fac).This happens because of the construction of algorithm for this function, which is using dynamic programming, giving it the ability to store the values in other ways than the first case (avoiding the leaks at least for a fair amount of values), making it more efficient both in terms of memory space and time. The last result which is false can be accounted to the fact that at some point we faced memory inefficiency as the numbers got really large, and thus we had leaks making impossible to store the values properly. So we can say that it’s a matter of how many bits the compiler can handle.
<br><br/>
 **choose  (already defined in R)**

Finally the third case seems to work fine for all of the cases that we examined. Choose () is a built in R command which is already well-defined from the base library of R, and so it has the proper tools to calculate huge numbers of factorials and binomial coefficients (it’s handling the storage and the computation procedure properly in order to end up at the correct output).

    
   4) Implementation of two E-M algorithms for Gaussian Mixture Models, reference: https://en.wikipedia.org/wiki/Expectation%E2%80%93maximization_algorithm
