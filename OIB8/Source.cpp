
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <math.h>
#define uch unsigned char

//bmp file: 564*740; 24
//max bit: 1 252 020 for symbols
//kod for 1, 2, 4
//kod for 1: 156502 symbols
//kod for 2: 313005 symbols
//kod for 4: 626010 symbols


//55 symbol person for biter;
//56-57-58-59-60 symbols person for size_text(10011000110101011010); 20; 5 symbols;
//if 1biter = 8 bit
//if 2biter = 7 bit
//if 4bitter = 6 bit


//11111000
//11111001
//11111110
//11111011

int menu(void) {
	int number = 0;

	printf("Выберите действие:\nЗашифровать сообщение -- 1\nДешифровать сообщение -- 2\n");
	scanf("%d", &number);
	return number;
}

int size_text(void) {

	FILE* message;
	int crt = 0;

	message = fopen("message.txt", "r");
	fseek(message, 0, SEEK_END);
	crt = ftell(message);
	fseek(message, 0, SEEK_SET);
	fclose(message);
	return crt;


}

bool cheak_size(int biter) {
	switch (biter) {
	case 1:
		if (size_text() > 156502) return false;
	case 2:
		if (size_text() > 313005) return false;
	case 4:
		if (size_text() > 626010) return false;
	case 8:
		if (size_text() > 1252020) return false;
	default:
		return false;
	}
}

int bitline() {
	int biter = 0;
	printf("\n-----------------------------------------------------------------------------------------------------------------------");
	printf("Выберите количество бит, количество которых будет заменено в цвете:\n1 бит -- 1\n2 бита -- 2\n4 бита -- 4\n8 бит -- 8\n");
	printf("-----------------------------------------------------------------------------------------------------------------------\n");
	scanf("%d", &biter);
	return biter;
}

int size_view(uch sym_1, uch sym_2, uch sym_3, uch sym_4, uch sym_5)
{

	int crt_message = pow(2, 19), crt_message_1 = pow(2, 19), crt_message_2 = pow(2, 15), crt_message_3 = pow(2, 11), crt_message_4 = pow(2, 7), crt_message_5 = pow(2, 3), lsd = 0;
	crt_message_1 = crt_message_1 << 1;
	crt_message_2 = crt_message_2 >> 20;
	crt_message_3 = crt_message_3 >> 20;
	crt_message_4 = crt_message_4 >> 20;
	crt_message_5 = crt_message_5 >> 20;

	// 1
	sym_1 = (sym_1 << 4);
	sym_1 = (sym_1 >> 4);
	crt_message_1 = crt_message_1 | sym_1;
	crt_message_1 = crt_message_1 << 16;

	//2
	sym_2 = (sym_2 << 4);
	sym_2 = (sym_2 >> 4);
	crt_message_2 = crt_message_2 | sym_2;
	crt_message_2 = crt_message_2 << 12;

	//3
	sym_3 = (sym_3 << 4);
	sym_3 = (sym_3 >> 4);
	crt_message_3 = crt_message_3 | sym_3;
	crt_message_3 = crt_message_3 << 8;

	//4
	sym_4 = (sym_4 << 4);
	sym_4 = (sym_4 >> 4);
	crt_message_4 = crt_message_4 | sym_4;
	crt_message_4 = crt_message_4 << 4;

	//5
	sym_5 = (sym_5 << 4);
	sym_5 = (sym_5 >> 4);
	crt_message_5 = crt_message_5 | sym_5;
	crt_message_5 = crt_message_5 << 0;


	crt_message = crt_message_1 | crt_message_2 | crt_message_3 | crt_message_4 | crt_message_5;
	return crt_message;
}

void coder(int biter, int crt)
{
	FILE* input; FILE* message; FILE* output;
	int sec = 0, cnt = 0;
	input = fopen("Test.bmp", "rb");
	output = fopen("Test_out.bmp", "wb");
	message = fopen("message.txt", "rb");
	uch lid, sex, important;
	uch crt_mask_1 = 240, crt_mask_2;
	for (int i = 0; i < 55; i++) {
		fread(&lid, 1, 1, input);
		fwrite(&lid, 1, 1, output);
	}
	fread(&lid, 1, 1, input);
	lid = lid & 240; //Освобождаем нули
	//printf("%d\n", lid);
	switch (biter) {
	case 1: lid = lid | 1; break;
	case 2: lid = lid | 2; break;
	case 4: lid = lid | 4; break;
	case 8: lid = lid | 8; break;
	}
	fwrite(&lid, 1, 1, output);
	//printf("%d\n", lid);
	
	for (int i = 0; i < 5; i++) {

		fread(&lid, 1, 1, input);
		lid = lid & crt_mask_1; //Освобождаем последнии 4 бита

		lid = lid | ((crt << sec) >> 16);
		sec += 4;

		fwrite(&lid, 1, 1, output);
	}

	switch (biter) {
	case 1: cnt = 8; break;
	case 2: cnt = 4; break;
	case 4: cnt = 2; break;
	case 8: cnt = 1; break;
	}

	//	while (!feof(input)) {
		//	fread(&lid, 1, 1, input);
		//	printf("%d   ", lid);
		//}



	crt_mask_1 = 255 << biter;
	sec = 0;
	while (!feof(message)) {
		fread(&sex, 1, 1, message);
		crt_mask_2 = sex;
		sec = 0;


		//	fread(&lid, 1, 1, input);
		//	lid = lid & crt_mask_1;
		//	lid = lid | ((sex << sec) >> 4)


		for (int i = 0; i < cnt; i++) {
			fread(&lid, 1, 1, input);
			//	printf("%d -- ", crt_mask_2);
			//	printf("%d  ", lid);
			lid = lid & crt_mask_1; //Освободили последнии биты
		//	printf("%d  ", lid);

			important = (crt_mask_2 << sec);

			lid = lid | (important >> (8 - biter));
			//	printf("%d\n", lid);
			sec += biter;
			//crt_mask_2 = sex;



			//printf("%d  ", lid);
			fwrite(&lid, 1, 1, output);
		}

	}

	while (!feof(input)) {
		fread(&sex, 1, 1, input);
		fwrite(&sex, 1, 1, output);
	}

	fclose(input);
	fclose(output);
	fclose(message);
}

void decoder()
{
	FILE* view; FILE* message;
	uch sym, sym_1, sym_2, sym_3, sym_4, sym_5, mask, kol_color, mask_test = 15;
	int crt = 0, biter, size_message;
	view = fopen("Test_out.bmp", "rb");
	message = fopen("YOU_MESSAGE.txt", "w");

	for (int i = 0; i < 56; i++) fread(&sym, 1, 1, view);

	//printf("%d\n", sym);
	biter = sym & mask_test;
	//printf("%d\n", biter);
	mask = 255 >> (8 - biter);
	//printf("%d\n", mask);


	if (biter == 1) kol_color = 8;
	if (biter == 2) kol_color = 4;
	if (biter == 4) kol_color = 2;
	if (biter == 8) kol_color = 1;


	fread(&sym_1, 1, 1, view);
	fread(&sym_2, 1, 1, view);
	fread(&sym_3, 1, 1, view);
	fread(&sym_4, 1, 1, view);
	fread(&sym_5, 1, 1, view);

	size_message = size_view(sym_1, sym_2, sym_3, sym_4, sym_5);

	if (size_message == 0) {
		system("cls");
		printf("------------------------------");
		printf("В данном файле отсутствует сообщение!");
		printf("------------------------------");
		return;
	}
	//printf("%d\n", size_message);

	//uch sim;
	//fread(&sim, 1, 1, view);
	//printf("%d ", sim);
	//fread(&sim, 1, 1, view);
	//printf("%d\n", sim);

	

	while (!feof(view)) {
		if (size_message != 0)
		{
			int kol = biter;
			unsigned char sim_text = 0, sim = 0;
			for (int i = 0; i < kol_color; i++)
			{
				fread(&sim, 1, 1, view);
				
				sim = sim & mask;
				//printf("%d ", sim); 
				
				sim_text = (sim_text << kol);
				sim_text = sim_text | sim;
				//printf("- %d -\n", sim_text);
				
			}
			
			printf("%c", sim_text);
			size_message--;
		}
		if (size_message == 0)
			break;
	

	}



	fclose(view);
	fclose(message);
}

int main(void)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");

	system("color 02");

	
	int biter;

	switch (menu()) {
	case 1: 
		system("cls");
		biter = bitline();
		coder(biter, size_text());
		system("cls");
		printf("------------------------------");
		printf("Ваше сообщение закодировано!");
		printf("------------------------------");
		break;
	case 2:
		decoder();
		break;
	}


	//11110000 when << 4
//	printf("%d\n", a); //00110011
//	printf("%d", (240 >> 6));
	//11111100
	//111111
	//	while (!feof(input)) {
	//		kol++;
	//
	//		fread(&n, 1, 1, input);
	//		if (kol > 55 & kol < 70) {
	//			printf("%d\t", n);
	//		}	
	//	}



}





/*FILE* input; FILE* message; FILE* output;
	int sec = 0, cnt = 0;
	input = fopen("Test.bmp", "rb");
	output = fopen("Test_out.bmp", "wb");
	message = fopen("message.txt", "rb");
	uch lid, sex, important;
	uch crt_mask_1 = 240, crt_mask_2;
	for (int i = 0; i < 55; i++) {
		fread(&lid, 1, 1, input);
		fwrite(&lid, 1, 1, output);
	}
	fread(&lid, 1, 1, input);
	lid = lid & 240; //Освобождаем нули

	switch (biter) {
	case 1: lid = lid | 1; break;
	case 2: lid = lid | 2; break;
	case 4: lid = lid | 4; break;
	case 8: lid = lid | 8; break;
	}
	fwrite(&lid, 1, 1, output);

	for (int i = 0; i < 5; i++) {

		fread(&lid, 1, 1, input);
		lid = lid & crt_mask_1; //Освобождаем последнии 4 бита

		lid = lid | ((crt << sec) >> 16);
		sec += 4;

		fwrite(&lid, 1, 1, output);
	}

	switch (biter) {
	case 1: cnt = 8; break;
	case 2: cnt = 4; break;
	case 4: cnt = 2; break;
	case 8: cnt = 1; break;
	}

//	while (!feof(input)) {
	//	fread(&lid, 1, 1, input);
	//	printf("%d   ", lid);
	//}

	crt_mask_1 = 255 << biter;
	sec = 0;





	fread(&sex, 1, 1, message);
	crt_mask_2 = sex;


	//	fread(&lid, 1, 1, input);
	//	lid = lid & crt_mask_1;
	//	lid = lid | ((sex << sec) >> 4)

		
	for (int i = 0; i < cnt; i++) {
		fread(&lid, 1, 1, input);
		//	printf("%d -- ", crt_mask_2);
		//	printf("%d  ", lid);
		lid = lid & crt_mask_1; //Освободили последнии биты
		//	printf("%d  ", lid);

		important = (crt_mask_2 << sec);

		lid = lid | (important >> (8 - biter));
		//	printf("%d\n", lid);
		sec += biter;
		//crt_mask_2 = sex;



		//	printf("%d ", lid);
		fwrite(&lid, 1, 1, output);


	}

	while (!feof(input)) {
		fread(&sex, 1, 1, input);
		fwrite(&sex, 1, 1, output);
	}

	fclose(input);
	fclose(output);
	fclose(message);
}*/