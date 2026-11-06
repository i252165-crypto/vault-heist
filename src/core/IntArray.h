class IntArray
{
  private:
    int *m_data;
    int m_size;
    int m_cap;

  public:
    IntArray();
    ~IntArray();
    IntArray(const IntArray &other);
    IntArray &operator=(const IntArray &other);

    // Core API
    void push(int v);
    void pop();
    int get(int i) const;

    // Operator overloading for array-like access
    int &operator[](int i);
    const int &operator[](int i) const;

    // Utilities
    int size() const;
    bool isEmpty() const;
    void clear();
    void reserve(int n);
    int indexOf(int v) const;
    void swap(int i, int j);
};
