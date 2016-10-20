#ifndef JCHMAPPER_H
#define JCHMAPPER_H
#include <string>

class JChMapper {
  private:
    JChMapper(); // JChMapper is a singleton!

  public:
    ~JChMapper();

    int GetLayer(int fec, int ch, int rcu = 0);
    int GetPad  (int fec, int ch, int rcu = 0);

    void GetPadAddress(int rcu, int ch, int &module, int &layer, int &pad);

    static JChMapper *GetInstance();
    static void       SetMapFile(const char *mapfile) { fgMapFile = mapfile; }

  private:
    static JChMapper   *fgInstancePtr;    // the singleton pointer
    static std::string  fgMapFile;        // map file
    int                 fModule[4][8192]; // module[rcu][ch]
    int                 fLayer [4][8192]; // layer [rcu][ch]
    int                 fPad   [4][8192]; // pad   [rcu][ch]
};
#endif
