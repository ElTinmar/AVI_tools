#include <stdio.h>

typedef unsigned char BYTE;
typedef unsigned int DWORD;

int show_list(FILE *in, int length, int* numframes);

class FourCC
{
public:
	DWORD code;
	int read(FILE *in);
	bool Equals(char *str) {  return (*(DWORD *)str == code); }
	void show();
};

class Chunk
{
public:
	FourCC type;
	DWORD length;
	int read(FILE *in);
	void show();
	void skip(FILE *in);
	bool Equals(char *str) { return (*(DWORD *)str == type.code); }
};

int main(int argc, char *argv[])
{
	FILE *in;
	char *filename;
    int endoffile = 0;
    int numframes = 0;

	if (argc<2) {
		printf("usage: chunks filename\n");
		return -1;
	}
	filename = argv[1];
	in = fopen(filename,"rb");
	if (!in) {
		printf("file not found\n");
		return -1;
	}
	while (endoffile==0) {
		Chunk hd;
		hd.read(in);
		if (hd.Equals("RIFF")) {
            endoffile = show_list(in,hd.length,&numframes);
        }
	}
    printf("#Frames: %d\n",numframes);
	return 0;
}

inline int GetLong(char *buf)
{
	return (*(long *)buf);
}

inline short GetShort(char *buf)
{
	return (*(short *)buf);
}

int show_list(FILE *in, int length, int* numframes)
{
	int i, iret, count;
	FourCC type;
	Chunk ck;
	type.read(in);
	count = 4; //  count includes code just read
	while (count<length) {
		iret = ck.read(in);
		if (iret) break;
		if (ck.Equals("00dc")) *numframes=*numframes+1;
		if (ck.Equals("LIST")) show_list(in, ck.length, numframes);
		else ck.skip(in);
		count += ck.length+8;
		if (ck.length&1) count++; // account for extra odd byte
	}
	if (iret) return 1;
    return 0;
}

int Chunk::read(FILE *in)
{
	int i;
	char buf[8];
	fread(buf,sizeof(char),8,in);
	if (feof(in)) return -1;
	type.code = GetLong(buf);
	length = GetLong(buf+4);
	return 0;
}

void Chunk::skip(FILE *in)
{
	long nbytes;
	nbytes = (length&1? length+1: length); // round up to even word boundary
	fseek(in,nbytes,SEEK_CUR);
}

int FourCC::read(FILE *in)
{
	fread((char *) &code,sizeof(FourCC),1,in);
	return (feof(in)? -1: 0);
}

