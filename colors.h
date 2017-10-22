#ifndef _COLORS_
#define _COLORS_

#define RST  "\x1B[0m"
#define BOLD(x) "\x1B[1m" x RST
#define DIM(x) "\x1B[2m" x RST
#define UNDL(x) "\x1B[4m" x RST
#define BLNK(x) "\x1B[5m" x RST
#define RVRS(x) "\x1B[7m" x RST
#define HIDD(x) "\x1B[7m" x RST


/* FOREGROUND */
#define KBLK  "\x1B[30m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

/* BACKGROUND */
#define LBLK  "\x1B[41m"
#define LRED  "\x1B[41m"
#define LGRN  "\x1B[42m"
#define LYEL  "\x1B[43m"
#define LBLU  "\x1B[44m"
#define LMAG  "\x1B[45m"
#define LCYN  "\x1B[46m"
#define LWHT  "\x1B[47m"

#define FBLK(x) KBLK x RST
#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BBLK(x) LBLK x RST
#define BRED(x) LRED x RST
#define BGRN(x) LGRN x RST
#define BYEL(x) LYEL x RST
#define BBLU(x) LBLU x RST
#define BMAG(x) LMAG x RST
#define BCYN(x) LCYN x RST
#define BWHT(x) LWHT x RST

#endif  /* _COLORS_ */
