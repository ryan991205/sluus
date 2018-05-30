#ifndef ILOCK_H
#define ILOCK_H

class ILock
{
	public:
		virtual ~ILock() {};

		virtual void Lock() = 0;

		virtual void Unlock() = 0;
};

#endif
