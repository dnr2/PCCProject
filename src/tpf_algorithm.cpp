#include "tpf_algorithm.h"

int tpf_find(char **i_patterns, int i_pattern_amount, char *i_textfile, int i_tpf_type, char **o_results)
{
	int ret = TPF_OK;
	switch (i_tpf_type){
		case TPF_EXACT:
			break;
		case TPF_EXACT_PATTERNFILE:
			ret = tpf_aho_corasick(i_patterns, i_pattern_amount, i_textfile, o_results);
			break;
		case TPF_APPROXIMATE:
			break;
		default:
			return TPF_ERR_NO_SUCH_OPTION;
			break;
	}
	return ret;
}


int tpf_aho_corasick(char **i_patterns, int i_pattern_amount, char *i_textfile, char **o_results)
{
	return TPF_OK;
}