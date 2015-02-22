
#include<bits/stdc++.h>

using namespace std;

class SuffixArray
{
	//TODO considera apenas caractereas alpha numericos!!!	
	
	public:
		
		int *RA, *SA, * tmpRA, *tmpSA, *cnt;		
		char *str; // A string de entrada
		int stringSize;
				
		
		//dar um sort no rank usando o count sort
		void csort(int k)
		{
			int ind, cub = max(stringSize, 300);
			memset( cnt, 0, sizeof( cnt ) );			
			
			for(int i = 0; i < stringSize; i++){
				ind = 0;
				if( i+k<stringSize) 
					ind = RA[i+k];				
				cnt[ind]++;
			}			
			for(int i = 1; i < cub; i++){
				cnt[i] += cnt[i-1];
			}
			for(int i=cub-1;i>=1;i--){
				cnt[i] = cnt[i-1];
			}
			cnt[0] = 0;			
			for(int i = 0; i < stringSize; i++){
				ind = 0;
				if( SA[i]+k<stringSize )
					ind = RA[SA[i]+k];
				tmpSA[ cnt[ ind ]++ ] = SA[i];
			}			
			for(int i = 0; i < stringSize; i++){
				SA[i] = tmpSA[i];
			}
		}
		
		// computar o array de sufixo e os ranks em O(n * log(n)) usando radix sort	
		void buildSA()
		{ 
			for( int i = 0; i < stringSize; i++){
				RA[i] = str[i];
				SA[i] = i;
			}
			int k = 1;
			while(k<stringSize){
				//radix sort no primeiro e no sedundo rank
				csort(k); csort(0); 
				int r = 0;
				tmpRA[SA[0]] = 0;
				//recalcular os ranks de acordo com o rank do SA passado
				for(int i = 1; i < stringSize; i++){
					if(RA[SA[i]]!=RA[SA[i-1]] || RA[SA[i]+k]!=RA[SA[i-1]+k])
						r++;
					tmpRA[SA[i]] = r;
				}				
				for( int i = 0; i < stringSize; i++){
					RA[i] = tmpRA[i];
				}
				//caso ja esteja ordenado, pare
				if(RA[SA[stringSize-1]]==stringSize-1) break;
				k <<= 1;
			}
		}
		
		void initialize()
		{
			RA = new int[stringSize + 1]; 
			SA = new int[stringSize + 1];
			tmpRA = new int[stringSize + 1];
			tmpSA = new int[stringSize + 1];
			cnt = new int[stringSize + 1];		
			str = new char[stringSize + 1];
		}
		
		SuffixArray(const string & _str)
		{
			
			stringSize = _str.size();
			initialize();
			strncpy( str, _str.c_str(), _str.size()	);
			buildSA();
		}
		
		bool comparator( int suffix, string & patt ){
			int cmp = strncmp( str + suffix, patt.c_str(), patt.size());
			return cmp < 0;
		}
		
		//string matching usando lower_bound e upper_bound em tempo O( m * log(n) )
		pair<int,int> findOccurrences( const string & patt)
		{			
			int lower = lower_bound( SA, SA + stringSize, patt, comparator);
			if( strncmp( str + SA[lower], patt.c_str(), patt.size()) != 0){				
				return make_pair(-1,-1); //nao encontrado
			}
			int upper = upper_bound( SA, SA + stringSize, patt, comparator);
			if( strncmp( str + SA[upper], patt.c_str(), patt.size()) != 0){
				upper--;	//caso especial
			}
			return make_pair( lower, upper );
		}
};


int main(){
		
	string str = "AGATAGACA$";
	SuffixArray sArray( str );
	sArray.findOccurrences("GA");
	
	return 0;
}