#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<assert.h>
#include<limits.h>
#include<time.h>
#include<ctype.h>
#include<stddef.h>
#include<iso646.h>

char orig[8][8];
char data[4096][4096];

void copyData(unsigned int size, unsigned int n) {
	register unsigned int i, j, k;
	for(i = 0; i < n; ++ i)
		for(j = 0; j < n; ++ j)
			if(i || j) {
		 		if(orig[i][j] != ' ')
					for(k = 0; k < size; ++ k) memcpy(data[k + size * i] + (size * j), data[k], size);
				else
					for(k = 0; k < size; ++ k) memset(data[k + size * i] + (size * j), 32, size);
			}
	if(orig[0][0] == ' ') {
		for(k = 0; k < size; ++ k) memset(data[k], 32, size);
	}
}

int main(void) {
	unsigned int n;
	register unsigned int i;
	unsigned int depth, tn;
	static char buff[131072];
	while(scanf("%u", &n), n) {
		gets(buff);
		memset(orig, 32, sizeof(orig)); // Spaces
		memset(data, 32, sizeof(data)); // Spaces
		for(i = 0; i < n; ++ i) {
			fgets(buff, 131072, stdin);
			memcpy(orig[i], buff, n);
			memcpy(data[i], buff, n);
		}
			scanf("%u", &depth);
		tn = n;
		for(i = 1; i < depth; ++ i) {
			copyData(n, tn);
			n *= tn;
		}
		for(i = 0; i < n; ++ i) {
			fwrite(data[i], 1, n, stdout);
			puts("");
		}
		n = 0;
	}
	return 0;
}
