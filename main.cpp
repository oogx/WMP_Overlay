#include "Process/Process.h"
using namespace cv;
int main() {
	char program_files[MAX_PATH];
	bool use_wmp = false;
	auto directory_exists = [](const std::string& dir_path) -> bool {
		auto attr = GetFileAttributesA(dir_path.c_str());
		if (attr == INVALID_FILE_ATTRIBUTES) { return false; }
		if (attr & FILE_ATTRIBUTE_DIRECTORY) { return true; }
		return false;
	};
	if (SHGetSpecialFolderPathA(0, program_files, CSIDL_PROGRAM_FILES, FALSE)) { if (directory_exists(std::string(program_files) + ("\\Windows Media Player\\"))) { use_wmp = true; }}
	auto explorer = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
	if (use_wmp) {
		auto path = std::string(program_files) + ("\\Windows Media Player\\wmplayer.exe");
		auto wmplayer = Process::Start(explorer, path.c_str());
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(2500));
	if (Process::Setup(use_wmp)) {
		/* Example of what to draw, i chose to draw a rectangle using opencv. */
		Mat frame = Mat::zeros(Process::Monitor_Y, Process::Monitor_X, CV_8UC3);
		HDC hdc = GetDC(Process::Hwnd);
		BITMAPINFO bmi = { sizeof(BITMAPINFOHEADER), Process::Monitor_X, -Process::Monitor_Y, 1, 24, BI_RGB, 0, 0, 0, 0, 0, };
		while (true) {
			rectangle(frame, Rect(1200, 600, 100, 200), Scalar(0, 0, 255), 1, 8, 0);
			StretchDIBits(hdc, 0, 0, Process::Monitor_X, Process::Monitor_Y, 0, 0, Process::Monitor_X, Process::Monitor_Y, frame.data, &bmi, DIB_RGB_COLORS, SRCCOPY);
		}
		ReleaseDC(Process::Hwnd, hdc);
	}
}