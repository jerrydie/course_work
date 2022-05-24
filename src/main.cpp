#include <iostream>
#include <iomanip>
#include <bitset>
#include "common/fea.h"
#include "common/FEA_Library/block_enc/block_local.h"

typedef uint8_t u8;
typedef uint64_t u64;
typedef struct _my_u128 {
	u64 high;
	u64 low;
} u128;

int main(int argc, char** argv) {
	int nb_rounds = 2;
	size_t block_size = 8;
	int key_bit_length = 128;
	size_t pt_bit_length = 8;
	int shift = 64 - pt_bit_length; // сдвиг добавила для совместимости с их функциями
	std::mt19937 gen(rd());
	//std::uniform_int_distribution<> distribution(1, 15); // для маски 0|alpha
	
	// структуры для задания конфигурации
	fpe_transform_ctx transform_ctx[1];
        fpe_encrypt_ctx encrypt_ctx[1];
	std::vector<u8> key(key_bit_length / 8, 0);
	
	//заполняем ключ произвольно
	fillRandom(key.data(), key.size(), gen);
	//указываем размер блока
	FEA_SetupLength(transform_ctx, FEA_DATA_BITS, block_size, nullptr);
	//генерируем раундовые ключи
	FEA_Keyschedule(encrypt_ctx, transform_ctx, FEA_ALG_ID_FEA_A, key.data(), key_bit_length);
	encrypt_ctx->e_ctx.rounds = nb_rounds;
	
	fpe_tweak tweak;
	// первая половина настройки фиксирована произвольной константой
	//tweak.tweak[0] = randomU64(gen);
	
	
	//int alpha = distribution(gen); //генерируем alpha
	for (int alpha = 1; alpha <=15; alpha++)
	{
		u8 mask1 = alpha; //первая маска, вида 0|alpha
		//std::cout << "Mask1: " << std::setw(9) << std::bitset<8>(mask1) << std::endl;
		u8 mask2 = alpha << 4; // вторая маска? вида alpha|0
		//std::cout << "Mask2: " << std::setw(9) << std::bitset<8>(mask2) << std::endl;
		
		double c1 = 0;
		double c2 = 0;
		
		//std::uniform_int_distribution<> fixed_distribution(1, 15);
		//u8 fixed_pt = fixed_distribution(gen);
		
		//"максимальный" открытый текст, для цикла
		u16 maximum_pt = std::pow(2, pt_bit_length) - 1;
		//Количество материала
		u16 qtty_pt = std::pow(2,8);
		for (u8 pt = 0; pt <= maximum_pt; pt += 1) // Перебираем все открытые тексты
		{

			//std::cout << "Plaintext:          " << std::setw(9) << std::bitset<8>(pt) << std::endl;
			//std::cout << "Plaintext & mask1:  " << std::setw(9) << std::bitset<8>(pt & mask1) << std::endl;
			u64 XX[2] = {pt, 0};
			//Вторая половина настройки, выбирается случайно
			tweak.tweak[0] = randomU64(gen);
			tweak.tweak[1] = randomU64(gen);
			
			// Шифруем с помощью встроенной функции
			FEA_A_Enc_U64ToU64(encrypt_ctx->e_ctx.rk, tweak.tweak, XX, encrypt_ctx->t_ctx.block_bits, encrypt_ctx->e_ctx.rounds);
			
			//Получаем шифртекст, приводим к нужному виду
			u8 ct = XX[1] >> shift;
			//std::cout << "Ciphertext:         " << std::setw(9) << std::bitset<8>(ct) << std::endl;
			//std::cout << "Ciphertext & mask1: " << std::setw(9) << std::bitset<8>(ct & mask1) << std::endl << std::endl;
			if ((ct & mask1) == (pt & mask1)) // проверяем выполнение соотношения для первой маски
			{
				c1+= 1;
				//std::cout << "C1: " << c1 <<std::endl;
				//std::cout << std::endl << std::endl << std::endl << "YEEEEEEEEES#1" << std::endl << std::endl << std::endl;
			}
			if ((ct & mask2) == (pt & mask2)) // проверяем выполнение соотношения для второй маски
			{
				c2+= 1;
				//std::cout << "C2: " << c2 <<std::endl;
				//std::cout << std::endl << std::endl << std::endl << "YEEEEEEEEES#1" << std::endl << std::endl << std::endl;
			}
			if (pt == maximum_pt) break;
		}
		c1 = std::pow((2*c1/qtty_pt - 1), 2);
		c2 = std::pow((2*c2/qtty_pt - 1), 2);
		
		// Вычисляем практические значения корреляции и выводим на экран для сравнения
		std::cout << "0|alpha experimental correlation: " << c1 << std::endl;
		std::cout << "alpha|0 experimental correlation: " << c2 << std::endl << std::endl;
	}
}

