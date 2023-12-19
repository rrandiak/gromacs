#include "gromacs/tools/dump_strategy_text.h"

bool DumpStrategyText::available(FILE* fp, const void* p, int indent, const char* title) {}
int DumpStrategyText::pr_title(FILE* fp, int indent, const char* title) {}
int DumpStrategyText::pr_title_n(FILE* fp, int indent, const char* title, int n) {}
int DumpStrategyText::pr_title_nxn(FILE* fp, int indent, const char* title, int n1, int n2) {}