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
#include<iomanip>
#include<algorithm>
#include<functional>
#include<exception>
#include<limits>
#include<numeric>
#include<complex>
#include<iterator>
#include<memory>
#include<utility>
#include<new>
#include<bitset>
#include<deque>
#include<queue>
#include<vector>
#include<map>
#include<set>
#include<stack>
#include<list>

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
		if(data < comp) {
			data = comp;
			return true;
		}
		return false;
	}

	inline int abs(const int &data) {
		if (data >= 0) return data;
		return -data;
	}

	int n, m;

	struct Location {
		int x, y;
		Location(){ this -> x = this -> y = 0; }
		Location(const Location& copy) { this -> x = copy.x, this -> y = copy.y; } // Clone one.
		Location(const int &x, const int &y) { this -> x = x, this -> y = y; }
		inline int operator-(const Location& to) const { // Calculate the Manhattan distance.
			return abs(this -> x - to.x) + abs(this -> y - to.y);
		}
	};

	struct Position;

	struct Damage {
		unsigned char bits;
		inline bool hasSet(const int &bit) const {
			return !!(bits & (1 << bit));
		}
		inline void setBit(const int &bit) {
			bits |= (unsigned char)(1 << bit);
		}
		inline void clearBit(const int &bit) {
			bits &= (unsigned char)((~0) ^ (1 << bit));
		}
		inline void goThrough(Damage &dmg) {
			bits |= dmg.bits;
		}
		inline unsigned char serialize() { return bits; }
		inline int getTotalDmg() {
			register int i, ret = 0;
			for(i = 0; i < 5; ++ i) {
				if(this -> bits & (1 << i)) ret += i + 1;
			}
			return ret;
		}
		Damage(){ this -> bits = 0; }
		Damage(const Damage& dmg) { this -> bits = dmg.bits; }
		Damage(const unsigned char serialized) { this -> bits = serialized; }
	};

	struct Position {
		Damage dmg;
		bool canPass;
		Position(){ dmg = Damage(), canPass = false; }
		Position(unsigned char damage, bool pass) { dmg = Damage(damage), canPass = pass; }
		inline void setBit(const int &bit) { this -> dmg.setBit(bit); }
		inline void setPass(const bool pass) { this -> canPass = pass; }
		inline void reset() { dmg = Damage(), canPass = false; }
	} pos[64][64];

	struct Status {
		Location loc;
		Damage dam;
		Status(){ loc = Location(), dam = Damage(); }
		Status(const int &x, const int &y) { loc = Location(x, y), dam = Damage(); }
		Status(const int &x, const int &y, const Damage &dmg) { loc = Location(x, y), dam = Damage(dmg); }
		Status(const Location &l) { loc = Location(l), dam = Damage(); }
		Status(const Location &l, const Damage &dmg) { loc = Location(l), dam = Damage(dmg); }
		Status(const int &serialized) { loc = Location(serialized & 63, (serialized >> 6) & 63), dam = Damage((unsigned char)(serialized >> 12)); }
		inline int serialize() { return loc.x | (loc.y << 6) | ((int)(dam.serialize()) << 12); }
		inline Status* moveUp() {
			if(loc.x) {
				if(!pos[loc.x - 1][loc.y].canPass) return NULL;
				static Damage d;
				d = Damage(this -> dam);
				d.goThrough(pos[loc.x - 1][loc.y].dmg);
				return new Status(loc.x - 1, loc.y, d);
			}
			return NULL;
		}
		inline Status* moveLeft() {
			if(loc.y) {
				if(!pos[loc.x][loc.y - 1].canPass) return NULL;
				static Damage d;
				d = Damage(this -> dam);
				d.goThrough(pos[loc.x][loc.y - 1].dmg);
				return new Status(loc.x, loc.y - 1, d);
			}
			return NULL;
		}
		inline Status* moveDown() {
			if(loc.x < n - 1) {
				if(!pos[loc.x + 1][loc.y].canPass) return NULL;
				static Damage d;
				d = Damage(this -> dam);
				d.goThrough(pos[loc.x + 1][loc.y].dmg);
				return new Status(loc.x + 1, loc.y, d);
			}
			return NULL;
		}
		inline Status* moveRight() {
			if(loc.y < m - 1) {
				if(!pos[loc.x][loc.y + 1].canPass) return NULL;
				static Damage d;
				d = Damage(this -> dam);
				d.goThrough(pos[loc.x][loc.y + 1].dmg);
				return new Status(loc.x, loc.y + 1, d);
			}
			return NULL;
		}
	};

	
	inline void setDmg(const register int x, const register int y, const int &dist, const int &bit) {
		int lx = max(0, x - dist), rx = min(n - 1, x + dist),
			ly = max(0, y - dist), ry = min(m - 1, y + dist);
		register int cx, cy;
		for(cx = lx; cx <= rx; ++ cx) {
			for(cy = ly; cy <= ry; ++ cy) {
				if(abs(x - cx) + abs(y - cy) <= dist) {
					pos[cx][cy].setBit(bit);
				}
			}
		}
	}

	bool done[1 << 18];
	queue<int> Q;
	Location target;

	inline void setPass(const register int x, const register int y, const bool canPass) {
		pos[x][y].setPass(canPass);
	}

	inline void main() {
		int T;
		static char buff[8192];
		fgets(buff, 8192, stdin);
		sscanf(buff, "%d", &T);
		for(int t = 1; t <= T; ++ t) {
			fgets(buff, 8192, stdin);
			sscanf(buff, "%d%d", &n, &m);
			memset(done, 0, sizeof(done));
			while(Q.size()) Q.pop();
			for(register int i = 0; i < n; ++ i) 
				for(register int j = 0; j < m; ++ j)
					pos[i][j].reset();
			for(register int i = 0; i < n; ++ i) {
				fgets(buff, 8192, stdin);
				for(register int j = 0; j < m; ++ j) {
					int serial;
					switch(buff[j]) {
						case 'A':
							setDmg(i, j, 2, 0);
							setPass(i, j, false);
							break;
						case 'B':
							setDmg(i, j, 3, 1);
							setPass(i, j, false);
							break;
						case 'C':
							setDmg(i, j, 0, 2);
							setPass(i, j, true);
							break;
						case 'D':
							setDmg(i, j, 2, 3);
							setPass(i, j, false);
							break;
						case 'E':
							setDmg(i, j, 1, 4);
							setPass(i, j, false);
							break;
						case '$':
							setPass(i, j, true);
							serial = Status(i, j).serialize();
							Q.push(serial);
							done[serial] = true;
							break;
						case '!':
							target = Location(i, j);
						case '.':
							setPass(i, j, true);
							break;
						case '#':
							setPass(i, j, false);
							break;
						default:
							fprintf(stderr, "'%c' not in allowed character!", buff[j]);
							abort();
					}
				}
			}
			static Status cur;
			register Status* next;
			while(Q.size()) {
				cur = Status(Q.front()), Q.pop();
				if((next = cur.moveLeft())) {
					int ns = next -> serialize();
					if(!done[ns]) {
						Q.push(ns);
						done[ns] = true;
					}
					delete next;
				}
				if((next = cur.moveUp())) {
					int ns = next -> serialize();
					if(!done[ns]) {
						Q.push(ns);
						done[ns] = true;
					}
					delete next;
				}
				if((next = cur.moveRight())) {
					int ns = next -> serialize();
					if(!done[ns]) {
						Q.push(ns);
						done[ns] = true;
					}
					delete next;
				}
				if((next = cur.moveDown())) {
					int ns = next -> serialize();
					if(!done[ns]) {
						Q.push(ns);
						done[ns] = true;
					}
					delete next;
				}
			}
			int ans = 0x3F3F3F3F;
			for(register unsigned char d = 0; d < 32; ++ d) {
				Damage dmg = Damage(d);
				if(done[Status(target, dmg).serialize()]) {
					_min(ans, dmg.getTotalDmg());
				}
			}
			printf("Case %d: %d\n", t, (ans < 0x3F3F3F3F ? ans : -1));
		}
	}
}

int main() {
	Solve::main();
	return 0;
}
