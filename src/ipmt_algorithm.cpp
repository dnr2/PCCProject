/*#include "ipmt_algorithm.h"
#include "SuffixTree.h"
#include "SuffixArray.h"*/

#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <tuple>
#include <sstream>

#define WINDOW_SIZE		65535
#define BUFFER_SIZE		255

using namespace std;

/*int tpf_find(vector<string> &patterns, string &textfile, int error, int tpf_type, bool count)
{
	int ret = TPF_OK;

	if (tpf_type == TPF_EXACT){ // Boyer Moore
	// int tpf_boyer_moore(string &pat, string &textfile, bool count)
		string pat = patterns[0];

		tpf_boyer_moore(pat, textfile, count);

	} else if (tpf_type == TPF_EXACT_PATTERNFILE){ // Aho-Corasick
		
		tpf_aho_corasick(patterns, textfile, count);
		
	} else if (tpf_type == TPF_APPROXIMATE){ // Wu-Manber
	// int tpf_wu_manber(string &pat, string &textfile, int error, bool count)		
		
		for (string &pat : patterns){		
			int cur_error = min( error, (int) pat.length());
			if( (error < 10 && pat.length() < 63 ) || (error < 2 && pat.length() > 100) ) 
			// if( (error < 2 && pat.length() > 100) ) 
				tpf_wu_manber(pat, textfile, cur_error, count);
			else 
				tpf_sellers(pat, textfile, cur_error, count);			
		}

	} else {
		ret = TPF_ERR_NO_SUCH_OPTION;
	}

	return ret;
}*/

string tuple_encode(int pos, int size, char lit)
{
    ostringstream oss;
    oss << pos << '|';
    oss << size << '|';
    oss << lit << '$';

	return oss.str();
}

int lz77_encode(string text, string &ret)
{
	ret = "";

	uint i = 0;

	string window, buffer;
	int f_pos, f_size;
	char f_lit;

	while (i < text.length()){
		int begin = (i > WINDOW_SIZE)? i-WINDOW_SIZE : 0;

		window = text.substr(begin, i-begin);
		buffer = text.substr(i, BUFFER_SIZE);

		auto tup = make_tuple(0, 0, text[i]);

		for (int size = buffer.length(); size>0; size--){
			int index = window.rfind(buffer.substr(0, size));
			if (index >= 0){
				char lit = '\0';
				if (i + size < text.length()){
					lit = text[i+size];
				}
				get<0>(tup) = window.length()-index-1;
				get<1>(tup) = size;
				get<2>(tup) = lit;
				break;
			}
		}
		i = i + get<1>(tup) + 1;
		tie (f_pos, f_size, f_lit) = tup;
		ret += tuple_encode(f_pos, f_size, f_lit);
	}

	return 0;
}

int lz77_decode(string encoded, string &text)
{
	text = "";
	int pos = 0;
	int size = 0;
	char lit;


	char *pch = strtok(const_cast<char*>(encoded.c_str()), "$");

	while (pch != NULL){
		cout << pch <<endl;
		
		char *pch2 = strchr(pch, '|');
		pch2[0] = '\0';
		pch2 = pch2+1;
		pos = atoi(pch);

		pch = strchr(pch2, '|');
		pch[0] = '\0';
		size = atoi(pch2);
		lit = pch[1];

		text += text.substr(pos, size);
		text += lit;

		pch = strtok(NULL, "$");
	}

	return 0;
}

int main() {
	string str = "ababa";
	string encoded, decoded;
	lz77_encode(str, encoded);
	
	cout << "Original\t" << str << endl; 
	cout << "Encoded\t" << encoded << endl;
	lz77_decode(encoded, decoded);
	
	cout << "Decoded\t" << decoded << endl;
	
	return 0;
}