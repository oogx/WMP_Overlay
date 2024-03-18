#include "Process.h"
int Process::Monitor_X = { 3840 };
int Process::Monitor_Y = { 2160 };
HWND Process::Hwnd;
Vec3 Process::Position;
Vec3 Process::Size;
bool Process::Setup(bool Enable) {
	if (Enable) {
		auto window = FindWindowA(("ms_sqlce_se_notify_wndproc"), nullptr);
		if (!window) {
			Hwnd = FindWindowA(("WMPlayerApp"), nullptr);
		}
	} else { return false; }
	RECT desktop{ };
	GetWindowRect(GetDesktopWindow(), &desktop);
	Process::Position = Vec3(0.f, 0.f, 0.f);
	Process::Size.x = desktop.right - desktop.left;
	Process::Size.y = desktop.bottom - desktop.top;
	if (!SetMenu(Hwnd, nullptr)) { return false; }
	if (!SetWindowLongPtrA(Hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE)) { return false; }
	if (!SetWindowLongPtrA(Hwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_TRANSPARENT)) { return false; }
	if (!SetWindowPos(Hwnd, HWND_TOPMOST, Process::Position.x, Process::Position.y, Process::Size.x, Process::Size.y, 0)) { return false; }
	if (!SetLayeredWindowAttributes(Hwnd, 0, 255, LWA_ALPHA)) { return false; }
	MARGINS margin = { 0, 0, desktop.bottom, desktop.right };
	if (DwmExtendFrameIntoClientArea(Hwnd, &margin) != S_OK) { return false; }
	return true;
}
HANDLE Process::Start(HANDLE Parent, const char* Path) {
	SIZE_T size;
	STARTUPINFOEX si = {};
	PROCESS_INFORMATION pi = {};
	InitializeProcThreadAttributeList(nullptr, 1, 0, &size);
	si.lpAttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(HeapAlloc(GetProcessHeap(), 0, size));
	InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &size);
	UpdateProcThreadAttribute(si.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &Parent, sizeof(Parent), nullptr, nullptr);
	si.StartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.StartupInfo.wShowWindow = SW_SHOW;
	si.StartupInfo.cb = sizeof(STARTUPINFO);
	CreateProcessA(Path, nullptr, nullptr, nullptr, false, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW | EXTENDED_STARTUPINFO_PRESENT, nullptr, nullptr, reinterpret_cast<STARTUPINFO*>(&si), &pi);
	HeapFree(GetProcessHeap(), 0, si.lpAttributeList);
	CloseHandle(pi.hThread);
	return pi.hProcess;
}