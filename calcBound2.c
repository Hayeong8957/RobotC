#pragma config(Sensor, S1, c1, sensorEV3_Color)
#pragma config(Sensor, S2, c2, sensorEV3_Color)
#pragma config(Sensor, S3, c3, sensorEV3_Color)
#pragma config(Motor, motorB, lm, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor, motorC, rm, tmotorEV3_Large, PIDControl, encoder)

int speed = 10;

int calcBound2()
{
	int bound;
	int black = 0;
	int white = 0;

	for(int i = 0; i < 5; i++)
	{
		black += getColorReflected(c1);
		black += getColorReflected(c3);
		sleep(10);
	}
		setMotorSpeed(lm, speed);
		setMotorSpeed(rm, speed);
		sleep(500);
		setMotorSpeed(lm, 0);
		setMotorSpeed(rm, 0);

		for(int i = 0; i < 5; i++)
		{
			white += getColorReflected(c1);
			white += getColorReflected(c3);
			sleep(10);
		}

		bound = (white/10 + black/10)/2;
		return bound;
	}

task main()
{
	int gray = calcBound2();

	while(1)
	{
		int left = getColorReflected(c1);
		int right = getColorReflected(c3);

		if(left >= gray && right >= gray)
		{
			setMotorSpeed(lm, speed);
			setMotorSpeed(rm, speed);
		}
		else if(left >= gray && right > gray)
		{
			setMotorSpeed(lm, speed);
			setMotorSpeed(rm, -speed);
		}
		else if(left<gray && right>=gray)
		{
			setMotorSpeed(lm, -speed);
			setMotorSpeed(rm, speed);
		}
		else
		{
			setMotorSpeed(lm, speed);
			setMotorSpeed(rm, speed);
			sleep(200);
		}
	}
}
