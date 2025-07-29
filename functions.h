/* Key binding functions */
static void togglegaps(const Arg *arg);
/* Layouts */
static void bstack(Monitor *m);
static void centeredmaster(Monitor *m);
static void horizgrid(Monitor *m);
static void tile(Monitor *m);
/* Internals */
static void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc);
static void getfacts(Monitor *m, int msize, int ssize, float *mf, float *sf, int *mr, int *sr);

static void movestack(const Arg *arg);
