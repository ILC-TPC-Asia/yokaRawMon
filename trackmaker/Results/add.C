#include <sstream>
#include <fstream>
void add(const char* f1 = "deltax1.dat", const char* f2 = "deltax2.dat")
{
  ifstream in1(f1);
  ifstream in2(f2);
  ofstream out("deltaxsum.dat");
  for (int i=0; i<84; i++) {
    int    l1, l2;
    double x1, x2;
    in1 >> l1 >> x1;
    in2 >> l2 >> x2;
    out << l1 << " " << x1+x2 << endl;
  }
}
