#pragma once
#include <string>

namespace enti
{
	enum class Color // List of colors that can be used to render text
	{
		BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY, DARKGRAY, LIGHTBLUE,
		LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
	};

	enum class Endl; // End line mark
	enum class End;  // Render buffer mark

	typedef unsigned short WORD;
	struct CharInfo // Info of each character rendered
	{
		union { char asciiChar; wchar_t unicodeChar; } data;
		WORD color;
	};

	class Renderer;
	using R = Renderer; // Alias type for simplification

	class Renderer
	{
	public:
		Renderer();
		~Renderer();
		friend const R & operator<<(const R & r, const Endl & endl);
		friend const R & operator<<(const R & r, const End & end);
		friend const R & operator<<(const R & r, const Color & col);
		friend const R & operator<<(const R & r, Color && col);
		friend const R & operator<<(const R & r, const char & c);
		friend const R & operator<<(const R & r, char && c);
		friend const R & operator<<(const R & r, const std::string & str);
		friend const R & operator<<(const R & r, std::string && str);
		friend const R & operator<<(const R & r, const char * str);
		template<class T> inline friend const R & operator<<(const R & r, const T & n);
	private:
		void * hConsole;		 // Windows OS console handler
		short maxCols;			 // Console width
		short maxRows;			 // Console height
		mutable short curRow;	 // Last line to be pushed to the buffer
		mutable short curCol;	 // Last column to be pushed to the buffer
		mutable Color curColor;	 // Last text color
		CharInfo * data;		 // Character output buffer
	};

	extern const Renderer cout; // Renderer object (like std::cout)
	extern const Endl endl;		// Endline object (like std::endl)
	extern const End cend;		// Ender object that tells when to write the buffer to the console

	template<class T> inline const R &
	operator<<(const R & r, const T & n) // Overloaded template for any integer or floating point number
	{
		static_assert(std::is_arithmetic<T>::value, "T must be a number, otherwise the operator must be overloaded");
		for (auto &c : std::to_string(n)) r.data[r.curRow*r.maxCols + r.curCol++] = { c, static_cast<WORD>(r.curColor) };
		return r;
	}
}