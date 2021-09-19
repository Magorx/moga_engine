#ifndef GENERAL_MATRIX
#define GENERAL_MATRIX

template <typename T>
class Matrix
{
public:
	size_t width;
	size_t height;
	T *data;

	Matrix(size_t width_, size_t height_):
	width(width_),
	height(height_),
	data((T*) calloc(width * height, sizeof(T)))
	{}

	T *operator[](int i) {
		return &(data[i * width]);
	}

	T &operator[](const Vec2d pos) {
		return data[(int) pos.y * width + (int) pos.x];
	}

	T &get(const Vec2d pos) {
		return data[(int) pos.y * width + (int) pos.x];
	}

	T &get(const double x, const double y) {
		return data[(int) y * width + (int) x];
	}

	bool is_valid_pos(const Vec2d pos) {
		return pos.x >= 0 && pos.y >= 0 && pos.x < width && pos.y < height;
	}

	bool is_valid_pos(const double x, const double y) {
		return x >= 0 && y >= 0 && x < width && y < height;
	}

	void destroy() {
		free(data);
	}
};

#endif // GENERAL_MATRIX