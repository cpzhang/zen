#pragma once
#include "ConfigMisc.h"
#include "stdHead.h"
class MemoryFileWriter
{
public:
	MemoryFileWriter();
	~MemoryFileWriter();
public:
	bool isEnd();

	bool isOverFlow(size_t s);

	bool reallocate(size_t s);

	bool write(void* b, size_t s);

	bool _writeImp(void* b, size_t s);

	size_t getSize();

	void* getBuffer();
private:
	void*			_buffer;
	unsigned int	_cursor;
	unsigned int	_length;
};

struct sChunk
{
public:
	sChunk()
	{
	}
	virtual ~sChunk()
	{
	}
	virtual void writeTag(std::ofstream& o)
	{

	}
public:
	MemoryFileWriter buffer;
};

struct sChunkName: public sChunk
{
public:
	sChunkName()
	{
	}
	~sChunkName()
	{
	}
	virtual void writeTag(std::ofstream& o)
	{
		char c[4];
		c[0] = name[3];
		c[1] = name[2];
		c[2] = name[1];
		c[3] = name[0];

		o.write(c, 4);
	}
	std::string name;
};

struct sChunkID: public sChunk
{
public:
	sChunkID()
	{
		name = 0;
	}
	~sChunkID()
	{
	}
	virtual void writeTag(std::ofstream& o)
	{
		o.write((char*)&name, sizeof(int));
	}
	int name;
};

class ApiMisc_ ChunkSet
{
public:
	std::vector<sChunk*> _chunkSet;
	sChunk*			_currentChunk;

public:
	ChunkSet();
	~ChunkSet();
public:
	bool beginChunk(const std::string& name);
	bool beginChunk(const int name);
	void endChunk();

	bool write(const char* b, size_t s);

	bool write(void* b, size_t s);

	void save(const std::string& fileName);

	MemoryFileWriter* getCurrentMemoryFileWriter();
};

template<class T>
void writeSequence(ChunkSet& cs, T &t)
{
	T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		cs.write(&(*i),sizeof(*i));
	}
}

template<class T>
void writeSequenceEx(ChunkSet& cs, T &t)
{
	u32 nBones = t.size();
	cs.write(&nBones, sizeof(nBones));		
	T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		cs.write(&(*i),sizeof(*i));
	}
}

template<class T, class Ele>
void readSequenceEx(std::ifstream& f, T &t)
{
	u32 nBones;
	f.read((char*)&nBones, sizeof(nBones));		
	t.resize(nBones);
	for(int i = 0; i < nBones; ++i)
	{
		Ele e;
		f.read((char*)&e, sizeof(Ele));
		t[i] = e;
	}
}
