#include <iostream>

#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
using namespace std;

cpp_int binom(int n, int k){
  cpp_int x = 1;
  for(int i=1 ; i<=n ; i++){
    x *= i;
  }
  for(int i=1 ; i<=k ; i++){
    x /= i;
  }
  for(int i=1 ; i<=n-k ; i++){
    x /= i;
  }
  return x;
}

cpp_int Catalan(int n){
  cpp_int x = binom(2*n, n);
  x /= (n+1);
  return x;
}


cpp_int numBPG(int n){
  cpp_int x;
  if(n % 2 == 0){
    x = Catalan(n-1) + Catalan(n/2 - 1) + binom(n-1, (n-1)/2) + binom(n-1, (n-1)/2);
    x /= 4;
  }
  else{
    x = Catalan(n-1) + binom(n-1, (n-1)/2);
    x /= 4;
  }
  return x;
}


int main()
{
  int n;
  cout << "Please input an integer: ";
  cin >> n;

  cout << "Number of Bipartite permutation graphs" << endl;
  for(int i=1 ; i<=n ; i++){
    cout << i << ": " << numBPG(i) << endl;
  }
}
