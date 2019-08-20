#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

enum Parenthesis{LEFT, RIGHT};
int swap_point;
// int* connected_value_top = new int[size_array];
// int* connected_value_bottom = new int[size_array];
struct date{
  int swap_point;
  // int* connected_value_top = new int[size_array];
  // int* connected_value_bottom = new int[size_array];
  vector<int> connected_value_top;
  vector<int> connected_value_bottom;
};
class date data_value;
stack<date> recover_data;


unsigned long numBPG=0;
bool isParent=true;
bool update_is_s1=true;

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

  bool isCanonical(){
    Sequence sr = rotate();
    if(sr < *this) return false;

    if(p == q){
      Sequence sv = flipVertical();
      if(sv < *this) return false;
      Sequence sh = flipHorizontal();
      if(sh < *this) return false;
    }
    return true;
  }

  bool isConnected(){
    int c1, c2;
    c1 = c2 = 0;
    int update_value[2];

    if(isParent == 1){
      for(int i=0 ; i<s1.size()-1 ; i++){
        if(s1[i] == LEFT) c1++;
        else c1--;

        if(s2[i] == RIGHT) c2++;
        else c2--;

        if(c1 == c2) return false;
        data_value.connected_value_top.push_back(c1);
        data_value.connected_value_bottom.push_back(c2);
      }
    }
    else if(update_is_s1 == 1) {
      update_value[0]=data_value.connected_value_top[data_value.swap_point]-2;
      if(update_value[0] == data_value.connected_value_bottom[data_value.swap_point]) return false; 
      update_value[1]=update_value[0]+1;
      if(update_value[1] == data_value.connected_value_bottom[data_value.swap_point+1]) return false;

      data_value.connected_value_top[data_value.swap_point]=update_value[0];
      data_value.connected_value_top[data_value.swap_point+1]=update_value[1];
    }
    else{
      update_value[0]=data_value.connected_value_bottom[data_value.swap_point]+2;
      if(data_value.connected_value_top[data_value.swap_point] == update_value[0]) return false;
      update_value[1]=update_value[0]-1;
      if(data_value.connected_value_top[data_value.swap_point+1] == update_value[1]) return false;

      data_value.connected_value_bottom[data_value.swap_point]=update_value[0];
      data_value.connected_value_bottom[data_value.swap_point+1]=update_value[1];
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

void push_data(int point){
  data_value.swap_point = point;
  recover_data.push(data_value);
}

void recover_and_pop(){
  data_value=recover_data.top();
  recover_data.pop();
}


void enumeration(Sequence& s){
  if(!s.isCanonical() || !s.isConnected()){
    return;
  }

  s.output();
  numBPG++;
  isParent = 0;

  if(s.isS1Root()){
    // case 1
    for(int i=s.s2.size()-1 ; i>0 ; i--){
      if(s.s2[i-1] == LEFT && s.s2[i] == RIGHT){
        swap(s.s2[i], s.s2[i-1]);
        push_data(i-1);
        update_is_s1 = 0;
        enumeration(s);
        recover_and_pop();
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
          push_data(i-1);
          update_is_s1 = 0;
          enumeration(s);
          recover_and_pop();
          swap(s.s2[i], s.s2[i-1]);
        }
        break;
      }
      if(!flag && s.s2[i-1] == LEFT && s.s2[i] == RIGHT) flag = true;
    }
    // case 3 (swap on s1)
    for(int i=0 ; i<s.s1.size() - 1 ; i++){
      if(s.s1[i] == LEFT && s.s1[i+1] == RIGHT){
        swap(s.s1[i], s.s1[i+1]);
        push_data(i);
        update_is_s1 = 1;
        enumeration(s);
        recover_and_pop();
        swap(s.s1[i], s.s1[i+1]);
        break;
      }
    }
  }
  
  else{
    // case 1
    for(int i=0 ; i<s.s1.size()-1 ; i++){
      if(s.s1[i] == LEFT && s.s1[i+1] == RIGHT){
        swap(s.s1[i], s.s1[i+1]);
        push_data(i);
        update_is_s1 = 1;
        enumeration(s);
        recover_and_pop();
        swap(s.s1[i], s.s1[i+1]);
        break;
      }
    }
    // case 2
    bool flag = false; // leftmost "]["
    for(int i=0 ; i<s.s1.size()-1 ; i++){
      if(flag && s.s1[i] == RIGHT) continue;
      
      if(flag && s.s1[i] == LEFT){
        if(s.s1[i+1] == RIGHT){
          swap(s.s1[i], s.s1[i+1]);
          push_data(i);
          update_is_s1 = 1;
          enumeration(s);
          recover_and_pop();
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
      data_value.connected_value_top.clear();
      data_value.connected_value_bottom.clear();
      isParent = 1;
      update_is_s1 = 1;
      enumeration(j-i, i);
      cout << numBPG << endl;
    }
  }
}
