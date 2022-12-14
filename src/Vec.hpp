#pragma once

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
	constexpr int& operator[](int i)
	{
		return data[i];
	}

	constexpr const int& operator[](int i) const
	{
		return data[i];
	}

	constexpr void fill(const int n)
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
		data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = data[6] = data[7] = data[8] = 0;
	}

	Vec9(const int n)
	{
		data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = data[6] = data[7] = data[8] = n;
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
	constexpr int& operator[](int i)
	{
		return data[i];
	}

	constexpr const int& operator[](int i) const
	{
		return data[i];
	}

	constexpr void fill(const int n)
	{
		data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = data[6] = data[7] = data[8] = n;
	}

	// Check if there is a tris
	constexpr int tris() const
	{
		// Return 1 or 2 if there is a tris on board (win)
		if ((data[4] == 1 || data[4] == 2) &&
			((data[4] == data[3] && data[4] == data[5]) ||
			 (data[4] == data[1] && data[4] == data[7]) ||
			 (data[4] == data[0] && data[4] == data[8]) ||
			 (data[4] == data[2] && data[4] == data[6])))
			return data[4];

		if ((data[0] == 1 || data[0] == 2) &&
			((data[0] == data[1] && data[0] == data[2]) ||
			 (data[0] == data[3] && data[0] == data[6])))
			return data[0];

		if ((data[8] == 1 || data[8] == 2) &&
			((data[8] == data[6] && data[8] == data[7]) ||
			 (data[8] == data[2] && data[8] == data[5])))
			return data[8];

		// Return 0 if there is still space (not decided yet)
		if (!(data[0] && data[1] && data[2] && data[3] && data[4] && data[5] && data[6] && data[7] && data[8]))
			return 0;

		// Return 3 if the board if full and no tris found (draw)
		return 3;
	}

	// Check if a not ended game can only end in a draw or can still be won
	constexpr bool forcedDraw() const
	{
		if ((data[0] < 2 && data[1] < 2 && data[2] < 2) ||
			(data[0] % 2 == 0 && data[1] % 2 == 0 && data[2] % 2 == 0) ||
			(data[3] < 2 && data[4] < 2 && data[5] < 2) ||
			(data[3] % 2 == 0 && data[4] % 2 == 0 && data[5] % 2 == 0) ||
			(data[6] < 2 && data[7] < 2 && data[8] < 2) ||
			(data[6] % 2 == 0 && data[7] % 2 == 0 && data[8] % 2 == 0) ||
			(data[0] < 2 && data[3] < 2 && data[6] < 2) ||
			(data[0] % 2 == 0 && data[3] % 2 == 0 && data[6] % 2 == 0) ||
			(data[1] < 2 && data[4] < 2 && data[7] < 2) ||
			(data[1] % 2 == 0 && data[4] % 2 == 0 && data[7] % 2 == 0) ||
			(data[2] < 2 && data[5] < 2 && data[8] < 2) ||
			(data[2] % 2 == 0 && data[5] % 2 == 0 && data[8] % 2 == 0) ||
			(data[0] < 2 && data[4] < 2 && data[8] < 2) ||
			(data[0] % 2 == 0 && data[4] % 2 == 0 && data[8] % 2 == 0) ||
			(data[2] < 2 && data[4] < 2 && data[6] < 2) ||
			(data[2] % 2 == 0 && data[4] % 2 == 0 && data[6] % 2 == 0))
			return false;

		return true;
	}

	// Members
	int data[9];
};