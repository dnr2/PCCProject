#ifndef _TPF_ALGORITHM
#define _TPF_ALGORITHM

#include <bits/stdc++.h>

//#include "SuffixTree.cpp"

/*#include <getopt.h>
#include <dirent.h>
#include <sys/types.h>*/



#define IPMT_INDEX_TREE 			1
#define IPMT_INDEX_ARRAY			2
#define IPMT_SEARCH					4

#define IPMT_OK						0
#define IPMT_ERR_NO_SUCH_OPTION		1
#define IPMT_ERROR_TOO_LONG			2
#define IPMT_ERROR_READING_FILE		4

#define ALPHABET_SIZE 			256

using namespace std;

int ipmt_index_tree(string &textfile, int error, int tpf_type, bool count);
int ipmt_index_array(string &textfile, int error, int tpf_type, bool count);
int ipmt_search(vector<string> &patterns, string &textfile, int error, int tpf_type, bool count);

void lzw_encode(const string &text, vector<int> &ret);
string lzw_decode(const vector<int> encoded);


#endif