#include <Windows.h>
#include <stdint.h>

#define global_variable static
#define local_persist static
#define internal static

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef i32 bool32;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float r32;
typedef double r64;

struct win32_offscreen_buffer
{
	BITMAPINFO Info;
	void *Memory;
	i32 Width;
	i32 Height;
	i32 Pitch;
};

struct win32_window_dimension
{
	i32 Width;
	i32 Height;
};

global_variable bool32 GlobalRunning;
global_variable win32_offscreen_buffer GlobalBackBuffer;

internal win32_window_dimension
Win32GetWindowDimension(HWND Window)
{
	win32_window_dimension Dimension;
	
	RECT ClientRect;
	GetClientRect(Window, &ClientRect);
	Dimension.Width = ClientRect.right - ClientRect.left;
	Dimension.Height = ClientRect.bottom - ClientRect.top;

	return Dimension;
}

internal void
Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, i32 Width, i32 Height)
{
	if(Buffer->Memory)
	{
		VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
	}

	Buffer->Width = Width;
	Buffer->Height = Height;

	i8 BytesPerPixel = 4;

	// NOTE(rajiv): When the biHeight field is negative, this is the clue to
    // Windows to treat this bitmap as top-down, not bottom-up, meaning that
    // the first three bytes of the image are the color for the top left pixel
    // in the bitmap, not the bottom left!
    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;

    int BitmapMemorySize = (Buffer->Width*Buffer->Height)*BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    Buffer->Pitch = Width*BytesPerPixel;
}

internal void
Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer,
                           HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    // TODO(rajiv): Aspect ratio correction
    StretchDIBits(DeviceContext,
                  0, 0, WindowWidth, WindowHeight,
                  0, 0, Buffer->Width, Buffer->Height,
                  Buffer->Memory,
                  &Buffer->Info,
                  DIB_RGB_COLORS, SRCCOPY);
}

internal void
RenderGame(win32_offscreen_buffer *Buffer)
{
	local_persist i8 XOffset = 0;
	local_persist i8 YOffset = 0;
	u8 *Row = (u8 *)Buffer->Memory;
	for(i32 Y=0; Y < Buffer->Height; Y++)
	{
		u32 *Pixel = (u32 *)Row;
		for(i32 X=0; X < Buffer->Width; X++)
		{
			u8 Blue = X + XOffset;
			u8 Green = Y + YOffset;
			*Pixel++ = ((Green << 8) | Blue);
		}
		Row += Buffer->Pitch;
	}
	XOffset++;
	YOffset++;
}

internal LRESULT CALLBACK
Win32MainWindowCallback(HWND Window,
						UINT Message, 
						WPARAM WParam,
						LPARAM LParam)
{
	LRESULT Result = 0;

	switch(Message)
	{
		case WM_CLOSE:
		{
			// TODO(rajiv): Handle this message to the user
			GlobalRunning = false;

		} break;

		case WM_DESTROY:
		{
			// TODO(rajiv): Handle this as an error - recrete window
			GlobalRunning = false;		
		} break;

		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{

		} break;

		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			// TextOut(
			// 	DeviceContext,
			// 	0,
			// 	0,
			// 	"Game",
			// 	4);
			win32_window_dimension Dimension = Win32GetWindowDimension(Window);
			Win32DisplayBufferInWindow(&GlobalBackBuffer, DeviceContext, 
										Dimension.Width, Dimension.Height);
			EndPaint(Window, &Paint);
		} break;

		default:
		{
			Result = DefWindowProcA(Window, Message, WParam, LParam);
		} break;
	}

	return Result;
} 

int CALLBACK 
WinMain(HINSTANCE Instance,
		HINSTANCE PrevInstance,
		LPSTR     CmdLine,
		int       nCmdShow)
{
	WNDCLASSA WindowClass = {};

	Win32ResizeDIBSection(&GlobalBackBuffer, 1280, 720);

	WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	WindowClass.lpfnWndProc = Win32MainWindowCallback;
	WindowClass.hInstance = Instance;
//	WindowClass.hIcon;
	WindowClass.lpszClassName = "GameWindowClass";

	if(RegisterClassA(&WindowClass))
	{
		HWND Window = CreateWindowEx(
						0,
  						WindowClass.lpszClassName,
  						"Game",
  						WS_OVERLAPPEDWINDOW|WS_VISIBLE,
  						CW_USEDEFAULT,
						CW_USEDEFAULT,
        				CW_USEDEFAULT,
        				CW_USEDEFAULT,
  						0,
  						0,
  						Instance,
  						0);
		if(Window)
		{
			HDC DeviceContext = GetDC(Window);
			GlobalRunning = true;

			while(GlobalRunning)
			{
				MSG Message;
				while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
				{
					if(Message.message == WM_QUIT)
					{
						GlobalRunning =  false;
					}

					TranslateMessage(&Message);
					DispatchMessageA(&Message);
				}

				RenderGame(&GlobalBackBuffer);
				win32_window_dimension Dimension = Win32GetWindowDimension(Window);
				Win32DisplayBufferInWindow(&GlobalBackBuffer, DeviceContext, 
											Dimension.Width, Dimension.Height);
			}
		}
		else
		{
			// TODO(rajiv): Logging
		}

	}
	else
	{
		// TODO(rajiv): Logging
	}
}
