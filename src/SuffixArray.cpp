
#include<bits/stdc++.h>

using namespace std;

class SuffixArray
{
	
	public:
		
		int *RA, *SA, * tmpRA, *tmpSA, *cnt;		
		char *str; // A string de entrada
		int stringSize;
				
		
		//dar um sort no rank usando o count sort
		void csort(int k)
		{
			int ind, cub = max(stringSize, 300);
			memset( cnt, 0, (stringSize + 400) * sizeof( cnt ) );			
			
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
			RA = new int[stringSize + 400]; 
			SA = new int[stringSize + 400];
			tmpRA = new int[stringSize + 400];
			tmpSA = new int[stringSize + 400];
			cnt = new int[stringSize + 400];		
			str = new char[stringSize + 400];
		}
		
		SuffixArray(string & _str)
		{
			
			stringSize = _str.size();
			initialize();
			strncpy( str, _str.c_str(), _str.size()	);
			buildSA();			
		}
		
		
		//string matching usando lower_bound e upper_bound em tempo O( m * log(n) )
		void findOccurrences( string & patt )
		{			
			
			int start, end;
			int lo = 0, hi = stringSize-1, mid = lo;	
			while( lo < hi ){
				mid = (lo+hi) / 2;
				if( strncmp( str + SA[mid], patt.c_str(), patt.size()) >= 0){
					hi = mid;
				} else{
					lo = mid + 1;
				}
			}
			start = lo;
			if (strncmp( str + SA[lo], patt.c_str(), patt.size()) != 0) 
				return; //nao encontrado
			
			lo = 0, hi = stringSize-1;
			while( lo < hi ){
				mid = (lo + hi) / 2;
				if( strncmp( str + SA[mid], patt.c_str(), patt.size()) > 0){
					hi = mid;
				} else{
					lo = mid + 1;
				}
			}
			
			if( strncmp( str + SA[hi], patt.c_str(), patt.size() ) )
				hi--;
			end = hi;
			
			printf("=== encontrado padrao: %s ===\n", patt.c_str());
			for( int i = start; i < end+1; i++){				
				printf("ocorrencia na posicao: %d\n", SA[i] );
			}
		}
};


int main(){
		
	string str = "GATAGACA$";

	SuffixArray sArray( str );
	string patt = "GATAGA";
	sArray.findOccurrences(patt);
	
	return 0;
}