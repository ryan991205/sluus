#ifndef IDOOR_H
#define IDOOR_H


class IDoor
{
	public:
		virtual ~IDoor() {};

		virtual void Open() = 0;

		virtual void Close() = 0;

		virtual void Stop() = 0;
};

#endif
