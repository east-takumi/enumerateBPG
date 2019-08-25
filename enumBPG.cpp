#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <algorithm>

using namespace std;

enum Parenthesis{LEFT, RIGHT};
stack<int> recover_point;
list<int> index_point_r, index_point_v, index_point_h;

unsigned long numBPG=0;
bool updateRoot = false;

class Sequence{
public:
  int p, q;
  vector<int> s1;
  vector<int> s2;

  Sequence(int p, int q){
    this->p = p; this->q = q;
    for(int i=0 ; i<p ; i++) s1.push_back(LEFT);
    for(int i=0 ; i<q ; i++) s1.push_back(RIGHT);
    for(int i=0 ; i<q ; i++) s2.push_back(LEFT);
    for(int i=0 ; i<p ; i++) s2.push_back(RIGHT);

    if(s1.size() != s2.size()) exit(-1);
  }

  Sequence(vector<int> s1, vector<int> s2){
    this->s1 = s1;
    this->s2 = s2;
  }

  Sequence flipVertical(){
    vector<int> sv1, sv2;
    for(int i=s1.size()-1 ; i>=0 ; i--){
      if(s1[i] == LEFT) sv1.push_back(RIGHT);
      else sv1.push_back(LEFT);
    }
    for(int i=s2.size()-1 ; i>=0 ; i--){
      if(s2[i] == LEFT) sv2.push_back(RIGHT);
      else sv2.push_back(LEFT);
    }
    return Sequence(sv1, sv2);
  }

  Sequence flipHorizontal(){
    return Sequence(s2, s1);
  }

  Sequence rotate(){
    Sequence sv = flipVertical();
    return sv.flipHorizontal();
  }

  void indexPoint(Sequence& root, Sequence& edited, list<int> index_point){
    for(int i=1; i<s1.size()-1; i++){
      if(root.s1[i]!=edited.s1[i]){
        index_point.push_back(i);
      }
    }
    for(int i=1; i<s2.size()-1; i++){
      if(root.s2[i]!=edited.s2[i])
        index_point.push_back(i+s2.size());
    }
  }

  bool checkCanonical(list<int>){
    if(index_point_r.front() < s1.size()){
      if(s1[index_point_r.front()] == 1) return false;
    }
    else{
      if(s2[index_point_r.front()] == 1) return false;
    }
    return true;
  }

  bool isCanonical(){
    if(updateRoot == false){
      Sequence sr = rotate();
      if(sr < *this) return false;
      if(p == q){
        Sequence sv = flipVertical();
        if(sv < *this) return false;
        Sequence sh = flipHorizontal();
        if(sh < *this) return false;
        indexPoint(*this, sv, index_point_v);
        indexPoint(*this, sh, index_point_h);
      }
      indexPoint(*this, sr, index_point_r);
    }
    else{

      if(checkCanonical(index_point_r) == false) return false;
      if(p == q){
        if(checkCanonical(index_point_v) == false) return false;
        if(checkCanonical(index_point_h) == false) return false;
      }
    }
    return true;
  }

  bool isConnected(){
    int c1, c2;
    c1 = c2 = 0;
    for(int i=0 ; i<s1.size()-1 ; i++){
      if(s1[i] == LEFT) c1++;
      else c1--;
      
      if(s2[i] == RIGHT) c2++;
      else c2--;

      if(c1 == c2) return false;
    }
    return true;
  }

  bool isS1Root(){
    bool flag = false; // maintain appearing RIGHT
    for(int i=0 ; i<s1.size() ; i++){
      if(flag == true && s1[i] == LEFT) return false;
      if(s1[i] == RIGHT) flag = true;
    }
    return true;
  }

  bool operator<(const Sequence& rhs){
    for(int i=0 ; i<s1.size() ; i++){
      if(s1[i] == rhs.s1[i]) continue;
      if(s1[i] == LEFT) return true;
      else return false;
    }
    for(int i=s2.size()-1 ; i>=0 ; i--){
      if(s2[i] == rhs.s2[i]) continue;
      if(s2[i] == RIGHT) return true;
      else return false;
    }
    return false;
  }

  void output(){
    for(int i=0 ; i<s1.size() ; i++){
      if(s1[i]==LEFT) cout << "0";
      else cout << "1";
    }
    cout << " ";
    for(int i=0 ; i<s2.size() ; i++){
      if(s2[i]==LEFT) cout << "0";
      else cout << "1";
    }
    cout << endl;
  }

  bool equalS2(vector<int> s2){ return this->s2 == s2;}
  bool equal(vector<int> s1, vector<int> s2){
    return (this->s1 == s1 && this->s2 == s2);
  }
  bool operator==(const Sequence& rhs){
    return (this->s1 == rhs.s1 && this->s2 == rhs.s2);
  }

};


void enumeration(Sequence& s){
  if(!s.isCanonical() || !s.isConnected()){
    return;
  }

  s.output();
  numBPG++;
  updateRoot = true;

  if(s.isS1Root()){
    // case 1
    for(int i=s.s2.size()-1 ; i>0 ; i--){
      if(s.s2[i-1] == LEFT && s.s2[i] == RIGHT){
        swap(s.s2[i], s.s2[i-1]);
        recover_point.push(i-1);
        enumeration(s);
        swap(s.s2[i], s.s2[i-1]);
        break;
      }
    }
    // case 2
    bool flag = false; // maintain Rightmost "]["
    for(int i=s.s2.size()-1 ; i>0 ; i--){
      if(flag && s.s2[i] == LEFT) continue;
      
      if(flag && s.s2[i] == RIGHT){
        if(s.s2[i-1] == LEFT){
          swap(s.s2[i], s.s2[i-1]);
          recover_point.push(i-1);
          enumeration(s);
          recover_point.pop();
          swap(s.s2[i], s.s2[i-1]);
        }
        break;
      }
      if(!flag && s.s2[i-1] == LEFT && s.s2[i] == RIGHT) flag = true;
    }
    // case 3 (swap on s1)
    for(int i=0 ; i<s.s1.size() ; i++){
      if(s.s1[i] == LEFT && s.s1[i+1] == RIGHT){
        swap(s.s1[i], s.s1[i+1]);
        recover_point.push(i);
        enumeration(s);
        recover_point.pop();
        swap(s.s1[i], s.s1[i+1]);
        break;
      }
    }
  }
  
  else{
    // case 1
    for(int i=0 ; i<s.s1.size() ; i++){
      if(s.s1[i] == LEFT && s.s1[i+1] == RIGHT){
        swap(s.s1[i], s.s1[i+1]);
        recover_point.push(i);
        enumeration(s);
        recover_point.pop();
        swap(s.s1[i], s.s1[i+1]);
        break;
      }
    }
    // case 2
    bool flag = false; // leftmost "]["
    for(int i=0 ; i<s.s1.size() ; i++){
      if(flag && s.s1[i] == RIGHT) continue;
      
      if(flag && s.s1[i] == LEFT){
        if(s.s1[i+1] == RIGHT){
          swap(s.s1[i], s.s1[i+1]);
          recover_point.push(i);
          enumeration(s);
          recover_point.pop();
          swap(s.s1[i], s.s1[i+1]);
        }
        break;
      }
      if(s.s1[i] == LEFT && s.s1[i+1] == RIGHT) flag = true;
    }
  }
}


void enumeration(int p, int q){
  Sequence s(p, q);
  updateRoot = false;
  index_point_h.clear();
  index_point_r.clear();
  index_point_v.clear();
  enumeration(s);
}


int main(int argc, char** argv){
  int n;
  cout << "Please input a number of vertices: ";
  cin >> n;

  for(int j=n ; j<=n ; j++){
    numBPG = 0;
    for(int i=1 ; i<=j/2 ; i++){
      cerr << "p, q: " << j-i << ", " << i << endl;
      enumeration(j-i, i);
      cout << numBPG << endl;
    }
  }
}
