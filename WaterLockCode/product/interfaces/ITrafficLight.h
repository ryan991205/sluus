#ifndef ITRAFFICLIGHT_H
#define ITRAFFICLIGHT_H


class ITrafficLight
{
	public:
		virtual ~ITrafficLight() {};

		virtual void Green() = 0;

		virtual void Red() = 0;
};

#endif
