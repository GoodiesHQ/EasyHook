#include "EasyHook.hpp"

HOOKINIT(
	MessageBoxA_F,								// the type created 
	MessageBoxA,								// the function prototyped
	MessageBoxA_Tramp,							// the trampoline to the original function
	MessageBoxA_Prologue						// the prologue object of the function used for this hook
)

EasyHook::Hook32 hooker;						// an object meant to service you


/* This is identical to the original function's definition.*/
int WINAPI MessageBoxA_H(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
	lpText = "Hook is working!";
	if(MessageBoxA_Tramp)						// call the original function with the altered parameters if it exists.
		return MessageBoxA_Tramp(hWnd, lpText, lpCaption, uType);
	return 0;
}

void makeMessage() {
	MessageBoxA(NULL, TEXT("Testing my hook."), TEXT("Test"), MB_OK);
}


int main() {
	HMODULE user32 = GetModuleHandleA("User32.dll");
	if (!user32)
		return std::cerr << "Could not get a handle to the DLL.", EXIT_FAILURE;
	FARPROC func = GetProcAddress(user32, "MessageBoxA");
	if (!func)
		return std::cerr << "Could not find the function within the DLL.", EXIT_FAILURE;

	makeMessage();								// first execution (before hooking)

	MessageBoxA_Tramp = (MessageBoxA_F)hooker.hook(
		func,									// pointer to the function you'd like to hook
		MessageBoxA_Prologue,					// the prologue created by the INIT macro
		MessageBoxA_H							// the hook function to which you want to redirect the original
	);

	makeMessage();								// second execution (after hooking)

	hooker.unhook(MessageBoxA_Tramp, MessageBoxA_Prologue);

	makeMessage();								// third execution (after unhooking)
	return 0;
}