#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<cassert>
#include<climits>
#include<ciso646>
#include<cctype>
#include<ctime>
#include<cstddef>
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<algorithm>
#include<functional>
#include<numeric>
#include<complex>
#include<exception>
#include<iterator>
#include<limits>
#include<locale>
#include<memory>
#include<new>
#include<stdexcept>
#include<string>
#include<typeinfo>
#include<utility>
#include<valarray>
#include<bitset>
#include<deque>
#include<list>
#include<map>
#include<queue>
#include<set>
#include<stack>
#include<vector>

using namespace std;

namespace Solve {
	inline unsigned char serialize(const register char* tile) {
		register unsigned char ret;
		ret = tile[0] - '0'; // 1 ~ 9
		switch(tile[1]) {
			case 'm':
				return ret | 16;
			case 's':
				return ret | 32;
			case 'p':
				return ret | 48;
		}
		return ret;
	}

	inline char* toString(const register int &serialized) {
		static char data[4] = {'c', 'm', 's', 'p'};
		//static char buff[16];
		register char *ret = (char *)malloc(3);
		ret[0] = (serialized & 15) + '0';
		ret[1] = data[serialized >> 4];
		ret[2] = '\0';
		return ret;
	}

	vector<char *> Ans;

	unsigned char cnt[64], _cnt[64];
	const static unsigned char tileList[] = {
		17, 18, 19, 20, 21, 22, 23, 24, 25, // 1m ~ 9m
		33, 34, 35, 36, 37, 38, 39, 40, 41, // 1s ~ 9s
		49, 50, 51, 52, 53, 54, 55, 56, 57, // 1p ~ 9p
		1, 2, 3, 4, 5, 6, 7, // 1c ~ 7c
	};

	bool hasPair = false;

#define Cnt _cnt[Depth]

	bool checkTile(const int depth) {
		if(depth >= sizeof tileList) return true;
		register unsigned char Depth = tileList[depth];
		register unsigned char CNT = _cnt[Depth];
		if(!CNT) return checkTile(depth + 1);
		if(CNT > 1) {
			if(CNT > 2) {
				Cnt -= 3;
				if(checkTile(depth)) return true;
				Cnt += 3;
			}
			if(!hasPair) {
				Cnt -= 2, hasPair = true;
				if(checkTile(depth)) return true;
				Cnt += 2, hasPair = false;
			}
		}
		if(Depth > 7) { // Not characters.
			if(_cnt[Depth + 1] >= CNT && _cnt[Depth + 2] >= CNT) {
				_cnt[Depth + 1] -= CNT, _cnt[Depth + 2] -= CNT;
				if(checkTile(depth + 1)) return true;
				_cnt[Depth + 1] += CNT, _cnt[Depth + 2] += CNT;
			}
		}
		return false;
	}

	inline bool isAQD() { // An Qi Dui?
		register unsigned char i;
		for(i = 0; i < sizeof tileList; ++ i) {
			if(cnt[tileList[i]] & 5) return false;
		}
		return true;
	}

	const unsigned char JBZ[] = {
		1, 2, 3, 4, 5, 6, 7, // 1c ~ 7c
		17, 25, // 1m, 9m
		33, 41, // 1s, 9s
		49, 57, // 1p, 9p
	};

	inline bool isJBZ() { // Jiu Bu Zhan?
		register unsigned char i, kinds = 0, total = 0, cur;
		for(i = 0; i < sizeof JBZ; ++ i) {
			cur = cnt[JBZ[i]];
			total += cur;
			kinds += !!cur;
		}
		return kinds == 13 && total == 14;
	}

	void main() {
		//ios_base::sync_with_stdio(false);
		register int i, j;
		int T;
		scanf("%d\n", &T);
		while(T --) {
			memset(cnt, 0, sizeof cnt);
			static char buff[1024];
			register char *ptr = buff;
			gets(buff);
			for(i = 0; i < 13; ++ i) {
				++ cnt[serialize(ptr)];
				ptr += 3;
			}
			while (Ans.size()) free(Ans.back()), Ans.pop_back();
			for(i = 0; i < sizeof tileList; ++ i) {
				register unsigned char cur = tileList[i];
				if (cnt[cur] > 3) continue;
				++ cnt[cur];
				memcpy(_cnt, cnt, 64); hasPair = false;
				if(checkTile(0) || isJBZ() || isAQD()) Ans.push_back(toString(cur));
				-- cnt[cur];
			}
			size_t sz = Ans.size();
			if(!sz) {
				puts("Nooten");
				continue;
			}
			printf("%u", sz);
			for(i = 0; i < sz; ++ i) {
				printf(" %s", Ans[i]);
			}
			puts("");
		}
	}
}

int main(int argc, char *argv[]) {
	Solve::main();
	return 0;
}
