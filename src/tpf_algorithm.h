#ifndef _TPF_ALGORITHM
#include <string>
#include <bitset>
#include <vector>
#include <queue>
#include <iostream>
#include <cstring>

#define _TPF_ALGORITHM

#define TPF_EXACT 				1
#define TPF_EXACT_PATTERNFILE 	2
#define TPF_APPROXIMATE			4

#define TPF_OK					0
#define TPF_ERR_NO_SUCH_OPTION	1

#define ALPHABET_SIZE 			256

int tpf_find(char **i_patterns, int i_pattern_amount, char *i_textfile, int i_tpf_type, char **o_results);

int tpf_aho_corasick(char **i_patterns, int i_pattern_amount, char *i_textfile, char **o_results);

#endif