class PadGeom {
public:
  int GetPadInfo(int drorc, int rcu, int fec, int ch, int *Mod, int *lay, int *Pad);
  int GetPadPost(int module, int layer, int pads, double *xc, double *yc);

 private:
  int GetPadGeom(int Layer, int Pad, double *xc, double *yc);
};
