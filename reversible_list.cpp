#include <cstdio>
#include <cctype>
#include <algorithm>
#include <iostream>

struct _0T_RevListNodeBase {
	_0T_RevListNodeBase *m_pn;
	_0T_RevListNodeBase(_0T_RevListNodeBase *pn = nullptr): m_pn(pn) {}
};

template <class T>
struct _0T_RevListNode: public _0T_RevListNodeBase {
	T m_val;
	_0T_RevListNode(_0T_RevListNodeBase *pn, const T& val): _0T_RevListNodeBase(pn), m_val(val) {}
};

template <class T>
inline T *ptrXor(T *lhs, T *rhs) {
	return (T *)((unsigned long)(lhs) ^ (unsigned long)(rhs));
}

template <class T> class list;
template <class T> class _0T_RevListConstIterator;

template <class T>
class _0T_RevListIterator {
	friend class list<T>;
	friend class _0T_RevListConstIterator<T>;
	_0T_RevListNodeBase *m_prev, *m_pos;
	_0T_RevListIterator(_0T_RevListNodeBase *pos, _0T_RevListNodeBase *prev): m_prev(prev), m_pos(pos) {}
public:
	typedef typename list<T>::size_type       size_type;
	typedef typename list<T>::difference_type difference_type;
	typedef T                                 value_type;
	typedef typename list<T>::pointer         pointer;
	typedef typename list<T>::reference       reference;
	typedef std::bidirectional_iterator_tag   iterator_category;

	T& operator * () const { return static_cast<_0T_RevListNode<T> *>(m_pos)->m_val; }
	T* operator ->() const { return &static_cast<_0T_RevListNode<T> *>(m_pos)->m_val; }

	_0T_RevListIterator& operator ++() {
		_0T_RevListNodeBase *next = ptrXor(m_pos->m_pn, m_prev);
		m_prev = m_pos;
		m_pos = next;
		return *this;
	}

	_0T_RevListIterator  operator ++(int) {
		_0T_RevListIterator result = *this;
		++*this;
		return result;
	}

	_0T_RevListIterator& operator --() {
		_0T_RevListNodeBase *prev = ptrXor(m_prev->m_pn, m_pos);
		m_pos = m_prev;
		m_prev = prev;
	}

	_0T_RevListIterator  operator --(int) {
		_0T_RevListIterator result = *this;
		--*this;
		return result;
	}

	bool operator == (const _0T_RevListIterator &other) const {
		return m_pos == other.m_pos;
	}

	bool operator != (const _0T_RevListIterator &other) const {
		return m_pos != other.m_pos;
	}
};

template <class T>
class _0T_RevListConstIterator {
	friend class list<T>;
	const _0T_RevListNodeBase *m_prev, *m_pos;
	_0T_RevListConstIterator(const _0T_RevListNodeBase *pos, const _0T_RevListNodeBase *prev): m_prev(prev), m_pos(pos) {}
public:
	typedef typename list<T>::size_type       size_type;
	typedef typename list<T>::difference_type difference_type;
	typedef const T                           value_type;
	typedef typename list<T>::const_pointer   pointer;
	typedef typename list<T>::const_reference reference;
	typedef std::bidirectional_iterator_tag   iterator_category;

	_0T_RevListConstIterator(const _0T_RevListIterator<T> &other): m_prev(other.m_prev), m_pos(other.m_pos) {}

	const T& operator * () const { return static_cast<const _0T_RevListNode<T> >(m_pos)->m_val; }
	const T* operator ->() const { return &static_cast<const _0T_RevListNode<T> >(m_pos)->m_val; }

	_0T_RevListConstIterator& operator ++() {
		const _0T_RevListNodeBase *next = ptrXor<const _0T_RevListNodeBase>(m_pos->m_pn, m_prev);
		m_prev = m_pos;
		m_pos = next;
		return *this;
	}

	_0T_RevListConstIterator  operator ++(int) {
		_0T_RevListConstIterator result = *this;
		++*this;
		return result;
	}

	_0T_RevListConstIterator& operator --() {
		const _0T_RevListNodeBase *prev = ptrXor<const _0T_RevListNodeBase>(m_prev->m_pn, m_pos);
		m_pos = m_prev;
		m_prev = prev;
	}

	_0T_RevListConstIterator  operator --(int) {
		_0T_RevListConstIterator result = *this;
		--*this;
		return result;
	}

	bool operator == (const _0T_RevListConstIterator &other) const {
		return m_pos == other.m_pos;
	}

	bool operator != (const _0T_RevListConstIterator &other) const {
		return m_pos != other.m_pos;
	}
};

using std::pair;
using std::make_pair;

template <class T>
class list {
	_0T_RevListNodeBase m_head, m_tail;
	size_t m_sz;
public:
	typedef size_t                      size_type;
	typedef ptrdiff_t                   difference_type;
	typedef T                           value_type;
	typedef T *                         pointer;
	typedef T &                         reference;
	typedef const T *                   const_pointer;
	typedef const T &                   const_reference;
	typedef _0T_RevListIterator<T>      iterator;
	typedef _0T_RevListConstIterator<T> const_iterator;

	list(): m_head(&m_tail), m_tail(&m_head), m_sz(0) {}

	list(const list &other): m_head(&m_tail), m_tail(&m_head), m_sz(0) {
		insert(begin(), other.begin(), other.end());
	}

	template <class InputIterator>
	list(InputIterator first, InputIterator last): m_head(&m_tail), m_tail(&m_head), m_sz(0) {
		insert(begin(), first, last);
	}

	~list() {
		clear();
	}

	iterator       begin()        { return iterator(m_head.m_pn, &m_head); }
	const_iterator begin()  const { return const_iterator(m_head.m_pn, &m_head); }
	const_iterator cbegin() const { return const_iterator(m_head.m_pn, &m_head); }
	iterator       end()          { return iterator(&m_tail, m_tail.m_pn); }
	const_iterator end()    const { return const_iterator(&m_tail, m_tail.m_pn); }
	const_iterator cend()   const { return const_iterator(&m_tail, m_tail.m_pn); }

	/** NOTE that inserting into a reversible list will cause the current iterator to invalidate! */
	iterator insert(const_iterator _pos, const T& val) {
		iterator pos(const_cast<_0T_RevListNodeBase *>(_pos.m_pos), const_cast<_0T_RevListNodeBase *>(_pos.m_prev));
		_0T_RevListNodeBase *new_node = new _0T_RevListNode<T>(ptrXor(pos.m_pos, pos.m_prev), val);
		pos.m_prev->m_pn = ptrXor(pos.m_prev->m_pn, ptrXor(pos.m_pos, new_node));
		pos.m_pos->m_pn  = ptrXor(pos.m_pos->m_pn, ptrXor(pos.m_prev, new_node));
		pos.m_pos = new_node;
		return pos;
	}


	/** NOTE that inserting into a reversible list will cause the current iterator to invalidate! */
	template <class InputIterator>
	iterator insert (const_iterator pos, InputIterator first, InputIterator last) {
		if (first == last) {
			return iterator(const_cast<_0T_RevListNodeBase *>(pos.m_pos), const_cast<_0T_RevListNodeBase *>(pos.m_prev));
		}
		iterator result = insert(pos, *first);
		++first;
		pos = result;
		++pos;
		while (first != last) {
			pos = ++insert(pos, *first);
			++first;
		}
		return result;
	}


	/** NOTE that erasing from a reversible list will cause the current iterator AND ITS SUCCESSOR to invalidate! */
	iterator erase  (const_iterator _pos) {
		iterator pos(const_cast<_0T_RevListNodeBase *>(_pos.m_pos), const_cast<_0T_RevListNodeBase *>(_pos.m_prev));
		_0T_RevListNodeBase *next = ptrXor(pos.m_pos->m_pn, pos.m_prev);
		next->m_pn = ptrXor(next->m_pn, ptrXor(pos.m_pos, pos.m_prev));
		pos.m_prev->m_pn = ptrXor(pos.m_prev->m_pn, ptrXor(next, pos.m_pos));
		delete static_cast<_0T_RevListNode<T> *>(pos.m_pos);
		pos.m_pos = next;
		return pos;
	}

	/** NOTE that erasing from a reversible list will cause the current iterator AND ITS SUCCESSOR to invalidate! */
	iterator erase  (const_iterator first, const_iterator last) {
		iterator pos(const_cast<_0T_RevListNodeBase *>(first.m_pos), const_cast<_0T_RevListNodeBase *>(first.m_prev));
		while (pos.m_pos != last.m_pos) pos = erase(pos);
		return pos;
	}

	/** NOTE that reversing a range [first, last) will cause the first ans last iterator to invalidate! */
	pair<iterator, iterator> reverse(const_iterator first, const_iterator last) {
		_0T_RevListNodeBase *first_prev = const_cast<_0T_RevListNodeBase *>(first.m_prev);
		_0T_RevListNodeBase *first_pos  = const_cast<_0T_RevListNodeBase *>(first.m_pos);
		_0T_RevListNodeBase *last_prev  = const_cast<_0T_RevListNodeBase *>(last.m_prev);
		_0T_RevListNodeBase *last_pos   = const_cast<_0T_RevListNodeBase *>(last.m_pos);
		first_prev->m_pn = ptrXor(first_prev->m_pn, ptrXor(first_pos, last_prev));
		first_pos->m_pn = ptrXor(first_pos->m_pn, ptrXor(first_prev, last_pos));
		last_pos->m_pn = ptrXor(last_pos->m_pn, ptrXor(last_prev, first_pos));
		last_prev->m_pn = ptrXor(last_prev->m_pn, ptrXor(first_prev, last_pos));
		return make_pair(iterator(first_prev, last_prev), iterator(first_pos, last_pos));
	}

	bool      empty() const { return !m_sz; }
	size_type size()  const { return m_sz; }

	void clear() { erase(begin(), end()); }

	void push_back(const T& val)  { insert(end(), val); }
	void push_front(const T& val) { insert(begin(), val); }
	void pop_back()               { erase(end()); }
	void pop_front()              { erase(begin()); }
};

int init[] = { 7, 5, 16, 8 };
int main()
{
    // Create a list containing integers
    list<int> l(init + 0, init + sizeof(init) / sizeof(init[0]));

    // Add an integer to the front of the list
    l.push_front(25);
    // Add an integer to the back of the list
    l.push_back(13);

    // Insert an integer before 16 by searching
    auto it = std::find(l.begin(), l.end(), 16);
    if (it != l.end()) {
        l.insert(it, 42);
    }

    // Iterate and print values of the list
    for (int n : l) {
        std::cout << n << '\n';
    }

    l.reverse(l.begin(), l.end());

    for (int n : l) {
        std::cout << n << '\n';
    }

}
