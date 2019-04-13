// main.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c


#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <CommCtrl.h>
#include <ShlObj.h>
#include <ShlObj_core.h>
#include <stdio.h>

#include "GUIDefaults.hpp"
#include "IconResources.h"

#define BUFSIZE 4096

const static TCHAR szWindowClass[] = _T("YoutubeDL");
const static TCHAR szTitle[] = _T("Youtube-DL-GUI (SoonTM)");
const static TCHAR defaultFontName[] = _T("Courier New");
const static TCHAR fatalError[] = _T("Fatal Error");

HINSTANCE hInstMain, hInstEXEStatus;
LRESULT CALLBACK WndProcMain(HWND, UINT, WPARAM, LPARAM);
// int HookAndReceive(HINSTANCE, HINSTANCE, LPSTR, int);

// Default fonts. I wonder if it's okay to make these here...
static HFONT headerfont = \
CreateFont(headerTextHeight, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
	DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
	CLEARTYPE_QUALITY, VARIABLE_PITCH, defaultFontName);

static HFONT defaultFont = \
CreateFont(defaultTextHeight, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
	DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
	CLEARTYPE_QUALITY, VARIABLE_PITCH, defaultFontName);

static HFONT entryFont = \
CreateFont(defaultTextHeight - 2, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
	DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
	CLEARTYPE_QUALITY, VARIABLE_PITCH, defaultFontName);

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine, _In_ int       nCmdShow) {
	// Define and set parameters for the windows class attribute
	WNDCLASSEX wcex;
	MSG msg;
	int dummy;
	HWND hWnd;
	TCHAR registerFail[] = _T("Failed to register windows class attribute. Exitting.");
	TCHAR windowFail[] = _T("Failed to create window. Exitting.");
	TCHAR msgLoopFail[] = _T("Unable to get message. Is hWnd undefined?");

	hInstMain = hInstance;

	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstMain;
	wcex.lpfnWndProc = (WNDPROC)WndProcMain;
	wcex.hIcon = LoadIcon(hInstMain, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, registerFail, fatalError, MB_OK);
		return -1;
	}

	hWnd = CreateWindow(szWindowClass, szTitle, WS_NORESIZE, CW_USEDEFAULT, CW_USEDEFAULT,
		MainWindowWidth, MainWindowLength, NULL, NULL, hInstMain, NULL);
	if (!hWnd) {
		MessageBox(NULL, windowFail, fatalError, MB_OK);
		return -1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while ((dummy = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (dummy == -1) {
			MessageBox(NULL, msgLoopFail, fatalError, MB_OK);
			return -1;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	RECT headerRectangle;
	RECT URLLabelRectangle;
	RECT OutputLabelRectangle;

	HDC hdc;
	HWND pasteFromCBButton;
	HWND CloseButton;
	HWND URLEntry;
	HWND FormatListBox;
	HWND EmbedCheckBox;
	HWND outputEntry;
	HWND browseButton;
	HWND closeButton;
	HWND goButton;

	HBRUSH hBackground = NULL;
	HBITMAP hBit;
	HICON hIcon1, hIcon2;


	TCHAR header[] = _T("YouTube-DL-GUI");
	TCHAR URLprompt[] = _T("Video/Playlist URL:");
	TCHAR outputPrompt[] = _T("Output Location:");
	TCHAR URLEntryDefault[] = _T("https://www.youtube.com/");
	TCHAR CheckBoxText[] = _T("Embed Thumbnail as Album Artwork?");
	TCHAR outputDefault[MAX_PATH];
	TCHAR edit[] = _T("Edit");
	TCHAR button[] = _T("Button");
	TCHAR resourceFailure[] = _T("Unable to load asset resources.");
	TCHAR closeText[] = _T("Close");
	TCHAR goText[] = _T("Go");

	BROWSEINFO bInfo;
	PIDLIST_ABSOLUTE pidlist;
	TCHAR outputLocation[MAX_PATH]; // MAX_PATH = 260 usually.

	TCHAR Formats[15][32] = { _T("Select a Format"),
							 _T(".mp3 (audio)"), _T(".wav (audio)"), _T(".m4a (audio)"),
							 _T(".aac (audio)"), _T(".flac (audio)"), _T(".opus (audio)"),
							 _T(".vorbis (audio)"), _T(".mp4 (video)"),
							 _T(".avi (video)"), _T(".webm (video)"), _T(".mkv (video)"),
							 _T(".flv (video)"), _T(".ogg (video)") };
	int nFormats = 13 + 1; // Add 1 because of the "Select a Format" option.

	TCHAR F[32];

	switch (message) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// Make the Header
		SelectObject(hdc, headerfont);
		SetRect(&headerRectangle, headerXi, headerYi, headerXf, headerYf);
		DrawText(hdc, header, -1, &headerRectangle, DT_CENTER);

		// Make the URL label
		SelectObject(hdc, defaultFont);
		SetRect(&URLLabelRectangle, URLLabelXi, URLLabelYi, URLLabelXf, URLLabelYf);
		DrawText(hdc, URLprompt, -1, &URLLabelRectangle, DT_RIGHT);

		// Make the Output Label
		SetRect(&OutputLabelRectangle,
			OutputLabelXi, OutputLabelYi, OutputLabelXf, OutputLabelYf);
		DrawText(hdc, outputPrompt, -1, &OutputLabelRectangle, DT_RIGHT);

		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:
		hdc = GetDC(hWnd);
		// Make the URL Entry (Edit Window)
		URLEntry = CreateWindowEx(NULL, edit, URLEntryDefault, ENTRY_STYLE,
			URLEntryXi, URLEntryYi,
			URLEntryWidth, URLEntryHeight,
			hWnd, (HMENU)IDC_URL_ENTRY, hInstMain, NULL);
		SendMessage(URLEntry,
			WM_SETFONT, (WPARAM)entryFont, (LPARAM)MAKELONG(TRUE, 0));

		// Make the paste from clipboard button
		pasteFromCBButton = CreateWindow(button, NULL, BUTTON_STYLE | BS_ICON,
			pasteButtonXi, pasteButtonYi,
			pasteButtonWidth, pasteButtonHeight,
			hWnd, (HMENU)IDC_PASTE_INTO_URL,
			(HINSTANCE)GetWindowLong(hWnd, -6), NULL);
		hIcon1 = (HICON)LoadImage(hInstMain, MAKEINTRESOURCE(IDI_ICON1),
			IMAGE_ICON, 18, 18, LR_DEFAULTCOLOR);
		
		//hBit1 = LoadBitmap(hInstMain, MAKEINTRESOURCE(IDB_BITMAP3));
		if (!hIcon1) {
			// MessageBox(NULL, resourceFailure, fatalError, MB_OK);
			MessageBox(NULL, _T("AHHH"), fatalError, MB_OK);
			return -1;
		}
		SendMessage(pasteFromCBButton,
			BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon1);

		// Make the Format dropdown menu
		FormatListBox = CreateWindow(WC_COMBOBOX, NULL, DROPDOWN_STYLE,
			formatListXi, formatListYi,
			formatListWidth, formatListHeight,
			hWnd, (HMENU)IDC_FORMAT_ENTRY, hInstMain, NULL);
		memset(&F, 0, sizeof(F));
		for (int k = 0; k < nFormats; k += 1) {
			wcscpy_s(F, sizeof(F) / sizeof(TCHAR), (TCHAR*)Formats[k]);
			SendMessage(GetDlgItem(hWnd, IDC_FORMAT_ENTRY),
				CB_ADDSTRING, (WPARAM)0, (LPARAM)F);
		}
		SendMessage(FormatListBox,
			CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		SendMessage(FormatListBox,
			WM_SETFONT, (WPARAM)entryFont, (LPARAM)MAKELONG(TRUE, 0));

		// Make the Embed Thumbnail as Album Art Checkbox
		EmbedCheckBox = CreateWindow(button, CheckBoxText, CHECKBOX_STYLE,
			embedCheckXi, embedCheckYi,
			embedCheckWidth, embedCheckHeight,
			hWnd, (HMENU)IDC_EMBED_CHECKBOX, hInstMain, NULL);
		SendMessage(EmbedCheckBox,
			WM_SETFONT, (WPARAM)defaultFont, (LPARAM)MAKELONG(TRUE, 0));

		// Make the Output Entry
		SHGetFolderPath(NULL, CSIDL_MYMUSIC, NULL, SHGFP_TYPE_CURRENT, outputDefault);
		outputEntry = CreateWindowEx(NULL, edit, outputDefault, ENTRY_STYLE,
			outputEntryXi, outputEntryYi,
			outputEntryWidth, outputEntryHeight,
			hWnd, (HMENU)IDC_OUTPUT_ENTRY, hInstMain, NULL);
		SendMessage(outputEntry,
			WM_SETFONT, (WPARAM)entryFont, (LPARAM)MAKELONG(TRUE, 0));

		// Browse for path button
		browseButton = CreateWindow(button, NULL, BUTTON_STYLE | BS_ICON,
			browseButtonXi, browseButtonYi,
			browseButtonWidth, browseButtonHeight,
			hWnd, (HMENU)IDC_BROWSE_BUTTON,
			(HINSTANCE)GetWindowLong(hWnd, -6), NULL);
		hIcon2 = (HICON)LoadImage(hInstMain, MAKEINTRESOURCE(IDI_ICON1),
			IMAGE_ICON, 18, 18, LR_DEFAULTCOLOR);
		if (!hIcon2) {
			MessageBox(NULL, resourceFailure, fatalError, MB_OK);
			return -1;
		}
		SendMessage(browseButton, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon2);

		// Make the Close Button
		closeButton = CreateWindow(button, closeText, BUTTON_STYLE,
			closeButtonXi, closeButtonYi,
			closeButtonWidth, closeButtonHeight,
			hWnd, (HMENU)IDC_CLOSE_BUTTON,
			(HINSTANCE)GetWindowLong(hWnd, -6), NULL);
		SendMessage(closeButton,
			WM_SETFONT, (WPARAM)headerfont, (LPARAM)MAKELONG(TRUE, 0));

		// Make the Go Button
		goButton = CreateWindow(button, goText, BUTTON_STYLE,
			goButtonXi, goButtonYi, goButtonWidth, goButtonHeight,
			hWnd, (HMENU)IDC_GO_BUTTON,
			(HINSTANCE)GetWindowLong(hWnd, -6), NULL);
		SendMessage(goButton,
			WM_SETFONT, (WPARAM)headerfont, (LPARAM)MAKELONG(TRUE, 0));

		// Release hWnd.
		ReleaseDC(hWnd, hdc);
		break;

	case WM_COMMAND:
		switch LOWORD(wParam) {
		case IDC_URL_ENTRY:
			switch HIWORD(wParam) {
			case EN_UPDATE:
				break; // User modified text. Display not updated yet.
			case EN_CHANGE:
				break; // User modified text. Display updated already.
			case EN_ERRSPACE:
				break; // The edit control cannot allocate enough memory.
			case EN_HSCROLL:
				break; // User has clicked the edit control's h-scroll bar. 
			case EN_SETFOCUS: // User has selected edit control.
				PostMessage((HWND)lParam, EM_SETSEL, 0, -1);
				break;
			case EN_KILLFOCUS:
				break; // User has selected another control.
			case EN_MAXTEXT:
				break; // User has exceeded specified number of characters.
			case EN_VSCROLL:
				break; // User has clicked the edit control's v-scroll bar.
			default:
				break; // Otherwise
			}
			break;

		case IDC_PASTE_INTO_URL:
			SendMessage(GetDlgItem(hWnd, IDC_URL_ENTRY), EM_SETSEL, 0, -1);
			SendMessage(GetDlgItem(hWnd, IDC_URL_ENTRY), WM_CLEAR, 0, 0);
			SendMessage(GetDlgItem(hWnd, IDC_URL_ENTRY), WM_PASTE, 0, 0);
			SetFocus(GetDlgItem(hWnd, IDC_URL_ENTRY));
			SendMessage(GetDlgItem(hWnd, IDC_URL_ENTRY), EM_SETSEL, 0, -1);
			break;

		case IDC_BROWSE_BUTTON:
			bInfo.hwndOwner = hWnd;
			bInfo.pidlRoot = NULL;
			bInfo.pszDisplayName = NULL; // outputLocation;
			bInfo.lpszTitle = _T("UwU ...");
			bInfo.ulFlags = BIF_USENEWUI;
			bInfo.lpfn = NULL;
			bInfo.lParam = NULL;
			bInfo.iImage = NULL;

			pidlist = SHBrowseForFolder(&bInfo);
			if (SHGetPathFromIDList(pidlist, outputLocation) == 0) {
				return 0;
			}
			SendMessage(GetDlgItem(hWnd, IDC_OUTPUT_ENTRY), WM_SETTEXT,
				(WPARAM)0, (LPARAM)outputLocation);
			SetFocus(GetDlgItem(hWnd, IDC_OUTPUT_ENTRY));
			SendMessage(GetDlgItem(hWnd, IDC_OUTPUT_ENTRY), EM_SETSEL, 0, -1);
			CoTaskMemFree(pidlist);
			break;

		case IDC_CLOSE_BUTTON:
			PostQuitMessage(0);
			break;

		case IDC_GO_BUTTON:
			// MessageBox(NULL, goText, goText, MB_OK);
			EnableWindow(hWnd, FALSE);
			// HookAndReceive();
			EnableWindow(hWnd, TRUE);
			break;
		}
		break;


	case WM_CTLCOLORSTATIC:
		hdc = (HDC)wParam;
		SetTextColor(hdc, RGB(0, 0, 0));
		SetBkColor(hdc, RGB(255, 255, 255));
		if (hBackground == NULL) {
			hBackground = CreateSolidBrush(RGB(255, 255, 255));
		}
		return (INT_PTR)hBackground;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

LRESULT CALLBACK WndProcEXEstatus(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;
	HDC hdc;
	RECT sampleRect;


	TCHAR sampleText[] = _T("Sample Text");

	switch (message) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		SetRect(&sampleRect, 10, 10, 400, 60);
		DrawText(hdc, sampleText, -1, &sampleRect, DT_LEFT);

		EndPaint(hWnd, &ps);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
int HookAndReceive(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance,
				   _In_ LPSTR     lpCmdLine, _In_ int       nCmdShow) {
	// Work in Progress =)

	HANDLE childInputRead;
	HANDLE childInputWrite;
	HANDLE childOutputRead;
	HANDLE childOutputWrite;

	char buffer[4096];
	TCHAR buffer2[4096];
	TCHAR ProcessName[256];

	DWORD bytesRead, bytesWritten;
	DWORD exitCode;

	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	SECURITY_ATTRIBUTES securityAttributes;

	WNDCLASSEX wcex;
	// ###################################################################################
	// ###################################################################################
	// ###################################################################################
	// To pickup on this tomorrow, need to define WCEX and fix arguments for this function
	// To pickup on this tomorrow, need to define WCEX and fix arguments for this function
	// To pickup on this tomorrow, need to define WCEX and fix arguments for this function
	// To pickup on this tomorrow, need to define WCEX and fix arguments for this function
	// To pickup on this tomorrow, need to define WCEX and fix arguments for this function
	// To pickup on this tomorrow, need to define WCEX and fix arguments for this function
	// To pickup on this tomorrow, need to define WCEX and fix arguments for this function
	// ###################################################################################
	// ###################################################################################
	// ###################################################################################
	ZeroMemory(&ProcessName, sizeof(ProcessName));
	wcscpy_s(ProcessName, _T("child"));

	securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttributes.bInheritHandle = TRUE;
	securityAttributes.lpSecurityDescriptor = NULL;
	CreatePipe(&childOutputRead, &childOutputWrite, &securityAttributes, 0);
	CreatePipe(&childInputRead, &childInputWrite, &securityAttributes, 0);

	ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	startupInfo.cb = sizeof(STARTUPINFO);
	startupInfo.hStdInput = childInputRead;
	startupInfo.hStdOutput = childOutputWrite;
	startupInfo.hStdError = childOutputWrite;
	startupInfo.dwFlags |= STARTF_USESTDHANDLES;

	CreateProcess(NULL, ProcessName, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL,
				  &startupInfo, &processInfo);

	while (TRUE) {
		GetExitCodeProcess(processInfo.hProcess, &exitCode);
		switch (exitCode) {
			case STILL_ACTIVE:
				ReadFile(childOutputRead, buffer, sizeof(buffer), &bytesRead, NULL);
				buffer[bytesRead] = 0;
				swprintf(buffer2, sizeof(buffer), L"%hs", buffer);
				MessageBox(NULL, buffer2, _T("Message from child process"), MB_OK);
				break;
			case -1:
				MessageBox(NULL, _T("Failed"), fatalError, MB_OK);
			default:
				MessageBox(NULL, _T("Daddy process says you finished uwu"), _T("."), MB_OK);
				return 0;
				break;
		}
	}
	return 0;
}*/