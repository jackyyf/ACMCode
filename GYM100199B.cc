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

	template<class T> inline bool _min(T& data, const T& comp) {
		if (comp < data) {
			data = comp;
			return true;
		}
		return false;
	}

	template<class T> inline bool _max(T& data, const T& comp) {
		if (data < comp) {
			data = comp;
			return true;
		}
		return false;
	}


	template<class T> inline vector<T>& operator<<(vector<T> &container, const T &data) {
		container.push_back(data);
		return container;
	}

	template<class T> inline list<T>& operator<<(list<T> &container, const T &data) {
		container.push_back(data);
		return container;
	}

	template<class T> inline list<T>& operator>>(list<T> &container, T &data) {
		data = container.front();
		container.pop_front();
		return container;
	}

	typedef long long Edge;

	list<Edge> edge_list;
	vector<int> edge[256];
	int in[256], out[256];
	int lim[256][256];
	int stream[256][256];
	size_t last[256];
	int level[256];
	size_t size[256];

#define S 0
#define T 255
#define INF 0x3f3f3f3f
#define LAST last[cur]
#define SIZE size[cur]

#define buildEdge(from, to) ( ((long long)(from) << 32) + (to) )
#define recoverEdge(edge, from, to) do { from = (int)(edge >> 32), to = (int)(edge & (~0U)); } while (0)

	inline void addEdge(const int &from, const int &to, const int &low, const int &high) {
		lim[from][to] = low;
		in[to] += low;
		out[from] += low;
		edge[from] << to;
		edge[to] << from;
		stream[from][to] = high - low;
	}

	inline bool relabel() {
		memset(level, 0xFF, sizeof(level));
		level[S] = 0;
		static queue<int> Q;
		while(Q.size()) Q.pop();
		Q.push(S);
		while(Q.size()) {
			int cur = Q.front(); Q.pop();
			for(register size_t i = 0, length = SIZE; i < length; ++ i) {
				int next = edge[cur][i];
				if(level[next] == -1 && stream[cur][next] > 0) {
					level[next] = level[cur] + 1;
					Q.push(next);
				}
			}
		}
		return level[T] != -1;
	}

	int flow(const int &cur, const int &f) {
		if(__builtin_expect(cur == T, 0)) return f;
		int curf = 0, left = f;
		for(;LAST < SIZE; ++ LAST) {
			int next = edge[cur][LAST];
			if(level[next] == level[cur] + 1 && stream[cur][next] > 0) {
				int nextf = flow(next, min(left, stream[cur][next]));
				curf += nextf, left -= nextf, stream[cur][next] -= nextf, stream[next][cur] += nextf;
				if(left == 0) return f;
			}
		}
		return curf;
	}

	inline void dinic() {
		while (relabel()) {
			memset(last, 0, sizeof(last));
			flow(S, INF);
		}
	}

	inline void main() {
#ifdef ONLINE_JUDGE
		freopen("cooling.in", "r", stdin);
		freopen("cooling.out", "w", stdout);
#endif
		ios_base::sync_with_stdio(false);
		int n, m;
		cin >> n >> m;
		register int i;
		for(i = 0; i < m; ++ i) {
			int from, to, low, high;
			cin >> from >> to >> low >> high;
			edge_list << buildEdge(from, to);
			addEdge(from, to, low, high);
		}
		for(i = 1; i <= n; ++ i) {
			if(in[i] > out[i]) {
				addEdge(S, i, 0, in[i] - out[i]);
				continue;
			}
			addEdge(i, T, 0, out[i] - in[i]);
		}
		for(i = 0; i < 256; ++ i) {
			size[i] = edge[i].size();
		}
		dinic();
		for(i = 0; i < size[0]; ++ i) {
			int next = edge[0][i];
			if(stream[0][next]) {
				cout << "NO" << endl;
				return ;
			}
		}
		cout << "YES" << endl;
		Edge now;
		int from, to;
		while(edge_list.size()) {
			edge_list >> now;
			recoverEdge(now, from, to);
			cout << lim[from][to] + stream[to][from] << endl;
		}
	}
}

int main () {
	Solve::main();
	return 0;
}
