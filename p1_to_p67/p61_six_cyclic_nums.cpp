#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <utility>

using namespace std;

template<class T1, class T2>
ostream& operator<<(ostream& out, const pair<T1,T2> &p) {
  out << "(" << p.first << ", " << p.second << ")";
  return out;
}

template<class T>
ostream& operator<<(ostream& out, const vector<T> &vec) {
  if(!vec.empty()) {
    out << "( ";

    //copy(vec.begin(), vec.end(), ostream_iterator<T>(out, " "));

    for(typename vector< T >::const_iterator it = vec.begin(); 
	it != vec.end(); it++)
      out << *it << " ";
    out << " )";
  }
  return out;
}

int get_polygon_number(int polygon, int n) {
  switch(polygon) {
  case 3: return ((n*(n+1)) >> 1);
  case 4: return n*n;
  case 5: return ((n*(3*n-1)) >> 1);
  case 6: return (n*(2*n-1));
  case 7: return ((n*(5*n-3)) >> 1);
  case 8: return (n*(3*n-2));
  }
  return 0;
}

vector<int> generate_polygon_nums(int min, int max, int polygon) {
  vector<int> polygon_nums;
  int i = 1;
  int n;
  do {
    n = get_polygon_number(polygon, i);
    if(n >= min && n <= max && n%100 > 9)
      polygon_nums.push_back(n);
    i++;
  }while(n < max);

  return polygon_nums;
}


void generate_polygon_nums(int min, int max, int polygon, vector<pair<int,int> > &polygon_nums) {
  int i = 1;
  int n;
  do {
    n = get_polygon_number(polygon, i);
    if(n >= min && n <= max && n%100 > 9)
      polygon_nums.push_back(pair<int,int>(polygon, n));
    i++;
  }while(n < max);
}

bool is_fwd_cyclic_pair(int n1, int n2) {
  return (n1%100 == n2/100);
}

void generate_polygon_nums_cyclic_pairs(const vector<pair<int,int> > &polygon_nums, 
					multimap<pair<int,int >, vector<pair<int,int> > > &cyclic_pairs) {
  multimap<pair<int,int >, vector<pair<int,int> > >::iterator m_it = cyclic_pairs.begin();
  for(vector<pair<int,int> >::const_iterator it = polygon_nums.begin();
      it != polygon_nums.end(); it++) {
    vector<pair<int,int> > temp;
    for(vector<pair<int,int> >::const_iterator it_2 = polygon_nums.begin();
	it_2 != polygon_nums.end(); it_2++) {
      if(it->first != it_2->first && is_fwd_cyclic_pair(it->second, it_2->second)) {
	temp.push_back(*it_2);
      }
    }
    if(!temp.empty())
      m_it = cyclic_pairs.insert(m_it, pair<pair<int,int >, vector<pair<int,int> > >(*it, temp));
  }
}

bool is_present(int n, const vector<pair<int,int> > &vec) {
  for(vector<pair<int,int> >::const_iterator it = vec.begin(); it != vec.end(); it++)
    if(n == it->first) return true;

  return false;
}

bool update_res(const vector<pair<int,int> > &fwd_cyclic_polygons, 
		const multimap<pair<int,int>, vector<pair<int,int> > > &cyclic_pairs,
		vector<pair<int,int> > &res) {
  if(res.size() == 6
     && is_fwd_cyclic_pair(res[res.size()-1].second, res[0].second)) 
    return true;

  if(res.size() > 6) return false;

  for(vector<pair<int, int> >::const_iterator it = fwd_cyclic_polygons.begin();
      it != fwd_cyclic_polygons.end(); it++) {
    if(!is_present(it->first, res)) {
      res.push_back(*it);
      multimap<pair<int,int>, vector<pair<int,int> > >::const_iterator m_it = cyclic_pairs.find(*it);
      cout << res << endl;
      if(m_it != cyclic_pairs.end()) 
	if( update_res(m_it->second, cyclic_pairs, res))
	  return true;
      res.pop_back();
    }
  }

  return false;
}

vector<int> generate_4dig_cyclic_polygon_nums() {
  vector<pair<int,int> > polygon_nums;
  for(int i = 3; i < 9; i++)
    generate_polygon_nums(1000,9999,i,polygon_nums);

  multimap<pair<int,int>, vector<pair<int,int> > > cyclic_pairs;
  generate_polygon_nums_cyclic_pairs(polygon_nums, cyclic_pairs);
  //for(multimap<pair<int,int>, vector<pair<int,int> > >::iterator it = cyclic_pairs.begin();
  //it != cyclic_pairs.end(); it++)
  //cout << it->first << ": " << it->second << endl;

  vector<pair<int,int> > res;
  for(multimap<pair<int,int>, vector<pair<int,int> > >::iterator it = cyclic_pairs.begin();
      it != cyclic_pairs.end(); it++,res.clear()) {
    res.push_back(it->first);
    if(update_res(it->second, cyclic_pairs, res)) break;
  }
  vector<int> result;
  for(vector<pair<int,int> >::const_iterator it = res.begin(); it != res.end(); it++)
    result.push_back(it->second);

  return result;
}


template<class T>
void quick_sort(T begin, T end) {
  if(end <= begin+1) return;
  T pivot = begin + (end - begin)/2;
  iter_swap(pivot, begin);
  pivot = begin;

  T i = begin+1, j = end-1;
  while(i <= j) {
    if(*i > *pivot) {
      iter_swap(i,j);
      j--;
    }
    else i++;
  }
  if(j > pivot) {
    iter_swap(pivot, j);
    pivot = j;
  }

  quick_sort(begin, pivot);
  quick_sort(pivot+1, end);
}

template<class T>
bool next_perm(T start, T end) {
  T pos = start;
  for(T it = start; (it+1) != end; it++)
    if(*it < *(it+1))
      pos = it;

  T swap_pos = pos;
  for(T it = pos; it != end; it++)
    if(*it > *pos)
      swap_pos = it;

  if(pos == swap_pos) return false;

  iter_swap(pos, swap_pos);
  reverse(pos+1, end);
  return true;
}

bool is_cyclic(int n1, int n2, int n3, int n4, int n5, int n6) {
  vector<int> all_nums;
  all_nums.push_back(n1); all_nums.push_back(n2); all_nums.push_back(n3);
  all_nums.push_back(n4); all_nums.push_back(n5); all_nums.push_back(n6);
  for(int i = 0; i < all_nums.size(); i++)
    for(int j = i+1; j < all_nums.size(); j++)
      if(all_nums[i] == all_nums[j]) return false;

  //quick_sort(all_nums.begin(), all_nums.end());

  //do {
    vector<int>::const_iterator it = all_nums.begin();
    if((*it)/100 == *(all_nums.end()-1)%100) {
      bool success = true;
      while(success && (it+1 != all_nums.end())) {
	success = success && ((*it)%100 == (*(it+1))/100);
	it++;
      }
      if(success) return true;
    }
    //}while(next_perm(all_nums.begin(), all_nums.end()));

  return false;
}

vector<int> get_6_cyclic_4dig_nums() {
  vector<int> res;

  int min = 1000, max = 9999;

  vector<int> triangle_nums = generate_polygon_nums(min,max,3);
  vector<int> square_nums = generate_polygon_nums(min,max,4);
  vector<int> pentagonal_nums = generate_polygon_nums(min,max,5);
  vector<int> hexagonal_nums = generate_polygon_nums(min,max,6);
  vector<int> heptagonal_nums = generate_polygon_nums(min,max,7);
  vector<int> octagonal_nums = generate_polygon_nums(min,max,8);

  

  /*  cout << "Triangle nums size: " << triangle_nums.size() << endl;
  cout << "Square nums size: " << square_nums.size() << endl;
  cout << "Pentagonal nums size: " << pentagonal_nums.size() << endl;
  cout << "Hexagonal nums size: " << hexagonal_nums.size() << endl;
  cout << "Heptagonal nums size: " << heptagonal_nums.size() << endl;
  cout << "Octagonal nums size: " << octagonal_nums.size() << endl;



  /*
  typedef vector<int>::const_iterator vec_const_iter;
  for(vec_const_iter it_triangle = triangle_nums.begin(); 
      it_triangle != triangle_nums.end(); it_triangle++)
    for(vec_const_iter it_square = square_nums.begin();
	it_square != square_nums.end(); it_square++)
      for(vec_const_iter it_pentagonal = pentagonal_nums.begin();
	  it_pentagonal != pentagonal_nums.end(); it_pentagonal++)
	for(vec_const_iter it_hexagonal = hexagonal_nums.begin();
	    it_hexagonal != hexagonal_nums.end(); it_hexagonal++)
	  for(vec_const_iter it_heptagonal = heptagonal_nums.begin();
	      it_heptagonal != heptagonal_nums.end(); it_heptagonal++)
	    for(vec_const_iter it_octagonal = octagonal_nums.begin();
		it_octagonal != octagonal_nums.end(); it_octagonal++) {
	      //cout << "Testing: " << *it_triangle << " " << *it_square 
	      //<< " " << *it_pentagonal << " " << *it_hexagonal
	      //<< " " << *it_heptagonal << " " << *it_octagonal << endl;
	      if(is_cyclic(*it_triangle, *it_square, *it_pentagonal,
			   *it_hexagonal, *it_heptagonal, *it_octagonal)) {
		res.push_back(*it_triangle);
		res.push_back(*it_square);
		res.push_back(*it_pentagonal);
		res.push_back(*it_hexagonal);
		res.push_back(*it_heptagonal);
		res.push_back(*it_octagonal);
		return res;
	      }
	    }
  */
  return res;
}

template<class T>
int sum(T start, T end) {
  int sum = 0;
  while(start != end)
    sum += (*(start++));
  return sum;
}

int main(int argc, char **argv) {
  vector<int> cyclic_numbers = generate_4dig_cyclic_polygon_nums();
  cout << sum(cyclic_numbers.begin(), cyclic_numbers.end()) << endl;
  copy(cyclic_numbers.begin(), cyclic_numbers.end(), 
       ostream_iterator<int>(cout, " "));
  cout << endl;
  return 0;
}

int main_1(int argc, char **argv) {
  vector<int> array;
  for(int i = 1; i < argc; i++)
    array.push_back(atoi(argv[i]));

  copy(array.begin(), array.end(), ostream_iterator<int>(cout, " "));
  cout << endl;
  quick_sort(array.begin(), array.end());
  do {
  copy(array.begin(), array.end(), ostream_iterator<int>(cout, " "));
  cout << endl;
  }while(next_perm(array.begin(), array.end()));
  return 0;
}
