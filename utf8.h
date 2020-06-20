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
int utf8uni(const char* s);

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
		char str[4];
};

/**
 * For random access iteration over
 * UTF8 strings
 */
class UTF8Iter {

}

/**
 * A string for working with utf-8
 *
 * All indexing is done with ints, as they're small
 * and if you're indexing over the range of a sighned integer 
 * you probably want to be using this
 * with something else at the very least
 */
class UTF8 {
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
		 * Get the character at the given index
		 *
		 * not the same as the character at the given index of the internal string,
		 * as utf-8 is variable with
		 */
		UTF8Char operator[](int idx);
		/**
		 * 	Get the character at the given index of the internal string
		 */
		int true_idx(int idx);
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
		void set(int idx, UTF8Char c);
		UTF8& replace(int pos, int len, const UTF8& str); 
		UTF8& replace(int pos, int len, const UTF8& str, int subpos, int sublen); 
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

	private:
};

inline UTF8Char::UTF8Char(const char* c){
	int size = utf8size(c);
	memcpy(str, c, size);
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

//Convert utf8 to unicode value
inline int utf8uni(const char* s) {
	int size = utf8size(s);
	if(size==1) return *s;
	int r = *s & 0x7f >> size;
	for(;size>1;size--){
		r = r<<6 | (*(++s) & 0x7f);
	}
	return r;
}

inline UTF8Char UTF8::operator[](int idx){
	return get(idx);
}
inline UTF8Char UTF8::get(int idx){
	const char* d = raw_string.data();
	int i=0;
	while(i<idx){
		d+=utf8size(d);
		i++;
	}
	return UTF8Char(d);
}
inline unicode_t UTF8::get_u(int idx){
	const char* d = raw_string.data();
	int i=0;
	while(i<idx){
		d+=utf8size(d);
		i++;
	}
	return utf8uni(d);
}
inline void UTF8::set(int idx, UTF8Char c){
	const char* d = raw_string.data();
	int i=0;
	int str_i=0;
	while(i<idx){
		str_i += utf8size(d+str_i);
		i++;
	}
	raw_string.replace(str_i, utf8size(d+str_i), c.str, utf8size(c.str));
}
inline int UTF8::true_idx(int idx){
	return raw_string[idx];
}
inline const char* UTF8::c_str(){
	return raw_string.c_str();
}
inline const char* UTF8::data(){
		return raw_string.data();
}

