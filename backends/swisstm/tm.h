#ifdef WLPDSTM
#include "tm_wlpdstm.h"
#elif defined TINYSTM
#include "tm_tinystm.h"
#elif defined RSTM
#include "tm_rstm.h"
#elif defined TANGER
#include "tm_tanger.h"
#elif defined ICC
#include "tm_icc.h"
#else
#include "tm_original.h"
#endif
