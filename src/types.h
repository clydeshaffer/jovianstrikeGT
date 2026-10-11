typedef struct two_bytes_t {
	char lsb;
	char msb;
} two_bytes_t;

typedef union fixedp {
	two_bytes_t b;
	unsigned int u;
	signed int i;
} fixedp;


typedef char (*CharVoidFunction)(void);
typedef void (*VoidFunction)(void);
