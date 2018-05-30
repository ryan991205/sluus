#ifndef IUSERINPUTEVENTGENERATOR_H
#define IUSERINPUTEVENTGENERATOR_H


class IUserInputEventGenerator
{
	public:
		virtual ~IUserInputEventGenerator() {};

		virtual void StartButtonPressed() = 0;

		virtual void ReleaseInsideButtonPressed() = 0;

		virtual void ReleaseOutsideButtonPressed() = 0;

		virtual void EmergencyButtonPressed() = 0;

		virtual void ResumeNormalOperationsButtonPressed() = 0;
};

#endif
