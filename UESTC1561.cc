#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<cassert>
#include<climits>
#include<cctype>
#include<ciso646>
#include<ctime>
#include<cstddef>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<algorithm>
#include<functional>
#include<numeric>
#include<complex>
#include<new>
#include<map>
#include<set>
#include<vector>
#include<deque>
#include<list>
#include<queue>
#include<stack>

using namespace std;

#define EMPTY 127

namespace Solve {
	template <typename T> inline bool _min(T& data, const T& comp) {
		if (data > comp) {
			data = comp;
			return true;
		}
		return false;
	}

	template <typename T> inline bool _max(T& data, const T& comp) {
		if (data < comp) {
			data = comp;
			return true;
		}
		return false;
	}

	map<long long, unsigned char> ANS;
	queue<long long> Q;

	inline long long getID(unsigned char *data) {
		long long ret = 0;
		for(register char i = 0; i < 9; ++ i) {
			ret = (ret << 4) + data[i];
		}
		return ret;
	}

	inline void setID(long long data, unsigned char* const target) {
		for(register char i = 8; i > -1; -- i) {
			target[i] = data & 15;
			data >>= 4;
		}
	}

	inline void swap3(unsigned char &a, unsigned char &b, unsigned char &c) {
		// A, B, C -> B, C, A
		swap(a, c), swap(a, b);
	}

	void initANS() {
		static unsigned char cur[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9}, next[9], ans;
		long long id = getID(cur);
		Q.push(id);
		ANS[id] = 0LL;
		while(Q.size()) {
			id = Q.front();
			setID(id, cur);
			Q.pop();
			ans = ANS[id] + 1;
			memcpy(next, cur, sizeof(cur));
			// U1
			swap3(next[0], next[3], next[6]);
			id = getID(next);
			if(ANS.find(id) == ANS.end()) {
				ANS[id] = ans;
				Q.push(id);
			}
			// U1 + U1 -> D1
			swap3(next[0], next[3], next[6]);
			id = getID(next);
			if(ANS.find(id) == ANS.end()) {
				ANS[id] = ans;
				Q.push(id);
			}
			memcpy(next, cur, sizeof(cur));
			// U2
			swap3(next[1], next[4], next[7]);
			id = getID(next);
			if(ANS.find(id) == ANS.end()) {
				ANS[id] = ans;
				Q.push(id);
			}
			// U2 + U2 -> D2
			swap3(next[1], next[4], next[7]);
			id = getID(next);
			if(ANS.find(id) == ANS.end()) {
				ANS[id] = ans;
				Q.push(id);
			}
			memcpy(next, cur, sizeof(cur));
			// U3
			swap3(next[2], next[5], next[8]);
			id = getID(next);
			if(ANS.find(id) == ANS.end()) {
				ANS[id] = ans;
				Q.push(id);
			}
			// U3 + U3 -> D3
			swap3(next[2], next[5], next[8]);
			id = getID(next);
			if(ANS.find(id) == ANS.end()) {
				ANS[id] = ans;
				Q.push(id);
			}
			memcpy(next, cur, sizeof(cur));
			// L1
			swap3(next[0], next[1], next[2]);
			id = getID(next);
			if(ANS.find(id) == ANS.end()) {
				ANS[id] = ans;
				Q.push(id);
			}
			// L1 + L1 -> R1
			swap3(next[0], next[1], next[2]);
			id = getID(next);
			if(ANS.find(id) == ANS.end()) {
				ANS[id] = ans;
				Q.push(id);
			}
			memcpy(next, cur, sizeof(cur));
			// L2
			swap3(next[3], next[4], next[5]);
			id = getID(next);
			if(ANS.find(id) == ANS.end()) {
				ANS[id] = ans;
				Q.push(id);
			}
			// L2 + L2 -> R2
			swap3(next[3], next[4], next[5]);
			id = getID(next);
			if(ANS.find(id) == ANS.end()) {
				ANS[id] = ans;
				Q.push(id);
			}
			memcpy(next, cur, sizeof(cur));
			// L3
			swap3(next[6], next[7], next[8]);
			id = getID(next);
			if(ANS.find(id) == ANS.end()) {
				ANS[id] = ans;
				Q.push(id);
			}
			// L3 + L3 -> R3
			swap3(next[6], next[7], next[8]);
			id = getID(next);
			if(ANS.find(id) == ANS.end()) {
				ANS[id] = ans;
				Q.push(id);
			}
		}
	}

	unsigned char map[16];
	unsigned char target[16];
	bool done[16];
	::map<long long, unsigned char>::iterator it;
	char buff[1024];

	int getANS(const int depth) {
		int ret = 0x3f3f3f3f;
		if(depth == 9) {
			it = ANS.find(getID(target));
			if(it != ANS.end()) return it -> second;
			return 0x3f3f3f3f;
		}
		if(target[depth] != EMPTY) return getANS(depth + 1);
		register int i;
		for(i = 1; i < 10; ++ i) {
			if(!done[i]) {
				done[i] = true;
				target[depth] = i;
				_min(ret, getANS(depth + 1));
				done[i] = false;
				if(!ret) {
					target[depth] = EMPTY;
					return 0;
				}
			}
		}
		target[depth] = EMPTY;
		return ret;
	}

	void main() {
		ios_base::sync_with_stdio(false);
		int T;
		cin >> T;
		ANS.clear();
		initANS();
		register int i, j, t;
		for(t = 1; t <= T; ++ t) {
			for(i = 1; i < 10; ++ i) {
				cin >> j;
				//cin >> map[i];
				//map[i] -= '0';
				map[j] = i;
			}
			int cnt = 0, last = -1;
			memset(done, 0, sizeof(done));
			for(i = 0; i < 9; ++ i) {
				cin >> buff;
				// cerr << buff[0] << endl;
				if(buff[0] == '*') {
					target[i] = EMPTY;
					++ cnt;
					continue;
				}
				done[target[i] = map[buff[0] - '0']] = true;
				last = i + 1;
			}
			if(cnt == 9) {
				printf("Case #%d: 0\n", t);
				continue;
			}
			if(cnt == 8) {
				i = last;
				printf("Case #%d: %d\n", t, (i % 3 != target[i - 1] % 3) + ((i - 1) / 3 != (target[i - 1] - 1) / 3) );
				continue;
			}
			int ans = getANS(0);
			if(ans == 0x3f3f3f3f) {
				printf("Case #%d: No Solution!\n", t);
				continue;
			}
			printf("Case #%d: %d\n", t, ans);
		}
	}
}

int main() {
	Solve::main();
	return 0;
}
