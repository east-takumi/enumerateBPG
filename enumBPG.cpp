#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

enum Parenthesis{LEFT, RIGHT};
vector<int> recover_point_s2;
vector<int> recover_point_s1;
vector<int> connected_value_top;
vector<int> connected_value_bottom;

unsigned long numBPG=0;
bool isParent = true;
bool update_connected_value = false;
bool update_n1 = false;
bool is_s1_root = true;

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
    int update_value_0, update_value_1 = 0;
    if(isParent == 1){
      for(int i=0 ; i<s1.size()-1 ; i++){
        if(s1[i] == LEFT) c1++;
        else c1--;
        if(s2[i] == RIGHT) c2++;
        else c2--;
        if(c1 == c2) return false;

        connected_value_top.push_back(c1);
        connected_value_bottom.push_back(c2);
      }
    }
     else if(is_s1_root == false) {
      update_value_0 = connected_value_top[recover_point_s1.back()] - 2;
      update_value_1 = update_value_0 + 1;
      if(update_value_0 == connected_value_bottom[recover_point_s1.back()]) return false;
      if(update_value_1 == connected_value_bottom[recover_point_s1.back()+1]) return false;

      connected_value_top[recover_point_s1.back()] = update_value_0;
      connected_value_top[recover_point_s1.back()+1] = update_value_1;
    }
    else{
      update_value_0 = connected_value_bottom[recover_point_s2.back()-1] + 2;
      update_value_1 = update_value_0 - 1;
      if(connected_value_top[recover_point_s2.back()-1] == update_value_0) return false;
      if(connected_value_top[recover_point_s2.back()] == update_value_1) return false;

      connected_value_bottom[recover_point_s2.back()-1] = update_value_0;
      connected_value_bottom[recover_point_s2.back()] = update_value_1;
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

void recover_connected_value_s1(){
  int update_value = 0;
  connected_value_top[recover_point_s1.back()] += 2;
  update_value = connected_value_top[recover_point_s1.back()];
  connected_value_top[recover_point_s1.back()+1] = update_value - 1;
}

void recover_connected_value_s2(){
  int update_value = 0;
  connected_value_bottom[recover_point_s2.back()-1] -= 2;
  update_value = connected_value_bottom[recover_point_s2.back()-1];
  connected_value_bottom[recover_point_s2.back()] = update_value + 1;
}

void enumeration(Sequence& s){
  // printf("target：");
  //  for(int i=0; i<s.s1.size(); i++){
  //         printf("%d",s.s1[i]);
  //       }
  //       printf(" ");
  //       for(int i=0; i<s.s2.size(); i++){
  //         printf("%d",s.s2[i]);
  //       }
  //       printf("\n");
  if(!s.isCanonical() || !s.isConnected()){
    //    if(!s.isCanonical()) printf("not canonical \n");
    //  if(!s.isConnected()) printf("not connected \n");
    return;
  }

  s.output();
  numBPG++;

  if(isParent == false){
    if(is_s1_root == true){
      // if(recover_point_s2.back() == s.s2.size() || recover_point_s2.back() == 0) return;
      if(update_n1 == true){
        if(s.s2[recover_point_s2.back()]+2 == LEFT){
          int n2 = recover_point_s2[recover_point_s2.size()-2];
          recover_point_s2.push_back(n2);

          if(s.s2[n2] != s.s2[n2-1] && recover_point_s2.back() != s.s2.size() && recover_point_s2.back() != 0){
            swap(s.s2[n2], s.s2[n2-1]);
            update_n1 = false;
            enumeration(s);
            swap(s.s2[n2], s.s2[n2-1]);
            if(update_connected_value == true) recover_connected_value_s2();
            update_connected_value = false;
          }
          else{
            //  printf("；じ文字続き");
          }
          recover_point_s2.pop_back();
        }
        else{
          int n1 = recover_point_s2.back();
          int n2 = recover_point_s2[recover_point_s2.size()-2];
          recover_point_s2.push_back(n2);
          recover_point_s2.push_back(n1-1);

          
          if(s.s2[n1-1] != s.s2[n1-1-1] && recover_point_s2.back() != s.s2.size() && recover_point_s2.back() != 0){
            swap(s.s2[n1-1], s.s2[n1-1-1]); 
            update_n1 = true;
            enumeration(s);
            swap(s.s2[n1-1], s.s2[n1-1-1]);
            if(update_connected_value == true) recover_connected_value_s2();
            update_connected_value = false;
          }
          else{
            //  printf("；じ文字続き");
          }
          recover_point_s2.pop_back();

          if(s.s2[n2] != s.s2[n2-1] && recover_point_s2.back() != s.s2.size() && recover_point_s2.back() != 0){
            swap(s.s2[n2], s.s2[n2-1]);
            update_n1 = false;
            enumeration(s);
            swap(s.s2[n2], s.s2[n2-1]);
            if(update_connected_value == true) recover_connected_value_s2();
            update_connected_value = false;
          }
          else{
            //  printf("；じ文字続き");
          }
          recover_point_s2.pop_back();
        }
      }
      else{
        if(s.s2[recover_point_s2.back()+2] == LEFT){
          int n2 = recover_point_s2.back();
          recover_point_s2.push_back(n2-1);

          if(s.s2[n2-1] != s.s2[n2-1-1] && recover_point_s2.back() != s.s2.size() && recover_point_s2.back() != 0){
            swap(s.s2[n2-1], s.s2[n2-1-1]);
            update_n1 = false;
            enumeration(s);
            swap(s.s2[n2-1], s.s2[n2-1-1]);
            if(update_connected_value == true) recover_connected_value_s2();
            update_connected_value = false;
          }
          else{
            //  printf("同じ文字続き");
          }
          recover_point_s2.pop_back();
        }
        else{
          int n2 = recover_point_s2.back();
          recover_point_s2.push_back(n2+1);
          recover_point_s2.push_back(n2-1);

          if(s.s2[n2-1] != s.s2[n2-2] && recover_point_s2.back() != s.s2.size() && recover_point_s2.back() != 0){
            swap(s.s2[n2-1], s.s2[n2-2]);
            update_n1 = true;
            enumeration(s);
            if(update_connected_value == true) recover_connected_value_s2();
            update_connected_value = false;
            swap(s.s2[n2-1], s.s2[n2-2]);
          }
          else{
            // printf("同じ文字続き\n");
          }
          recover_point_s2.pop_back();

          if(s.s2[n2+1] != s.s2[n2] && recover_point_s2.back() != s.s2.size() && recover_point_s2.back() != 0){
            swap(s.s2[n2+1], s.s2[n2] );
            update_n1 = false;
            enumeration(s);
            swap(s.s2[n2+1], s.s2[n2]);
            if(update_connected_value == true) recover_connected_value_s2();
            update_connected_value = false;
          }
          else{
            //  printf("同じ文字続き\n");
          }
          recover_point_s2.pop_back();
        }
      }
      is_s1_root = false;
      int n1 = recover_point_s1.back();
      swap(s.s1[n1], s.s1[n1+1]);
      update_n1 = true;
      enumeration(s);
      swap(s.s1[n1], s.s1[n1+1]);
      if(update_connected_value == true) recover_connected_value_s1();
      update_connected_value = false;
      is_s1_root = true;
    }

    else{
    // else if(is_s1_root == false){
      if(update_n1 == true){
        // if(recover_point_s1.back() == s.s1.size()-1 || recover_point_s1.back() == -1) return;
        if(s.s1[recover_point_s1.back()+2] == LEFT){
          int n1 = recover_point_s1.back();
          recover_point_s1.push_back(n1-1);

          if(s.s1[n1-1] != s.s1[n1] && recover_point_s1.back() != s.s1.size()-1 && recover_point_s1.back() != -1){
            swap(s.s1[n1-1], s.s1[n1]);
            update_n1 = true;
            enumeration(s);
            swap(s.s1[n1-1], s.s1[n1]);
            if(update_connected_value == true) recover_connected_value_s1();
            update_connected_value = false;
          }
          else{
            //  printf("同じ文字続き");
          }
          recover_point_s1.pop_back();
        }
        else{
          int n1 = recover_point_s1.back();
          recover_point_s1.push_back(n1-1);
          recover_point_s1.push_back(n1+1);
          
          if(s.s1[n1+1] != s.s1[n1+2] && recover_point_s1.back() != s.s1.size()-1 && recover_point_s1.back() != -1){
            swap(s.s1[n1+1], s.s1[n1+2]);
            update_n1 = false;
            enumeration(s);
            swap(s.s1[n1+1], s.s1[n1+2]);
            if(update_connected_value == true) recover_connected_value_s1();
            update_connected_value = false;
          }
          else{
            //  printf("同じ文字続き");
          }
          recover_point_s1.pop_back();

          if (s.s1[n1-1] != s.s1[n1] && recover_point_s1.back() != s.s1.size()-1 && recover_point_s1.back() != -1){
            swap(s.s1[n1-1], s.s1[n1]);
            update_n1 = true;
            enumeration(s);
            swap(s.s1[n1-1], s.s1[n1]);
            if(update_connected_value == true) recover_connected_value_s1();
            update_connected_value = false;
          }
          else {
            //  printf("同じ文字続き");
          }
          recover_point_s1.pop_back();
        }
      }
      else{
        // if(recover_point_s1.back() == s.s1.size()-1 || recover_point_s1.back() == 0) return;
        if(s.s2[recover_point_s1.back()+2] == LEFT){
          int n1 = recover_point_s1[recover_point_s1.size()-2];
          recover_point_s1.push_back(n1);
          
          if(s.s1[n1] != s.s1[n1+1] && recover_point_s1.back() != s.s1.size()-1 && recover_point_s1.back() != -1){
            swap(s.s1[n1], s.s1[n1+1]);
            update_n1 = true;
            enumeration(s);
            swap(s.s1[n1], s.s1[n1+1]);
            if(update_connected_value == true) recover_connected_value_s1();
            update_connected_value = false;
          }
          else {
            //  printf("同じ文字続き");
          } 
          recover_point_s1.pop_back();
        }
        // if(s.s1[recover_point_s1.back()+2] == RIGHT){
        else{
          int n1 = recover_point_s1[recover_point_s1.size()-2];
          int n2 = recover_point_s1.back();
          recover_point_s1.push_back(n1);
          recover_point_s1.push_back(n2+1);

          if(s.s1[n2+1] != s.s1[n2+2]&& recover_point_s1.back() != s.s1.size()-1 && recover_point_s1.back() != -1){
            swap(s.s1[n2+1], s.s1[n2+2]);
            update_n1 = false;
            enumeration(s);
            swap(s.s1[n2+1], s.s1[n2+2]);
            if(update_connected_value == true) recover_connected_value_s1();
            update_connected_value = false;
          }
          else {
            //  printf("同じ文字続き");
          }
          recover_point_s1.pop_back();

          if(s.s1[n1] != s.s1[n1+1]&& recover_point_s1.back() != s.s1.size()-1 && recover_point_s1.back() != -1){
            swap(s.s1[n1], s.s1[n1+1]);
            update_n1 = true;
            enumeration(s);
            swap(s.s1[n1], s.s1[n1+1]);
            if(update_connected_value == true) recover_connected_value_s1();
            update_connected_value = false;
          }
          else {
            //  printf("同じ文字続き");
          }
          recover_point_s1.pop_back();
        }
        // else return;
      }
    }
    update_connected_value = true;
    // else return;
  }
  else{
    for(int i=1 ; i<s.s1.size() ; i++){
      if(s.s1[i] == LEFT && s.s1[i+1] == RIGHT){
        if(!recover_point_s1.empty()) recover_point_s1.pop_back();
        recover_point_s1.push_back(i);
        break;
      }
    }
    for(int i=s.s2.size()-1 ; i>0 ; i--){
      if(s.s2[i-1] == LEFT && s.s2[i] == RIGHT){
        if(!recover_point_s2.empty()) recover_point_s2.pop_back();
        recover_point_s2.push_back(i);
        swap(s.s2[i], s.s2[i-1]);
        isParent = false;
        update_n1 = false;
        enumeration(s);
        swap(s.s2[i], s.s2[i-1]);
        recover_point_s2.pop_back();
        if(update_connected_value == true) recover_connected_value_s2();
        update_connected_value = false;
        break;
      }
    }
    recover_point_s1.pop_back();
  }
  return;
}


void enumeration(int p, int q){
  Sequence s(p, q);
  isParent = true;
  is_s1_root = true;
  enumeration(s);
}


int main(int argc, char** argv){
  int n;
  cout << "Please input a number of vertices: ";
  cin >> n;

  for(int j=n ; j<=n ; j++){
    numBPG = 0;
    for(int i=1 ; i<=j/2 ; i++){
      connected_value_top.clear();
      connected_value_bottom.clear();
      cerr << "p, q: " << j-i << ", " << i << endl;
      enumeration(j-i, i);
      cout << numBPG << endl;
    }
  }
}
