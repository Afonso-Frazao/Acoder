#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	char num, shift;
	char left, right;

	num=(char)atoi(argv[1]);
	shift=(char)atoi(argv[2]);

	printf("num: %hhd %08hhb\nshift: %hhd\n", num, num, shift);

	left=num<<(8-shift);
	right=num>>shift;

	printf("left: %hhd %08hhb\nright: %hhd %08hhb\n", left, left, right, right);

	num=left | (right&((1<<(8-shift))-1));

	printf("shifted: %hhd %08hhb\n", num, num);

	return 0;
}
