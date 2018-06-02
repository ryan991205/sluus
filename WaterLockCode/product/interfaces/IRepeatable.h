#ifndef IREPEATABLE_H
#define IREPEATABLE_H

class IRepeatable
{
	public:
		virtual ~IRepeatable() {};

		virtual void RepeatDoorOpenCommand() = 0;
		virtual void RepeatDoorCloseCommand() = 0;
};

#endif
