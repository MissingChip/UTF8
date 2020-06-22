#include <cstring>
#include <string>

typedef unsigned int unicode_t;

int reverse_int (int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 0xff;
    ch2 = (i >> 8) & 0xff;
    ch3 = (i >> 16) & 0xff;
    ch4 = (i >> 24) & 0xff;
    return((int) ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}

int utf8size(const char* s);
unicode_t utf8_uni(const char* s);
class UTF8Char;
class UTF8Ref;
class UTF8Iter;
unicode_t utf8_uni(const UTF8Char& s);
UTF8Char uni_utf8(unicode_t s);

/**
 * A string for working with utf-8
 *
 * All indexing is done with ints, as they're small
 * and if you're indexing over the range of a sighned integer 
 * you probably want to be using this
 * with something else at the very least
 */
class UTF8 {
	/* TODO
	 * find
	 * substr
	 * compare
	 * resize
	 * replace
	 *
	 * ???
	 * operator==
	 * shrink_to_fit
	 * reserve
	 */

	public:
		/**
		 * Create a UTF-8 encoded string based off of a c++ string
		 *
		 * (simply copy the string)
		 */
		UTF8(std::string s);
		/**
		 * Create a UTF-8 encoded string based off of a c string
		 *
		 * (simply copy the string)
		 */
		UTF8(const char* s);
		/**
		 * The size of the underlying string, in bytes
		 */
		int size() const;
		/**
		 * the number of charactrers in the utf-8 string
		 */
		int length();
		int length() const;
		/**
		 * An iterator starting at the beginning of the string
		 */
		UTF8Iter begin();
		/**
		 * An iterator starting at the end of the string
		 */
		UTF8Iter end();
		/**
		 * returns whether or not the string is empty (contains no characters)
		 */
		bool empty() { return raw_string.empty(); }
		/**
		 * returns whether or not the string is equivalent to another
		 * TODO substrings (char*, etc)
		 */
		int compare(const UTF8& b) { return raw_string.compare(b.raw_string); }
		/**
		 * Get the character at the given index
		 *
		 * not the same as the character at the given index of the internal string,
		 * as utf-8 is variable with
		 */
		UTF8Ref operator[](int idx);
		/**
		 * Set the contents of this string to the contents of another
		 * (using the std::string assignment)
		 */
		UTF8& operator=(const UTF8& b) { raw_string = b.raw_string; n_chars = b.n_chars; return *this; }
		/**
		 * Append the contents of another string to this string
		 */
		UTF8& operator+=(const UTF8& b) { raw_string += b.raw_string; n_chars += b.length(); return *this; }
		/**
		 * Append the contents of another string to this string
		 */
		UTF8& append(const UTF8& b) { raw_string.append(b.raw_string); n_chars += b.n_chars; return *this; }
		/**
		 * 	Push a UTF8Char to the back of this string
		 */
		UTF8& push_back(const UTF8Char& c);
		/**
		 * 	Push a UTF8Char to the back of this string
		 */
		UTF8& push_front(const UTF8Char& c);
		/**
		 * Insert a UTF8 char, string, standard string, or c string into the string
		 * beginning at index pos
		 */
		UTF8& insert(int pos, const UTF8Char& c);
		UTF8& insert(int pos, UTF8& c);
		UTF8& insert(int pos, const UTF8& c);
		UTF8& insert(int pos, const std::string& c);
		UTF8& insert(int pos, const char* c);

		/**
		 * Erases the portion of the string value that begins at the character position pos and spans len characters
		 */
		UTF8& erase(int pos, int len);
		/**
		 * Replaces the portion of the string value that begins at the character position pos and spans len characters
		 * with the content in s
		 */
		UTF8& replace(int pos, int len, const UTF8& str); 
		UTF8& replace(int pos, int len, const UTF8& str, int subpos, int sublen); 

		/**
		 * Get the true index of the internal string given the
		 * index into a utf-8 string
		 */
		int true_idx(int idx);
		/**
		 * Get the true index of the internal string given the
		 * index into a utf-8 string, do not change the internal 
		 */
		int true_idx_q(int idx) const;
		int true_idx(int idx) const { return true_idx_q(idx); }
		/**
		 * get the character at the given index in the underlying character string
		 */
		void true_range(int& a, int& b);
		/**
		 * Get the true index of the internal string given the
		 * index into a utf-8 string
		 */
		void true_range_q(int& a, int& b) const;
		void true_range(int& a, int& b) const { true_range_q(a, b); }
		/**
		 * get the character at the given index in the underlying character string
		 */
		int get_true(int idx);
		/**
		 * get the character at the given index
		 *
		 * same as operator[]
		 */
		UTF8Char get(int idx);
		/**
		 * get the unicode value at the given index
		 *
		 * i.e. the value of str.get_u(0) if s is "€"
		 * would be 0x20AC (€ is unicode character U+20AC)
		 */
		unicode_t get_u(int idx);
		/**
		 * set the value at the given index to the given character
		 */
		/**
		 * return the underlying string value, as a reference
		 * if there's anything unimplemented, it can be done with this
		 */
		std::string& get_string() { return raw_string; }
		const std::string& get_string() const { return raw_string; }
		void set(int idx, UTF8Char c);
		/**
		 * return the corresponding c string
		 */
		const char* c_str();
		/**
		 * return the corresponding c string,
		 *     not necessarily null terminated
		 */
		const char* data();

		/**
		 * the raw string used for all calculations
		 */
		std::string raw_string;
		int n_chars = -1;
		int access_idx = 0;
		int access_true_idx = 0;

	private:
		int calculate_length() const;
		void inc_n_chars(int amt = 1) { if(n_chars != -1) n_chars+=amt;}
};

/**
 * a class representing a single utf-8 char
 *
 * this can theoretically be done with an int,
 * but this is done for various reasons including that this allows
 * the first char to always be the most significant,
 * which makes calculations easier.
 */
class UTF8Char {
	public:
		UTF8Char(const char* c);
		UTF8Char(unicode_t c);
		char str[4] = {0}; //probably should set to zeros
		static char cstr[5]; //for c_str()
		bool operator==(const UTF8Char& b){return *((int*)str) == *((int*)b.str);};
		unicode_t uni() {return utf8_uni(str); }
		const char* c_str() const {memcpy(cstr, str, 4); return cstr; }
		std::string to_str() const {return std::string(str, utf8size(str));}
};
char UTF8Char::cstr[5];


class UTF8Ref {
	public:
		UTF8* parent;
		int idx;

		UTF8Ref ();
		UTF8Ref (const UTF8Ref& i) : parent{i.parent}, idx{i.idx} {}
		UTF8Ref (UTF8* parent, int idx) : parent{parent}, idx{idx} {}
		UTF8Ref& operator=(const UTF8Ref& b) { set_char(b.get_char()); return *this; } //assignment
		UTF8Ref& operator=(const UTF8Char& b) { set_char(b); return *this; } //assignment
		bool operator==(const UTF8Ref& b) { return get_char() == b.get_char(); }// equality
		UTF8Char get_char() const { return UTF8Char(parent->get(idx)); }
		const char* c_str() const { return get_char().c_str(); }
		void set_char(UTF8Char c) { parent->set(idx, c); }
		operator UTF8Char() { return get_char(); }
		operator unicode_t() { return get_char().uni(); }
		/* Doable but unimportant methods (adding to a unicode value is not very useful)
		UTF8Ref& operator++(); //pre-increment (++a)
		UTF8Ref operator++(int); //post-increment (a++)
		UTF8Ref& operator--(); //pre-decrement (--a)
		UTF8Ref operator--(int); //post-decrement (a--)
		UTF8Ref& operator+=(size_t i); //compund assignment
		UTF8Ref& operator-=(size_t i); //
		bool operator<(const UTF8Ref& b); //equality/inequality
		bool operator>(const UTF8Ref& b); //
		bool operator<=(const UTF8Ref& b); //
		bool operator>=(const UTF8Ref& b); //
		bool operator!=(const UTF8Ref& b); //
		UTF8Ref& operator+(size_t i); //arithmetic
		UTF8Ref& operator-(size_t i); //
		*/
};
/*
UTF8Ref& operator+(size_t i, const UTF8Ref& ref);
UTF8Ref& operator-(size_t i, const UTF8Ref& ref);
*/

/**
 * For random access iteration over
 * UTF8 strings
 */
class UTF8Iter {
	public:
		UTF8Ref ref;

		UTF8Iter ();
		UTF8Iter (const UTF8Iter& i) : ref{i.ref} {}
		UTF8Iter (UTF8* parent, int idx) : ref{parent, idx} {}
		UTF8Iter& operator++() { ++(ref.idx); return *this; } //pre-increment (++a)
		UTF8Iter operator++(int) { UTF8Iter r(*this); ++(r.ref.idx); return r; } //post-increment (a++)
		UTF8Iter& operator--()  { --(ref.idx); return *this; }//pre-decrement (--a)
		UTF8Iter operator--(int)  { UTF8Iter r(*this); --(r.ref.idx); return r; }//post-decrement (a--)
		UTF8Iter& operator+=(size_t i) { ref.idx += i; return *this; } //compund assignment
		UTF8Iter& operator-=(size_t i) { ref.idx -= i; return *this; } //
		bool operator<(const UTF8Iter& b) { return ref.idx < b.ref.idx; } //equality/inequality
		bool operator>(const UTF8Iter& b) { return ref.idx > b.ref.idx; } //
		bool operator<=(const UTF8Iter& b) { return ref.idx <= b.ref.idx; } //
		bool operator>=(const UTF8Iter& b) { return ref.idx >= b.ref.idx; } //
		bool operator==(const UTF8Iter& b) { return ref.idx == b.ref.idx && ref.parent == b.ref.parent; } //
		bool operator!=(const UTF8Iter& b) { return ref.idx != b.ref.idx; } //
		UTF8Iter& operator=(UTF8Iter b) { ref = b.ref; return *this; } //assignment
		UTF8Ref& operator*() { return ref; } //dereference
		UTF8Ref* operator->() { return &ref; } //dereference (must return pointer)[https://stackoverflow.com/a/28435758]
		UTF8Iter operator+(size_t i) { UTF8Iter r(*this); r.ref.idx += i; return r; } //arithmetic
		UTF8Iter operator+(UTF8Iter b) { UTF8Iter r(*this); r.ref.idx += b.ref.idx; return r; }//
		UTF8Iter operator-(size_t i) { UTF8Iter r(*this); r.ref.idx -= i; return r; }//
		UTF8Iter operator-(UTF8Iter b) { UTF8Iter r(*this); r.ref.idx -= b.ref.idx; return r; } //
		UTF8Ref operator[](size_t i) { return *(*this+i); } //offset dereference
};
UTF8Iter operator+(size_t i, const UTF8Iter& iter) { UTF8Iter r(iter); r.ref.idx += i; return r; }
UTF8Iter operator-(size_t i, const UTF8Iter& iter) { UTF8Iter r(iter); r.ref.idx -= i; return r; }

inline UTF8Char::UTF8Char(const char* c){
	int size = utf8size(c);
	memcpy(str, c, size);
}
inline UTF8Char::UTF8Char(unicode_t c){
	*this = uni_utf8(c);
}

inline UTF8::UTF8(std::string s){
	raw_string = s;
}
inline UTF8::UTF8(const char* s){
	raw_string = std::string(s);
}


//Get the number of chars in a utf character starting with s
//could be faaster to use a lookup table?
inline int utf8size(const char* s) {
	if(*s & 0x80){
		int i = 1;
		char shift = *s<<1;
		while(shift & 0x80){
			shift<<=1;
			i++;
		}
		if(i==1) return 0;
		return i;
	}
	return 1;
}

/// Convert utf8 to unicode value
inline unicode_t utf8_uni(const char* s) {
	int size = utf8size(s);
	// The unicode value for a 1-byte utf-8 character is the same as the character
	if(size==1) return *s;
	// the first part is related to the size of the utf-8 character (in bytes)
	// i.e. a 3-byte value starts with 1110xxx, only the xxxx is important
	// r is the return value
	int r = *s & 0x7f >> size;
	//the rest is 0b10xxxxxx repeated until the value is complete
	for(;size>1;size--){
		r = r<<6 | (*(++s) & 0x7f);
	}
	return r;
}
/// Convert utf8 to unicode value
inline unicode_t utf8_uni(const UTF8Char& s) {
	return utf8_uni(s.str);
}
/// Convert unicode value to utf8
inline UTF8Char uni_utf8(unicode_t c) {
	// anything smaller than 0b10000000 is a one-byte value
	if(c < 0x80){
		char utf8 = c;
		return UTF8Char(&utf8);
	}
	// the string will be built from the back, and has a max size of 4 bytes
	char str[4];
	// index in the string
	int i = 3;
	// mask for final length-detertmining byte
	char mask = 0x7f;
	while(c > mask){
		// each intermediate byte looks like 0b10xxxxxx
		str[i] = (c & 0x3f) | 0x80;
		c >>= 6;
		mask >>= 1;
		--i;
	}
	//create the final byte of the string
	str[i] = (~mask) | (c & (mask >> 1));
	return UTF8Char(str + i);
}

inline int UTF8::size() const{
	return raw_string.size();
}

inline int UTF8::length(){
	if(n_chars == -1){
		n_chars = calculate_length();
	}
	return n_chars;
}
inline int UTF8::length() const{
	int n = n_chars;
	if(n == -1){
		n = calculate_length();
	}
	return n;
}

inline UTF8Iter UTF8::begin(){
	return UTF8Iter(this, 0);
}
inline UTF8Iter UTF8::end(){
	return UTF8Iter(this, length());
}

inline UTF8Ref UTF8::operator[](int idx){
	return UTF8Ref(this, idx);
}
inline UTF8& UTF8::push_back(const UTF8Char& c){
	inc_n_chars();
	raw_string.append(c.str, utf8size(c.str));
	return *this;
}
inline UTF8& UTF8::push_front(const UTF8Char& c){
	int size = utf8size(c.str);
	if(access_idx > 0){
		++access_idx;
		access_true_idx += size;
	}
	raw_string.insert(0, c.str, size);
	inc_n_chars();
	return *this;
}
inline UTF8& UTF8::insert(int idx, const UTF8Char& c){
	int size = utf8size(c.str);
	if(access_idx > idx){
		++access_idx;
		access_true_idx += size;
	}
	inc_n_chars();
	raw_string.insert(idx, c.str, size);
	return *this;
}
inline UTF8& UTF8::insert(int idx, UTF8& c){
	int l = c.length();
	if(access_idx > idx){
		access_idx += l;
		access_true_idx = c.size();
	}
	inc_n_chars(l);
	raw_string.insert(true_idx_q(idx), c.raw_string);
	return *this;
}
inline UTF8& UTF8::insert(int idx, const UTF8& c){
	int l = c.length();
	if(access_idx > idx){
		access_idx = 0;
		access_true_idx = 0;
	}
	inc_n_chars(l);
	raw_string.insert(true_idx_q(idx), c.raw_string);
	return *this;
}
inline UTF8& UTF8::insert(int idx, const std::string& c){
	if(access_idx > idx){
		access_idx = 0;
		access_true_idx = 0;
	}
	n_chars = -1;
	raw_string.insert(true_idx(idx), c);
	return *this;
}
inline UTF8& UTF8::insert(int idx, const char* c){
	if(access_idx > idx){
		access_idx = 0;
		access_true_idx = 0;
	}
	n_chars = -1;
	raw_string.insert(true_idx(idx), c);
	return *this;
}
inline UTF8& UTF8::erase(int pos, int len){
	int er_begin = true_idx(pos);
	int er_end = true_idx_q(pos + len);
	inc_n_chars(-std::min(len, length() - pos));
	raw_string.erase(er_begin, er_end-er_begin);
	return *this;
}
inline UTF8& UTF8::replace(int pos, int len, const UTF8& s){
	int er_begin = true_idx(pos);
	int er_end = true_idx_q(pos + len);
	inc_n_chars(-std::min(len, length() - pos)+s.length());
	raw_string.replace(er_begin, er_end-er_begin, s.get_string());
	return *this;
}
inline UTF8& UTF8::replace(int pos, int len, const UTF8& s, int subpos, int sublen){
	int begin_a = true_idx(pos);
	int end_a = true_idx_q(pos + len);
	int begin_b = s.true_idx(subpos);
	int end_b = s.true_idx_q(subpos + sublen);
	//true_range(begin_a, end_a);
	//s.true_range(begin_b, end_b);
	inc_n_chars(-std::min(len, length() - pos)+std::min(sublen, s.length() - subpos));
	raw_string.replace(begin_a, begin_a-end_a, s.get_string(), begin_b, begin_b-end_b);
	return *this;
}

		
inline UTF8Char UTF8::get(int idx){
	const char* d = raw_string.data();
	int str_i = true_idx(idx);
	return UTF8Char(d+str_i);
}
inline unicode_t UTF8::get_u(int idx){
	const char* d = raw_string.data();
	int i=0;
	while(i<idx){
		d+=utf8size(d);
		i++;
	}
	return utf8_uni(d);
}
inline void UTF8::set(int idx, UTF8Char c){
	const char* d = raw_string.data();
	int str_i = true_idx(idx);
	raw_string.replace(str_i, utf8size(d+str_i), c.str, utf8size(c.str));
}
inline int UTF8::true_idx(int idx){
	const char* d = raw_string.data();
	int i=0;
	int str_i=0;
	if(idx >= access_idx){
		i = access_idx;
		str_i = access_true_idx;
	}
	while(i<idx){
		str_i += utf8size(d+str_i);
		i++;
	}
	access_idx = i; 
	access_true_idx = str_i;
	return str_i;
}
inline int UTF8::true_idx_q(int idx) const{
	const char* d = raw_string.data();
	int i=0;
	int str_i=0;
	if(idx >= access_idx){
		i = access_idx;
		str_i = access_true_idx;
	}
	while(i<idx){
		str_i += utf8size(d+str_i);
		i++;
	}
	return str_i;
}
inline void UTF8::true_range(int& a, int& b){
	a = true_idx(a);
	b = true_idx_q(b);
}
inline void UTF8::true_range_q(int& a, int& b) const{
	a = true_idx_q(a);
	b = true_idx_q(b);
}
inline int UTF8::get_true(int idx){
	return raw_string.at(idx);
}
inline const char* UTF8::c_str(){
	return raw_string.c_str();
}
inline const char* UTF8::data(){
		return raw_string.data();
}

inline int UTF8::calculate_length() const{
	const char* d = raw_string.data();
	int i=0;
	int str_i=0;
	while(str_i<raw_string.size()){
		str_i += utf8size(d+str_i);
		i++;
	}
	return i;
}
