#ifndef IVALVE_H
#define IVALVE_H


class IValve
{
	public:
		virtual ~IValve() {};

		virtual void Open() = 0;

		virtual void Close() = 0;
};

#endif
