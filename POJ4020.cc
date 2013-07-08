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

	struct Node {
		int first, second;
		Node() { first = second = 0; }
		Node(const int &f, const int &s) { first = f, second = s; }
		inline bool operator< (const Node &comp) const {
			if(first != comp.first) return first < comp.first;
			return second < comp.second;
		}
	} arr[1048576];

	Node *data[1048576];

	int sum[1048576];

	inline bool hash(const Node *a, const Node *b) {
		if(a -> second != b -> second) return a -> second < b -> second;
		return a -> first < b -> first;
	}

	inline bool comp(const Node *a, const Node *b) {
		if(a -> first != b -> first) return a -> first < b -> first;
		return a -> second < b -> second;
	}
	int n;

#define lowBit(ID) ( (ID) & -(ID) )

#define update(id, val) do { \
	int ID = id; \
	while(ID <= n) { \
		sum[ID] += val; \
		ID += lowBit(ID); \
	} \
} while(0);

#define get(id, result) do { \
	int ID = id; \
	result = 0; \
	while(ID) { \
		result += sum[ID]; \
		ID -= lowBit(ID); \
	} \
} while(0);

	void main() {
		ios_base::sync_with_stdio(false);
		register int i;
		scanf("%d", &n);
		for(i = 0; i < n; ++ i) {
			data[i] = arr + i;
			scanf("%d%d", &arr[i].first, &arr[i].second);
		}
		sort(data, data + n, hash);
		register int curHash = 0, last = -1;
		for(i = 0; i < n; ++ i) {
			if(last != data[i] -> second) { ++ curHash, last = data[i] -> second; }
			data[i] -> second = curHash;
		}
		sort(data, data + n, comp); // Real Sort.
		register long long ans = (long long)n * (n - 1) >> 1;
		update(data[0] -> second, 1);
		for(i = 1; i < n; ++ i) {
			//ans += T -> getSegment(data[i] -> second + 1, curHash + 10);
			int tmp;
			get(data[i] -> second, tmp);
			ans -= tmp;
			update(data[i] -> second, 1);
		}
		cout << ans << endl;
	}
}

int main(int argc, char *argv[]) {
	Solve::main();
	return 0;
}
