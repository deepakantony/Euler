#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

bool is_prime(int n) {
  if(n==2 || n==3 || n==5 || n==7) return true;
  if(n<11 || n%2 == 0 || n%3 == 0) return false;

  int sqrt_n = ceil(sqrt(n));
  for(int i = 5; i <= sqrt_n; i+=6) {
    if(n%i==0) return false;
    if(n%(i+2)==0) return false;
  }
  return true;
}

vector<int> generate_primes(int limit) {
  vector<int> primes;
  primes.push_back(2);
  primes.push_back(3);
  for(int i = 5; i < limit; i+=6) {
    if(is_prime(i)) primes.push_back(i);
    if(is_prime(i+2)) primes.push_back(i+2);
  }
  return primes;
}

int reverse(int n) {
  int rev = 0;
  while(n>0) {
    rev = rev*10+n%10;
    n/=10;
  }
  return rev;
}

int concat(int n1, int n2) {
  int res = n1;
  n2 = reverse(n2);
  while(n2 > 0) {
    res = res*10+n2%10;
    n2 /= 10;
  }
  return res;
}

bool is_concat_prime(int n1, int n2) {
  return(is_prime(concat(n1,n2)) && is_prime(concat(n2,n1)));
}

bool is_concat_prime(vector<int> vec) {
  int size = vec.size();
  for(int i = 0; i < size; i++)
    for(int j = i+1; j < size; j++)
      if(!is_concat_prime(vec[i], vec[j])) return false;
  return true;
}

int sum(vector<int>::const_iterator start, vector<int>::const_iterator end) {
  int s = 0;
  for(vector<int>::const_iterator it = start; start != end; it++)
    s += *it;
  return s;
}

vector<int> concatable_prime(vector<int> primes, int size) {
  vector<int> res;
  int smallest_sum = numeric_limits<int>::max();
  vector<int> cur_set;
  int cur_sum = sum(cur_set.begin(), cur_set.end());

  while(res.size() < size) {
    

int main(int argc, char **argv) {
  return 0;
}