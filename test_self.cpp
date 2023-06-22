#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>

#include <chrono>

using namespace std;

// ��ʱ����
template<typename Func, typename... Args>
void timing(Func func, Args&&... args)
{
	auto start = chrono::system_clock::now();
	func(args...);
	auto end = chrono::system_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "ִ��ʱ�䣺" << duration.count() << " ΢��" << endl;
}


#define u8 unsigned char
#define u32 unsigned long
void getRK(u32 MK[], u32 K[], u32 RK[]); // ����Կ��ȡ�㷨
void encryptSM4(u32 X[], u32 RK[], u32 Y[]); // �����㷨
void decryptSM4(u32 X[], u32 RK[], u32 Y[]); // �����㷨

bool hexToDec(const char hex[32], u32 dec[4]);
bool decToHex(u32 dec[4], char hex[33]);

//#define test_1
//#define test_2

#ifdef test_1

int sm4_ecb(const char enc_before[], const char enc_after[]) {

	u32 X[4]; // ����
	u32 MK[4]; // ��Կ
	u32 RK[32]; // ����Կ
	u32 K[4]; // �м�����
	u32 Y[4]; // ����
	short i; // ��ʱ����

	MK[0] = 0x01234657;
	MK[1] = 0x89abcdef;
	MK[2] = 0xfedcba98;
	MK[3] = 0x76543210;
	
	getRK(MK, K, RK);
	
	// ��txt�ļ�
	std::ifstream inFile(enc_before);
	std::ofstream outFile(enc_after);
	// ��ȡ8���ַ������洢���ַ�����char������
	int count = 0;

	char buffer[33] = { 0 };
	while (inFile.read(buffer, 32)) {
		hexToDec(buffer, X);//�Ѷ�ȡ��16�����ļ�ת��Ϊsm4�����u32[4]
		
		//����
		encryptSM4(X, RK, Y);

		char M1[33] = { 0 };
		decToHex(Y, M1);//��sm4���������u32[4]��ת��Ϊ16������

		outFile << M1;//д����ܺ���ļ�
		
		
	}
	printf("end\n");
	inFile.close();
	outFile.close();
	return 0;
}


int main() {

	timing(sm4_ecb,"video.txt", "enc.txt");

	return 0;
}

#endif


#ifdef test_2
int main(void) {

	//return 0;
	u32 X[4]; // ����
	u32 MK[4]; // ��Կ
	u32 RK[32]; // ����Կ
	u32 K[4]; // �м�����
	u32 Y[4]; // ����
	short i; // ��ʱ����

	MK[0] = 0x01234657;
	MK[1] = 0x89abcdef;
	MK[2] = 0xfedcba98;
	MK[3] = 0x76543210;

	getRK(MK, K, RK);

	char s[33] = "EFE5A6305C139D9814C5656D2FC40988";

	printf("�����ܵ�32�ֽ��ַ�����%s\n", s);
	hexToDec(s, X);//�Ѷ�ȡ��16�����ļ�ת��Ϊsm4�����u32[4]

	//����
	encryptSM4(X, RK, Y);

	char M[33] = { 0 };
	decToHex(Y, M);//��sm4���������u32[4]��ת��Ϊ16������
	printf("���ܺ��32�ֽ��ַ�����%s\n", M);

	//����

	decryptSM4(Y, RK, X);
	decToHex(X, M);//��sm4���������u32[4]��ת��Ϊ16������
	printf("���ܺ��32�ֽ��ַ�����%s\n\n", M);
	printf("end\n");

	return 0;
}

#endif



bool hexToDec(const char hex[32], u32 dec[4]) {
	char c;
	for(int j = 0; j < 4; j++){
		for (int i = 0; i < 8; i++) {
			c = hex[i+j*8];
			if (c >= '0' && c <= '9') {
				dec[j] = dec[j] * 16 + (c - '0');
			}
			else if (c >= 'a' && c <= 'f') {
				dec[j] = dec[j] * 16 + (c - 'a' + 10);
			}
			else if (c >= 'A' && c <= 'F') {
				dec[j] = dec[j] * 16 + (c - 'A' + 10);
			}
			else {
				// ����ַ����а�����16�����ַ����򷵻�0
				return false;
			}
		}
	}
	return true;
}

bool decToHex(u32 dec[4], char hex[33]) {
	u32 d, r;
    for (int i = 0; i < 4; i++) {
		d = dec[i];
		for (int j = 7; j >= 0; j--) {
			r = d % 16;
			d = d / 16;
			if (r < 10) {
				hex[i * 8 + j] = r + '0';
			}
			else {
				hex[i * 8 + j] = r + 'a' - 10;
			}
		}
    }
    hex[32] = '\0'; // ��hexβ����ӿ��ַ�
    return true;
}


//int main() {
//	char s[33] = "00000001111fec0011011d01111fec00";
//	u32 m[4];
//	hexToDec(s, m);
//	printf("%d %d %d %d\n", m[0], m[1], m[2], m[3]);
//	decToHex(m, s);
//	printf("%s", s);
//}