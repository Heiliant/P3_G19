#include "Renderer.hh"
#include <windows.h>

namespace enti
{
	const Renderer cout;
	const Endl endl{};
	const End cend{};

	static constexpr CharInfo EMPTY_CHAR{ L' ', static_cast<WORD>(Color::BLACK) };

	Renderer::Renderer() : hConsole{ GetStdHandle(STD_OUTPUT_HANDLE) }, curRow { 0 }, curCol{ 0 }, curColor{ Color::LIGHTGRAY }
	{
		::SendMessage(::GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000); // Set fullscreen
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(static_cast<HANDLE>(hConsole), &cursorInfo);
		cursorInfo.bVisible = false; // set the cursor visibility
		SetConsoleCursorInfo(static_cast<HANDLE>(hConsole), &cursorInfo);
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		GetConsoleScreenBufferInfo(static_cast<HANDLE>(hConsole), &csbiInfo);
		SetConsoleScreenBufferSize(static_cast<HANDLE>(hConsole), COORD{
			csbiInfo.srWindow.Right - csbiInfo.srWindow.Left + 1,
			csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top + 1
		}); // Disable vertical scrollbar
		maxRows = csbiInfo.srWindow.Bottom - 5;
		maxCols = csbiInfo.srWindow.Right - 5;
		data = new CharInfo[maxRows*maxCols];
	}

	Renderer::~Renderer() { delete[] data; }

	inline void fill(CharInfo *first, const CharInfo *last, const CharInfo &value) {  while (first != last) *first++ = value; }

	const R & operator<<(const R & r, const Endl & endl)
	{
		fill(r.data + r.curRow*r.maxCols + r.curCol, r.data + r.maxRows*r.maxCols, EMPTY_CHAR);
		++r.curRow; r.curCol = 0; return r;
	}

	const R & operator<<(const R & r, const End & end)
	{
		fill(r.data + r.curRow*r.maxCols + r.curCol, r.data + r.maxRows*r.maxCols, EMPTY_CHAR);
		static SMALL_RECT consoleWriteArea{ 0, 0, r.maxCols - 1, r.maxRows - 1 };
		static const COORD dataSize{ r.maxCols, r.maxRows };
		WriteConsoleOutput(static_cast<HANDLE>(r.hConsole), reinterpret_cast<CHAR_INFO*>(&r.data[0]), dataSize, COORD{}, &consoleWriteArea);
		r.curRow = r.curCol = 0;
		r.curColor = Color::LIGHTGRAY;
		return r;
	}

	#define SET_DATA(c) r.data[r.curRow*r.maxCols + r.curCol++] = { (c), static_cast<WORD>(r.curColor) }
	const R & operator<<(const R & r, const Color &col)		  { r.curColor = col; return r; }
	const R & operator<<(const R & r, Color && col)			  { r.curColor = std::move(col); return r; }
	const R & operator<<(const R & r, const char &c)		  { SET_DATA(c); return r; }
	const R & operator<<(const R & r, char && c)			  { SET_DATA(std::move(c)); return r; }
	const R & operator<<(const R & r, const std::string &str) { for (auto &c : str) SET_DATA(c); return r; }
	const R & operator<<(const R & r, std::string && str)	  { for (auto &c : std::move(str)) SET_DATA(c); return r; }
	const R & operator<<(const R & r, const char * str)		  { for (auto i = 0u; str[i] != '\0'; ++i) SET_DATA(str[i]); return r; }
	#undef SET
}