#pragma once
//typedef char byte;
typedef unsigned long int uint32;

class Bitvector
{
	//el tamano es de multiplos de 32
	size_t m_size;
	uint32* m_array = 0; //enteros de 32 bits

public:

	uint32* getArray()
	{
		return m_array;
	}
	//constructor por copia
	Bitvector(const Bitvector& bv)
	{
		this->m_size = 0;
		this->m_size = bv.m_size;
		this->m_array = new uint32[this->m_size / 32];
		memcpy(this->m_array, bv.m_array, m_size / 8);
	}

	Bitvector(size_t newsize)
	{
		if (newsize == 0) newsize = 1;

		//hay que calcular el tamano como multiplo de 32		
		int realzise = newsize + 32 - (newsize % 32);
		m_size = realzise;
		//inicializar m_array al tamano requerido
		m_array = new uint32[m_size / 32];
		clearAll();
	}

	size_t getsize()
	{
		return m_size;
	}

	//notacion con operador corchetes
	bool operator[] (int idx) const
	{
		return getval(idx);
	}
	//getter
	bool getval(int index) const
	{
		int bitindex = index % 32;
		int bitbank = index / 32;
		return (m_array[bitbank] & (1 << bitindex)) >> bitindex;
	}

	//setter
	void setval(bool b, int index)
	{
		int bitindex = index % 32;
		int bitbank = index / 32;
		if (b)
		{
			m_array[bitbank] = m_array[bitbank] | (1 << bitindex);
		}
		else
		{
			m_array[bitbank] = m_array[bitbank] & ~(1 << bitindex);
		}
	}

	//establece un banco de bits completo
	void setbank(uint32 bits, int index)
	{
		if (index < m_size / 32)
		{
			m_array[index] = bits;
		}
	}

	//Limpia (poner a 0) TODO el arreglo
	void clearAll()
	{
		int bancos = m_size / 32;
		for (int i = 0; i < bancos; i++)
		{
			m_array[i] = 0;
		}
	}

	//imprimir en binario
	void binaryprint(const char* name)
	{
		std::cout << name << " ";
		binaryprint();
	}
	void binaryprint()
	{
		//para cada banko
		int bancos = m_size / 32;

		for (int i = 0; i < bancos; i++)
		{
			std::cout << "0b";
			for (int j = 31; j >= 0; j--)
			{
				uint32 b = (m_array[i] >> j) & 1;
				std::cout << (b ? "1" : "0");
			}
			std::cout << std::endl;
		}

		std::cout << "\n";
	}
};