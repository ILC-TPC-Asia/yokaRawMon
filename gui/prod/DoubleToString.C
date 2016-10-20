//
// $Id: DoubleToString.C,v 1.1.1.1 2009/03/08 01:15:50 fujiik Exp $
//
#include <iomanip>
using namespace std;

Char_t *DoubleToString(Double_t x) {
  ostringstream sout;
  sout << setw(5) << setprecision(3) << x << ends;
  return sout.str().data();
}
