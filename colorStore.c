#pragma config(Sensor, S1, c1, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S2, c2, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S3, c3, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Motor, motorB, lm, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor, motorC, rm, tmotorEV3_Large, PIDControl, encoder)
#define DIFF 10

int nMotorSpeedSetting = 35, vertex = 0, count = 0, row = 0, val, temp_color, max_color, init = 0,
r = 0, c = 0, i, j, f, e, score;
char M[4][4];
int S[4][4], dt[4][4], d[4], J[4][4];

void colorSearchTurn()
{
      setMotorSpeed(lm, 15);
      setMotorSpeed(rm, -15);
      sleep(150);
      setMotorSpeed(lm, 0);
      setMotorSpeed(rm, 0);
      sleep(50);
}

int patchSector()
{
   int i = 0, max_color = 0 , temp_color = 0;
   setMotorSpeed(lm, 0);
   setMotorSpeed(rm, 0);

   max_color = getColorName(c2);

   if (init == 0 ){
         displayBigTextLine(1, "S[%d][%d] = NOT", row , count);
         M[row][count] = 0;
         sleep(1000);
         init++;
   }
   else if (row % 2 == 0){
      if(max_color == 2){
         displayBigTextLine(1,"S[%d][%d] = BLUE", row, ++count);
         M[row][count] = -1;
         sleep(1000);
      }
      else if (max_color == 5){
         displayBigTextLine(1, "S[%d][%d] = RED", row, ++count);
         M[row][count] = 1;
         sleep(1000);
      }
      else{
         displayBigTextLine(1, "S[%d][%d] = NOT", row , ++count);
         M[row][count] = 0;
         sleep(1000);
      }
   }

   else if(row % 2!= 0){
      if(max_color == 2){
         displayBigTextLine(1,"S[%d][%d] = BLUE", row, 3-(++count));
         M[row][3-count] = -1;
         sleep(1000);
      }
      else if(max_color == 5){
         displayBigTextLine(1,"S[%d][%d] = RED", row, 3-(++count));
         M[row][3-count] = 1;
         sleep(1000);
      }
      else{
         displayBigTextLine(1,"S[%d][%d] = NOT", row, 3-(++count));
         M[row][3-count] = 0;
         sleep(1000);
      }
   }
   setLEDColor(0);
   return max_color;
}

void go()
{
   val = 5;

   if(getColorName(c2) == 4)
   {
      setMotorSpeed(lm, nMotorSpeedSetting - val);
      setMotorSpeed(rm, nMotorSpeedSetting + val);
   }
   else
   {
      setMotorSpeed(lm, nMotorSpeedSetting + val);
      setMotorSpeed(rm, nMotorSpeedSetting - val);
   }
   if(row == 1)
   {
      if(getColorName(c1) == 4 && count != 4)
      {
         vertex++;
      }
      else
         vertex = 0;
  }
  else if(row == 3)
  {
     if(getColorName(c1) == 4 && count != 4)
     {
        vertex++;
     }
     else
        vertex = 0;
  }
  else if(row == 4)
  {
     if(getColorName(c1) == 4 && count != 4)
     {
        vertex++;
     }
     else
        vertex = 0;
  }
  else
  {
     if(getColorName(c3) == 4 && count != 4)
     {
        vertex++;
     }
     else
        vertex = 0;
  }
   // Edit motot speed
  if(vertex == 1)
  {
     if(row == 0 || row == 2)
     {
       colorSearchTurn();
       sleep(150);
       setMotorSpeed(lm, 12);
       setMotorSpeed(rm, 10);
       sleep(80);
     }
     else
     {
       colorSearchTurn();
       sleep(150);
       setMotorSpeed(lm, 10);
       setMotorSpeed(rm, 12);
       sleep(80);
     }
     S[row][count] = patchSector();
  }
}

void turnLeft()
{
   setMotorSpeed(lm, 25);
   setMotorSpeed(rm, 25);
   sleep(250);
   while(getColorName(c1) > 4)
   {
      setMotorSpeed(lm, -nMotorSpeedSetting * 6 / 10);
      setMotorSpeed(rm, nMotorSpeedSetting * 6 / 10);
      sleep(10);
   }
   while(getColorName(c2) > 4)
   {
      setMotorSpeed(lm, -nMotorSpeedSetting * 6 / 10);
      setMotorSpeed(rm, nMotorSpeedSetting * 6 / 10);
   }
   sleep(80);
   setMotorSpeed(lm, 0);
   setMotorSpeed(rm, 0);
   sleep(200);
   count = 0;
}

void turnRight()
{
   setMotorSpeed(lm, 25);
   setMotorSpeed(rm, 25);
   sleep(250);
   while(getColorName(c3) > 4)
   {
      setMotorSpeed(lm, nMotorSpeedSetting * 6 / 10);
      setMotorSpeed(rm, -nMotorSpeedSetting * 6 / 10);
      sleep(10);
   }
   while(getColorName(c2) > 4)
   {
      setMotorSpeed(lm, nMotorSpeedSetting * 6 / 10);
      setMotorSpeed(rm, -nMotorSpeedSetting * 6 / 10);
   }
   sleep(80);
   setMotorSpeed(lm, 0);
   setMotorSpeed(rm, 0);
   sleep(200);
   count = 0;
}

void goUp()
{
    row++;
    turnRight();
    count = 0;
    while(true)
    {
        go();
        if (count == 1)
        {
            sleep(300);
            turnLeft();
            break;
        }
    }
}

void goLeft()
{
    count = 0;
    while(true)
    {
        go();
        if (count == 1)
        {
            setMotorSpeed(lm,30);
            setMotorSpeed(rm,30);
            sleep(400);
            break;
        }
    }
}

void completeSearch()
{
    while(true)
    {
        go();
        if (count == 3)
        {
            if(row == 3)
            {
                return;
            }

            if (row % 2 == 0)
            {
                setMotorSpeed(lm,35);
                setMotorSpeed(rm,30);
                sleep(200);
                turnRight();
            }

            else
            {
                setMotorSpeed(lm,30);
                setMotorSpeed(rm,35);
                sleep(200);
                turnLeft();
            }

            row++;

            if(row%2 == 0)
            {
                while(getColorName(c1) == 6)
                {
                    go();
                }
                colorSearchTurn();
                      sleep(150);
                     setMotorSpeed(lm, 10);
                      setMotorSpeed(rm, 12);
                      sleep(80);

                count = -1;
                S[row][3] = patchSector();

                setMotorSpeed(lm,30);
                setMotorSpeed(rm,35);
                sleep(200);
                turnLeft();
            }

            else
            {
                while(getColorName(c3) == 6)
                {
                    go();
                }
                colorSearchTurn();
                      sleep(150);
                      setMotorSpeed(lm, 10);
                    setMotorSpeed(rm, 12);
                      sleep(80);

                count = -1;
                S[row][0] = patchSector();

                setMotorSpeed(lm, 35);
                setMotorSpeed(rm, 30);
                sleep(200);
                turnRight();
            }
            count = 0;
            sleep(200);
        }
    }
}

void printMap()
{
   char map[4][4];
   for(int i = 0; i < 4; i++)
   {
      for(int j = 0; j < 4; j++)
      {
         if(M[i][j] == 0)
         {
           map[i][j] = '+';
         }
         else if(M[i][j] == 1)
         {
           map[i][j] = 'O';
         }
         else
         {
           map[i][j] = 'X';
         }
      }
   }
   for(int p = 0; p < 4; p++)
   {
      for(int q =0; q < 4; q++)
      {
         displayStringAt((q+1)*10, 100-(p+1)*10, "%c",map[p][q]);
      }
   }
   sleep(3000);
}

void stopMotor()
{
   setMotorSpeed(lm, 0);
   setMotorSpeed(rm, 0);
}

void movePoint_44()
{
	count = 0;
	while(true)
	{
		go();
		if (count == 3)
		{
			setMotorSpeed(lm, 35);
			setMotorSpeed(rm, 30);
			sleep(400);
			turnLeft();
			turnLeft();
			setMotorSpeed(lm,-30);
			setMotorSpeed(rm,-30);
			sleep(350);
			if(getColorName(c1) == 4 && getColorName(c3) == 4)
			{
				setMotorSpeed(lm, 0);
				setMotorSpeed(rm, 0);
				sleep(100);
			}
			break;
		}
	}
}

task main()
{
   while(getButtonPress(1) == 0)
   {
      sleep(10);
   }

   completeSearch();
   setMotorSpeed(lm,30);
   setMotorSpeed(rm,30);
   sleep(350);
   turnLeft();
   turnLeft();
   //sleep(1000);

   sleep(2000);
   printMap();
   sleep(3000);

   movePoint_44();

}
