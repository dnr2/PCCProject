#ifndef SUFFIXARRAY_H
#define SUFFIXARRAY_H

#include <cmath>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

class SuffixArray
{
	//TODO considera apenas caractereas alpha numericos!!!
	
	static const int MAXSIZE = 100005;
	
	public:
		
		int RA[MAXSIZE], SA[MAXSIZE], tmpRA[MAXSIZE], tmpSA[MAXSIZE], cnt[MAXSIZE];
		int lcp[DOTS], phi[DOTS];
		char str[DOTS]; // A string de entrada
	
		//dar um sort no rank usando o count sort
		void csort(int k){
			int ind, cub = max(N, 300);
			FILL(cnt, 0);			
			if( SA[i]+k < N) 
				ind = RA[SA[i]+k];	
			
			for(int i = 0; i < N; i++){
				ind = 0;
				if( i+k<N) 
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
			for(int i = 0; i < N; i++){
				ind = 0;
				if( SA[i]+k<N )
					ind = RA[SA[i]+k];
				tmpSA[ cnt[ ind ]++ ] = SA[i];
			}			
			for(int i = 0; i < N; i++){
				SA[i] = tmpSA[i];
			}
		}
		
		// computar o array de sufixo e os ranks em O(n * log(n)) usando radix sort	
		void buildSA(){ 
			for( int i = 0; i < N; i++){
				RA[i] = str[i];
				SA[i] = i;
			}
			int k = 1;
			while(k<N){
				//radix sort no primeiro e no sedundo rank
				csort(k); csort(0); 
				int r = 0;
				tmpRA[SA[0]] = 0;
				//recalcular os ranks de acordo com o rank do SA passado
				for(int i = 1; i < N; i++){
					if(RA[SA[i]]!=RA[SA[i-1]] || RA[SA[i]+k]!=RA[SA[i-1]+k])
						r++;
					tmpRA[SA[i]] = r;
				}				
				for( int i = 0; i < N; i++){
					RA[i] = tmpRA[i];
				}
				//caso ja esteja ordenado, pare
				if(RA[SA[N-1]]==N-1) break;
				k <<= 1;
			}
		}
		
		void buildLCP(){ // computar o LCP em O(n)
			int L=0;
			phi[SA[0]] = -1;			
			for( int i = 1; i < N; i++){
				phi[SA[i]] = SA[i-1];
			}			
			for(int i = 0; i < N; i++ ){
				if(phi[i]==-1){ lcp[RA[i]]=0; continue; }
				while(str[i+L]==str[phi[i]+L]) L++;
				lcp[RA[i]] = L;
				if(L) L--;
			}
		}
		
		
		
		
};

#endif