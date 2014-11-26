#ifndef _TPF_ALGORITHM
#define _TPF_ALGORITHM

#include <string>
#include <bitset>

#include <vector>
#include <queue>
#include <iostream>
#include <cstring>

#define TPF_EXACT 				1
#define TPF_EXACT_PATTERNFILE 	2
#define TPF_APPROXIMATE			4

#define TPF_OK					0
#define TPF_ERR_NO_SUCH_OPTION	1
#define TPF_ERROR_TOO_LONG		2

#define ALPHABET_SIZE 			256

int tpf_find(vector<string> &patterns, string &textfile, int error, int tpf_type, bool count);

//int tpf_aho_corasick(char **i_patterns, int i_pattern_amount, char *i_textfile, char **o_results);

#endif