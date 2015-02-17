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
	//TODO dealing with 256 chars
	
	static const int MAXSIZE = 100005;
	
	public:
		
		int RA[MAXSIZE], SA[MAXSIZE], tmpRA[MAXSIZE], tmpSA[MAXSIZE], cnt[MAXSIZE];
		int lcp[DOTS], phi[DOTS];
		char str[DOTS]; // A string de entrada
	
		void csort(int k){
			int cub = max(N, 300);	// take the maximum of the length of the strings and the number of unique chars
			FILL(cnt, 0);
			//REP(i,0,N) cnt[i+k<N?RA[i+k]:0]++;
			for(int i = 0; i < N; i++){
				cnt[i+k<N?RA[i+k]:0]++;
			}
			// REP(i,1,cub) cnt[i] += cnt[i-1];
			for(int i = 1; i < cub; i++){
				cnt[i] += cnt[i-1];
			}
			for(int i=cub-1;i>=1;i--){
				cnt[i] = cnt[i-1];
			}
			cnt[0] = 0;
			// REP(i,0,N) tmpSA[ cnt[SA[i]+k<N?RA[SA[i]+k]:0]++ ] = SA[i];
			for(int i = 0; i < N; i++){
				tmpSA[ cnt[SA[i]+k<N?RA[SA[i]+k]:0]++ ] = SA[i];
			}
			// REP(i,0,N) SA[i] = tmpSA[i];
			for(int i = 0; i < N; i++){
				SA[i] = tmpSA[i];
			}
		}
		
		void buildSA(){ // compute SA and RA
			// REP(i,0,N){
			for( int i = 0; i < N; i++){
				RA[i] = str[i];
				SA[i] = i;
			}
			int k = 1;
			while(k<N){
				csort(k); csort(0); 	// cannot reverse order
				int r = 0;
				tmpRA[SA[0]] = 0;
				// REP(i,1,N){
				for(int i = 1; i < N; i++){
					if(RA[SA[i]]!=RA[SA[i-1]] || RA[SA[i]+k]!=RA[SA[i-1]+k])
						r++;
					tmpRA[SA[i]] = r;
				}
				// REP(i,0,N) RA[i] = tmpRA[i];
				for( int i = 0; i < N; i++){
					RA[i] = tmpRA[i];
				}
				if(RA[SA[N-1]]==N-1) break;
				k <<= 1;
			}
		}
		
		void buildLCP(){ // compute lcp and phi (optional); call this after buildSA()
			int L=0;
			phi[SA[0]] = -1;
			// REP(i,1,N) phi[SA[i]] = SA[i-1];
			for( int i = 1; i < N; i++){
				phi[SA[i]] = SA[i-1];
			}
			// REP(i,0,N){
			for(int i = 0; i < N; i++ ){
				if(phi[i]==-1){ lcp[RA[i]]=0; continue; }
				while(str[i+L]==str[phi[i]+L]) L++;
				lcp[RA[i]] = L;
				if(L) L--;
			}
		}
		
};

#endif