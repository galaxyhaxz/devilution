//HEADER_GOES_HERE
#ifndef __FAULT_H__
#define __FAULT_H__

typedef struct STACK_FRAME {
	struct STACK_FRAME *pNext;
	void *pCallRet;
} STACK_FRAME;

//int dword_52B9F4;
extern LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter; // idb

// void exception_cpp_init();
void exception_install_filter();
void j_exception_init_filter();
void __cdecl exception_init_filter();
LONG __stdcall TopLevelExceptionFilter(PEXCEPTION_POINTERS ExceptionInfo);
void exception_hex_format(BYTE *ptr, unsigned int numBytes);
void exception_unknown_module(LPCVOID lpAddress, LPSTR lpModuleName, int iMaxLength, int *sectionNum, int *sectionOffset);
void exception_call_stack(void *instr, STACK_FRAME *stackAddr);
char *exception_get_error_type(DWORD dwMessageId, LPSTR lpString1, DWORD nSize);
void exception_set_filter();
LPTOP_LEVEL_EXCEPTION_FILTER exception_set_filter_ptr();
LPTOP_LEVEL_EXCEPTION_FILTER exception_get_filter();

#endif /* __FAULT_H__ */
