/* RVO optimisation test (taken from wiki.com) 
$ g++ rvo.cpp -o rvo */

#include <iostream>
 
struct C {
  C() {}
  C(const C&) { std::cout << "A copy was made.\n"; }
};
 
C f() {
  return C();
}
 
int main() {
  std::cout << "Hello World!\n";
  C obj = f();
}

