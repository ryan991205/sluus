#ifndef IREPEATABLE_H
#define IREPEATABLE_H

class IRepeatable
{
	public:
		virtual ~IRepeatable() {};

		virtual void Repeat() = 0;
};

#endif
