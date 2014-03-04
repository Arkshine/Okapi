
#ifndef __ASSEMBLY_CREATE_H__
#define __ASSEMBLY_CREATE_H__

class InstructionBytes
{
	protected:
	
		unsigned char* bytes;

	public:
		
		InstructionBytes(unsigned char* bytes) : bytes(bytes)
		{
			this->bytes = bytes;
		}

		virtual void set_bytes(unsigned char* bytes)
		{
			this->bytes = bytes;
		}

		virtual ~InstructionBytes(){};
};

template <class T>
class Instruction : public InstructionBytes
{
	public:

		virtual ~Instruction(){}

		Instruction(unsigned char* bytes) : InstructionBytes(bytes)
		{

		}

		static CVector<unsigned char> get_bytes()
		{
			CVector<unsigned char> bytes;

			for(size_t i=0;i<ARRAYSIZE(T::data);i++)
			{
				bytes.push_back(T::data[i]);
			}

			return bytes;
		}

		void set_long(long value)
		{
			assert(sizeof(T::data) >= 5);

			*((long*)(&this->bytes[1])) = value;
		}
};


#define INSTRUCTION(T) class T : public Instruction<T>{ public: static unsigned char data[]; T(unsigned char* bytes) : Instruction(bytes){  } 

INSTRUCTION(Inst_Mov_EDX_VAL) };
INSTRUCTION(Inst_Mov_EAX_VAL) };
INSTRUCTION(Inst_Mov_ECX_VAL) };

INSTRUCTION(Inst_Mov_ptrEDX_EAX) };
INSTRUCTION(Inst_Mov_ptrEDX_ECX) };
INSTRUCTION(Inst_Mov_ptrEDXpVAL_EAX) 

	void set_inc(unsigned char value)
	{
		*((unsigned char*)(&this->bytes[2])) = value;
	}
};
INSTRUCTION(Inst_Mov_ptrEDXpVAL_ECX) 

	void set_inc(unsigned char value)
	{
		*((unsigned char*)(&this->bytes[2])) = value;
	}
};
INSTRUCTION(Inst_Pop_EDX) };
INSTRUCTION(Inst_Push_ECX) };
INSTRUCTION(Inst_Push_ESP) };
INSTRUCTION(Inst_Push_VAL) };
INSTRUCTION(Inst_Push_EDX) };
INSTRUCTION(Inst_Enter) };
INSTRUCTION(Inst_Call) 

	void set_address(long address)
	{
		*((long*)(&this->bytes[1])) = (long)address - ((long)this->bytes + 5);
	}
};

INSTRUCTION(Inst_Leave) };
INSTRUCTION(Inst_Add_ESP_Val) 

	void set_inc(unsigned char value)
	{
		*((unsigned char*)(&this->bytes[2])) = value;
	}
};

INSTRUCTION(Inst_Mov_EDX_ptrESPpVAL) 

	void set_inc(unsigned char value)
	{
		*((unsigned char*)(&this->bytes[3])) = value;
	}
};
INSTRUCTION(Inst_Mov_ptrESP_EDX) };
INSTRUCTION(Inst_Ret) };
INSTRUCTION(Inst_RetN) 

	void set_count(unsigned char value)
	{
		*((unsigned char*)(&this->bytes[1])) = value;
	}
};

INSTRUCTION(Inst_Fst) 

	void set_ptr(long value)
	{
		*((long*)(&this->bytes[2])) = value;
	}
};

INSTRUCTION(Inst_Fstp) 

	void set_ptr(long value)
	{
		*((long*)(&this->bytes[2])) = value;
	}
};

INSTRUCTION(Inst_Fld) 

	void set_ptr(long value)
	{
		*((long*)(&this->bytes[2])) = value;
	}
};

unsigned char Inst_Mov_EDX_VAL::data[]			= {0xBA,0x0,0x0,0x0,0x0};
unsigned char Inst_Mov_EAX_VAL::data[]			= {0xB8,0x0,0x0,0x0,0x0};
unsigned char Inst_Mov_ECX_VAL::data[]			= {0xB9,0x0,0x0,0x0,0x0};
unsigned char Inst_Mov_ptrEDX_EAX::data[]	    = {0x89,0x2};
unsigned char Inst_Mov_ptrEDX_ECX::data[]		= {0x89,0xA};
unsigned char Inst_Mov_ptrEDXpVAL_EAX::data[]	= {0x89,0x42,0x0};
unsigned char Inst_Mov_ptrEDXpVAL_ECX::data[]	= {0x89,0x4A,0x0};
unsigned char Inst_Pop_EDX::data[]			    = {0x5A};
unsigned char Inst_Push_ECX::data[]				= {0x51};
unsigned char Inst_Push_ESP::data[]				= {0x54};
unsigned char Inst_Push_VAL::data[]				= {0x68,0x0,0x0,0x0,0x0};
unsigned char Inst_Push_EDX::data[]				= {0x52};
unsigned char Inst_Enter::data[]				= {0xC8,0x0,0x0,0x0};
unsigned char Inst_Call::data[]					= {0xE8,0x0,0x0,0x0,0x0};
unsigned char Inst_Leave::data[]				= {0xC9};
unsigned char Inst_Add_ESP_Val::data[]			= {0x83,0xC4,0xC};
unsigned char Inst_Mov_EDX_ptrESPpVAL::data[]	= {0x8B,0x54,0x24,0x0};
unsigned char Inst_Mov_ptrESP_EDX::data[]		= {0x89,0x14,0x24};
unsigned char Inst_Ret::data[]					= {0xC3};
unsigned char Inst_RetN::data[]					= {0xC2,0x0,0x0};
unsigned char Inst_Fst::data[]					= {0xD9,0x15,0x0,0x0,0x0,0x0};
unsigned char Inst_Fstp::data[]					= {0xD9,0x1D,0x0,0x0,0x0,0x0};
unsigned char Inst_Fld::data[]					= {0xD9,0x5,0x0,0x0,0x0,0x0};

class AssemblyCreate
{
	struct InstructionBytesOffset
	{
		InstructionBytes* instruction_bytes;
		size_t offset;

		InstructionBytesOffset() { }
		InstructionBytesOffset(InstructionBytes* instruction_bytes,size_t offset) : instruction_bytes(instruction_bytes) , offset(offset) { }
	};

	CVector<unsigned char> bytes;
	CVector<InstructionBytesOffset> instructionsBytesData;

	public:

		~AssemblyCreate()
		{
			for(size_t i=0;i<instructionsBytesData.size();i++)
			{
				InstructionBytesOffset instructionBytesData = instructionsBytesData[i];

				delete instructionBytesData.instruction_bytes;
			}
		}

		template <class T>
		T* add()
		{
			int pos = this->bytes.size();

			CVector<unsigned char> bytes = T::get_bytes();

			this->bytes.merge(bytes);

			T* object = new T(&this->bytes.m_Data[pos]);

			instructionsBytesData.push_back(InstructionBytesOffset(object,pos));

			return object;
		}

		unsigned char* create_block()
		{
			unsigned char* block = new unsigned char[bytes.size()];
			memcpy(block,this->bytes.m_Data,bytes.size());

			for(size_t i=0;i<instructionsBytesData.size();i++)
			{
				InstructionBytesOffset instructionBytesData = instructionsBytesData[i];
				instructionBytesData.instruction_bytes->set_bytes(&block[instructionBytesData.offset]);
			}

			return block;
		}

		unsigned char* get_block()
		{
			for(size_t i=0;i<instructionsBytesData.size();i++)
			{
				InstructionBytesOffset instructionBytesData = instructionsBytesData[i];
				instructionBytesData.instruction_bytes->set_bytes(&this->bytes.m_Data[instructionBytesData.offset]);
			}

			return this->bytes.m_Data;
		}

		size_t size()
		{
			return this->bytes.size();
		}
};

#endif
