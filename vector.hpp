#ifndef VECTOR_HPP
#define VECTOR_HPP
namespace sjtu 
{
	template<class T>
	class vector 
	{
	private:
		T * data;
		size_t currentLength;
		size_t maxSize;
		void doubleSpace()
		{
			T *tmp = data;
			maxSize *= 2;
			data = new T[maxSize];
			for (size_t i = 0; i < currentLength; ++i)
				data[i] = tmp[i];
			delete[] tmp;
		}

	public:
		class const_iterator;
		class iterator {
			friend class vector;
		private:
			vector *ptr = nullptr;
			int index;

		public:
			iterator() = default;
			iterator(vector *_ptr, int _index)
			{
				ptr = _ptr;
				index = _index;
			}
			iterator(const iterator &other)
			{
				ptr = other.ptr;
				index = other.index;
			}
			iterator operator+(const int &n) const
			{
				return iterator(ptr, index + n);
			}
			iterator operator-(const int &n) const
			{
				return iterator(ptr, index - n);
			}
			int operator-(const iterator &rhs) const
			{
				return index - rhs.index;
			}
			iterator operator+=(const int &n)
			{
				index += n;
				return *this;
			}
			iterator operator-=(const int &n)
			{
				//TODO
				index -= n;
				return *this;
			}
			iterator operator++(int)
			{
				iterator tmp = *this;
				++index;
				return tmp;
			}
			iterator& operator++()
			{
				++index;
				return *this;
			}
			iterator operator--(int)
			{
				iterator tmp = *this;
				--index;
				return tmp;
			}
			iterator& operator--()
			{
				--index;
				return *this;
			}
			T& operator*() const
			{
				return (*ptr)[index];
			}
			bool operator==(const iterator &rhs) const
			{
				return ptr == rhs.ptr && index == rhs.index;
			}
			bool operator==(const const_iterator &rhs) const
			{
				return ptr == rhs.ptr && index == rhs.index;
			}
			bool operator!=(const iterator &rhs) const
			{
				return !(ptr == rhs.ptr && index == rhs.index);
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return !(ptr == rhs.ptr && index == rhs.index);
			}
		};
		class const_iterator
		{
		private:
			vector * ptr = nullptr;
			int index;

		public:
			const_iterator() = default;
			const_iterator(const vector *_ptr, int _index)
			{
				ptr = (vector*)_ptr;
				index = _index;
			}
			const_iterator(const const_iterator &other)
			{
				ptr = other.ptr;
				index = other.index;
			}
			const_iterator operator+(const int &n) const
			{
				return const_iterator(ptr, index + n);
			}
			const_iterator operator-(const int &n) const
			{
				return const_iterator(ptr, index - n);
			}
			int operator-(const const_iterator &rhs) const
			{
				return index - rhs.index;
			}
			const_iterator operator+=(const int &n)
			{
				index += n;
				return *this;
			}
			const_iterator operator-=(const int &n)
			{
				index -= n;
				return *this;
			}
			const_iterator operator++(int)
			{
				const_iterator tmp = *this;
				++index;
				return tmp;
			}
			const_iterator& operator++()
			{
				++index;
				return *this;
			}
			const_iterator operator--(int)
			{
				const_iterator tmp = *this;
				--index;
				return tmp;
			}
			const_iterator& operator--()
			{
				--index;
				return *this;
			}
			T operator*() const
			{
				return (*ptr)[index];
			}
			bool operator==(const iterator &rhs) const
			{
				return ptr == rhs.ptr && index == rhs.index;
			}
			bool operator==(const const_iterator &rhs) const
			{
				return ptr == rhs.ptr && index == rhs.index;
			}
			bool operator!=(const iterator &rhs) const
			{
				return !(ptr == rhs.ptr && index == rhs.index);
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return !(ptr == rhs.ptr && index == rhs.index);
			}
		};
		vector(int initSize = 1000)
		{
			data = new T[initSize];
			maxSize = initSize;
			currentLength = 0;
		}
		vector(const vector &other)
		{
			if (data)
				clear();
			data = new T[other.size()];
			maxSize = currentLength = other.size();
			for (size_t i = 0; i < other.size(); ++i)
				data [i] = other.data[i];
		}
		~vector()
		{
			clear();
		}
		vector &operator=(const vector &other)
		{
			
			if (this == &other)
				return *tihs;
			if (data)
				clear();
			data = new T[other.size()];
			maxSize = currentLength = other.size();
			for (size_t i = 0; i < other.size(); ++i)
				data [i] = other.data[i];
			return *this;
		}
		T & at(const size_t &pos)
		{
			return data[pos];
		}
		const T & at(const size_t &pos) const
		{
			return data[pos];
		}
		T & operator[](const size_t &pos)
		{
			return data[pos];
		}
		const T & operator[](const size_t &pos) const
		{
			return data[pos];
		}
		const T & front() const
		{
			return data[0];
		}
		const T & back() const
		{
			return data[currentLength - 1];
		}
		iterator begin()
		{
			return iterator(this, 0);
		}
		const_iterator cbegin() const
		{
			return const_iterator(this, 0);
		}
		iterator end()
		{
			return iterator(this, currentLength);
		}
		const_iterator cend() const
		{
			return const_iterator(this, currentLength);
		}
		bool empty() const
		{
			return currentLength == 0;
		}
		size_t size() const
		{
			return currentLength;
		}
		size_t capacity() const
		{
			return maxSize;
		}
		void clear()
		{
			delete[]data;
			maxSize = currentLength = 0;
		}
		iterator insert(iterator pos, const T &value)
		{
			if (currentLength == maxSize)
				doubleSpace();
			for (size_t i = currentLength; i > pos.index; --i)
				data[i] = data[i - 1];
			data[pos.index] = value;
			++currentLength;
			return pos;
		}
		iterator insert(const size_t &ind, const T &value)
		{
			if (currentLength == maxSize)
				doubleSpace();
			for (size_t i = currentLength; i > ind; --i)
				data[i] = data[i - 1];
			data[ind] = value;
			++currentLength;
			return iterator(this, ind);
		}
		iterator erase(iterator pos)
		{
			for (size_t i = pos.index; i < currentLength; ++i)
				data[i] = data[i + 1];
			--currentLength;
			return pos;
		}
		iterator erase(const size_t &ind)
		{
			for (size_t i = ind; i < currentLength; ++i)
				data[i] = data[i + 1];
			--currentLength;
			return iterator(this, ind);
		}
		void push_back(const T &value)
		{
			if (currentLength == maxSize)
				doubleSpace();
			data[currentLength] = value;
			++currentLength;
		}
		void pop_back()
		{
			--currentLength;
		}
	};


}

#endif
