#pragma once

#include <cstring>
#include <cstdint>

class Vec2
{
public:
	// Constructors
	Vec2()
	{
		data[0] = data[1] = 0;
	}

	Vec2(int n)
	{
		data[0] = data[1] = n;
	}

	Vec2(int x, int y)
	{
		data[0] = x;
		data[1] = y;
	}

	// Overload operator []
	inline int& operator[](int i)
	{
		return data[i];
	}

	inline const int& operator[](int i) const
	{
		return data[i];
	}

	friend inline bool operator==(const Vec2& lhs, const Vec2& rhs) 
	{
		return (lhs.data[0] == rhs.data[0]) && (lhs.data[1] == rhs.data[1]);
	}

	friend inline bool operator!=(const Vec2& lhs, const Vec2& rhs)
	{
		return (lhs.data[0] != rhs.data[0]) || (lhs.data[1] != rhs.data[1]);
	}

	inline void fill(const int n)
	{
		data[0] = data[1] = n;
	}

	// Members
	int data[2];
};

class Vec9
{
public:
	// Constructors
	Vec9()
	{
		fill(0);
	}

	Vec9(const int n)
	{
		fill(n);
	}

	Vec9(const int a, const int b, const int c, const int d, const int e,
		const int f, const int g, const int h, const int i)
	{
		data[0] = a;
		data[1] = b;
		data[2] = c;
		data[3] = d;
		data[4] = e;
		data[5] = f;
		data[6] = g;
		data[7] = h;
		data[8] = i;
	}

	// Overload operator []
	inline int& operator[](int i)
	{
		return data[i];
	}

	inline const int& operator[](int i) const
	{
		return data[i];
	}

	inline void fill(const int n)
	{
		data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = data[6] = data[7] = data[8] = n;
		
		// Memset copia solo il primo byte di n, quindi e' un bug se sizeof(data[0]) > 1
		// Se utilizzo std::int8_t data[9], allora memset e' perfetto
		//std::memset(this, &n, 9);
	}

	// Check if there is a tris
	inline int tris() const
	{
		int i = ((data[0] == 1) << 2) | ((data[1] == 1) << 1) | ((data[2] == 1) << 0);
		int j = ((data[3] == 1) << 5) | ((data[4] == 1) << 4) | ((data[5] == 1) << 3) |
				((data[6] == 1) << 2) | ((data[7] == 1) << 1) | ((data[8] == 1) << 0);
		if ((Vec9::lut[i] >> j) & 1)
			return 1;

		i = ((data[0] == 2) << 2) | ((data[1] == 2) << 1) | ((data[2] == 2) << 0);
		j = ((data[3] == 2) << 5) | ((data[4] == 2) << 4) | ((data[5] == 2) << 3) |
			((data[6] == 2) << 2) | ((data[7] == 2) << 1) | ((data[8] == 2) << 0);
		if ((Vec9::lut[i] >> j) & 1)
			return 2;

		// Return 0 if there is still space (not decided yet)
		if (!(data[0] && data[1] && data[2] && data[3] && data[4] && data[5] && data[6] && data[7] && data[8]))
			return 0;

		// Return 3 if the board if full and no tris found (draw)
		return 3;
	}

	inline bool forcedDraw() const
	{
		int i = ((data[0] < 2) << 2) | ((data[1] < 2) << 1) | ((data[2] < 2) << 0);
		int j = ((data[3] < 2) << 5) | ((data[4] < 2) << 4) | ((data[5] < 2) << 3) |
				((data[6] < 2) << 2) | ((data[7] < 2) << 1) | ((data[8] < 2) << 0);
		if ((Vec9::lut[i] >> j) & 1)
			return false;

		i = ((!(data[0] & 1)) << 2) | ((!(data[1] & 1)) << 1) | ((!(data[2] & 1)) << 0);
		j = ((!(data[3] & 1)) << 5) | ((!(data[4] & 1)) << 4) | ((!(data[5] & 1)) << 3) |
			((!(data[6] & 1)) << 2) | ((!(data[7] & 1)) << 1) | ((!(data[8] & 1)) << 0);
		return !((Vec9::lut[i] >> j) & 1);
	}

	// Members
	int data[9];

private:
	static constexpr std::uint64_t lut[8] = {
		0xff80808080808080,
		0xfff0aa80faf0aa80,
		0xffcc8080cccc8080,
		0xfffcaa80fefcaa80,
		0xfffaf0f0aaaa8080,
		0xfffafaf0fafaaa80,
		0xfffef0f0eeee8080,
		0xffffffffffffffff };
};
