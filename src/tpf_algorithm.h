#ifndef _TPF_ALGORITHM
#define _TPF_ALGORITHM

#include <string>

#include <vector>
#include <queue>
#include <iostream>
#include <cstring>

#include <getopt.h>
#include <stdlib.h>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include <assert.h>


#define TPF_EXACT 				1
#define TPF_EXACT_PATTERNFILE 	2
#define TPF_APPROXIMATE			4

#define TPF_OK						0
#define TPF_ERR_NO_SUCH_OPTION		1
#define TPF_ERROR_TOO_LONG			2
#define TPF_ERROR_READING_FILE		4

#define ALPHABET_SIZE 			256

using namespace std;

int tpf_find(vector<string> &patterns, string &textfile, int error, int tpf_type, bool count);

int tpf_wu_manber(string &pat, string &textfile, int error, bool count);

int tpf_boyer_moore(string &pat, string &textfile, bool count);

//int tpf_aho_corasick(char **i_patterns, int i_pattern_amount, char *i_textfile, char **o_results);

#endif