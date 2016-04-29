#include <stdio.h>
#include <stdint.h>

int jiemi(uint32_t i1, uint32_t i2, uint32_t round, uint32_t *i3, uint32_t *i4)
{
	unsigned int R0, R1, /*R2, */R3, R4, R5, R6, R7, /*R8, R9, R10, R11,*/ R12;
	unsigned int SP[4] = { 0 };

	R0 = i1;
	R1 = i2;
	//R0=0xF204C065; // 要解密的第一个4字节
	//R1=0x3527E74C; // 要解密的第二个4字节
	//R3 = round;

	R4 = 0x9E3779B9;
	R4 = R4*round;

	R5 = 0;
	R6 = 0;

	R12 = round; // 0x20

	/*int var_1c = -0x1c;
	int var_20 = -0x20;
	int var_24 = -0x24;
	int var_28 = -0x28;*/

	while (R5 != R12)
	{
		R3 = SP[3]; // SP[0x28+var_1c]; // SP[3], == 0
		R7 = (R0 << 4);

		R5 += 1;

		R7 = R7 + R3;

		R3 = R0 >> 5;

		R3 = R3 + R6;

		R7 = R7 ^ R3; // XOR

		R3 = R4 + R0;

		R3 = R3 ^ R7; // XOR

		R1 = R1 - R3;

		R3 = SP[1]; // SP[0x28+var_24]; // SP[1], 0

		R7 = R1 << 4;

		R7 = R7 + R3;

		//SP[0x28+var_28] = R7; // SP[0]
		SP[0] = R7;

		R7 = SP[2]; // SP[0x28+var_20]; // SP[2]

		R3 = R1 >> 5; // LSRS

		R3 = R3 + R7;

		R7 = SP[0]; // SP[0x28+var_28]; // SP[0]

		R3 = R3 ^ R7; // XOR

		R7 = R1 + R4;

		R7 = R7 ^ R3;

		R3 = 0x61C88647;

		R0 = R0 - R7;

		R4 = R4 + R3;
	}

	//printf("R0=%p, R1=%p\n", R0, R1);
	*i3 = R0;
	*i4 = R1;
	return 0;
}

//int main(void)
//{
//	test_jiemi(0x36f2e439, 0x99b9d19, 0x20);
//	//test_jiemi(0xF204C065, 0x3527E74C, 0x20);
//	//test_jiemi(0xE9575747, 0x71C04EB3, 0x20);
//	return 0;
//}
