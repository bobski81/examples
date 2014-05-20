#define SIZE 100  // 100 Characters deep buffer

struct Face {
	int noEyes;
	int noNose;
};


typedef struct {
	char name[SIZE];
	char year[SIZE];
	struct Face face;
} Person;

