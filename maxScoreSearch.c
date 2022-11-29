#pragma config(Sensor, S1, c1, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S2, c2, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S3, c3, sensorEV3_Color, modeEV3Color_Color)
#pragma config(Motor, motorB, lm, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor, motorC, rm, tmotorEV3_Large, PIDControl, encoder)

#define DIFF 10

// for using color value
#define Black 1
#define Blue 2
#define Green 3
#define Yellow 4
#define Red 5
#define White 6
#define Brown 7

// (0,0)에서 출발하여 (0,0)까지 최단 경로로 이동하면서 가장 많은 패치를 획득하는 문제
// 크기가 작은 각 문제를 해결한 결과(답)를 배열로 구현한 동적 테이블에 저장하고,
// 이 값을 이용하여 더 큰 문제를 해결
// dt[a][b] : (0,0)부터 출발하여 (a, b)까지 이동하여 얻을 수 있는 최대점수
// 위치 (a,b) 바로 이전의 위치는 최단 경로로 이동해야하기 때문에 (a-1, b)와 (a, b-1)뿐이다.
// dt[a][b] = max(dt[a-1][b], dt[a][b-1]) + (a, b)의 패치 수


int nMotorSpeedSetting = 35, vertex = 0, count = 0, row = 0, val, r = 0,
c = 0, i, j, f, e;
int S[5][5], dt[5][5], d[5], J[5][5];
// 배열 s[][] -> 패치의 위치
// dt[][] -> 최대 패치의 수
// d[] -> 최대 패치의 수를 계산하는 과정에서 값을 임시로 저장하는 공간
// d[] -> 최고 득점 경로를 주행할 때 각 열에서 가야하는 행 저장
// J[][] -> 이전 열의 행 번호

// 1센서 라인 트레이싱과 로봇의 위치를 파악하는 함수
void go()
{
	val = 7;
	// 빨간색 패치가 있다는 것, 배열에 빨간색 패치를 나타내는 1을 저장,
	if(getColorName(c2) == Red)
	{
		if(row % 2 == 0) S[row][count] = 1;
		else S[row][3-count] = 1;
		// 행 번호가 1, 3일 경우에는 주의 : 로봇의 진행 방향이 다르기 때문에
		// S[row][count]에 1을 저장하는 것이 아니라 S[row][3-count]에 1을 저장
		playTone(440, 20); sleep(100);
	}
	// 파란색 패치가 있다는 것, 배열에 파란색 패치를 나타내는 -1을 저장
	else if(getColorName(c2) == Blue)
	{
		if(row % 2 == 0) S[row][count] = -1;
		else S[row][3-count] = -1;
		playTone(400, 20);
		sleep(100);
	}
	if(getColorName(c2) == Yellow)
	{
		setMotorSpeed(lm, nMotorSpeedSetting - val);
		setMotorSpeed(rm, nMotorSpeedSetting + val);
	}
	else
	{
		setMotorSpeed(lm, nMotorSpeedSetting + val);
		setMotorSpeed(rm, nMotorSpeedSetting - val);
	}
			if(getColorName(c1) == Yellow || getColorName(c3) == Yellow)
			vertex++;
			else vertex = 0;

	if(vertex == 1) count++;
}

void turnLeft()
{
	setMotorSpeed(lm, 20);
	setMotorSpeed(rm, 35);
	sleep(300);
	while(getColorName(c1) > Yellow)
	{
		setMotorSpeed(lm, -nMotorSpeedSetting * 6/10);
		setMotorSpeed(rm, nMotorSpeedSetting * 6/10);
		sleep(10);
	}
	while(getColorName(c2) > Yellow)
	{
		setMotorSpeed(lm, -nMotorSpeedSetting * 6/10);
		setMotorSpeed(rm, nMotorSpeedSetting * 6/10);
		sleep(5);
	}
	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
	sleep(100);
}

void turnRight()
{
	setMotorSpeed(lm, 30);
	setMotorSpeed(rm, 30);
	sleep(100);
	while(getColorName(c3) > Yellow)
	{
		setMotorSpeed(lm, nMotorSpeedSetting * 6/10);
		setMotorSpeed(rm, -nMotorSpeedSetting * 6/10);
		sleep(10);
	}
	while(getColorName(c2) > Yellow)
	{
		setMotorSpeed(lm, nMotorSpeedSetting * 6/10);
		setMotorSpeed(rm, -nMotorSpeedSetting * 6/10);
		sleep(5);
	}
	sleep(200);
	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
	sleep(100);
}

void stopMotor()
{
	setMotorSpeed(lm, 0);
	setMotorSpeed(rm, 0);
}

void completeSearch()
{
	while(true)
	{
		go();
		if(count == 3)
		{
			if(row == 3) return;
			if(row % 2 == 0) // 0, 2 번째 행
			{
				setMotorSpeed(lm, 40);
				setMotorSpeed(rm, 35);
				// 행 끝에서 빨간색 패치 & 파란색 패치의 유무를 확인하기 위해
				// 로봇이 회전하는 과정에서 빨간색 & 파란색 패치를 인식하지 못하는 경우
				// 반복문으로 오랫동안 빨간색 & 파란색 패치의 유무를 확인
				for(int i = 0; i < 3; i++)
				{
					if(getColorName(c2) == Red)
					{
						if(row % 2 ==0) S[row][count] = 1;
						else S[row][3-count]=1;
						playTone(440, 20);
						sleep(80);
					}
					else if(getColorName(c2) == Blue)
					{
						if(row % 2 ==0) S[row][count] = -1;
						else S[row][3-count] = -1;
						playTone(400, 20);
						sleep(80);
					}
					else sleep(90);
				}
				turnRight();
			}
			else // 1, 3번째 행
			{
				// 행 끝에서 빨간색 패치의 유무를 확인하기 위해
				for(int i = 0; i < 3;i++)
				{
					if(getColorName(c2) == Red)
					{
						if(row % 2 == 0) S[row][count] = 1;
						else S[row][3-count] = 1;
						playTone(440, 20);
						sleep(80);
					}
					else if(getColorName(c2) == Blue)
					{
						if(row % 2 == 0) S[row][count] = -1;
						else S[row][3-count] = -1;
						playTone(400, 20);
						sleep(80);
					}
					else sleep(90);
				}
				turnLeft();
			}
			if(row % 2 == 0)
			{
				while(getColorName(c3) == White) go();
				setMotorSpeed(lm, 35);
				setMotorSpeed(rm, 30);
				sleep(400);
				turnRight();
			}
			else
			{
				while(getColorName(c1) == White) go();
				setMotorSpeed(lm, 30);
				setMotorSpeed(rm, 35);
				sleep(400);
				turnLeft();
			}
			row++;
			count = 0;
			// 새로운 행의 시작점에서도 빨간색 패치가 있을 수 있다.
			// 로봇이 회전을 하고 나면 센서 c2는 이미 시작점보다 앞에 위치
			// 따라서 회전 후, 후진을 하여 시작점의 빨간색 패치 유무 확인
			setMotorSpeed(lm, -30);
			setMotorSpeed(rm, -30);
			sleep(410);
		}
	}
}

// 열 방향으로 로봇이 -1칸 이동하는 함수
void goUp()
{
	r--; row++; // 이 경우, 행 번호가 달라지기 때문에 r을 감소시킨다.
	turnRight(); // 로봇은 행 방향으로 향하고 있기 때문에 우회전을 하여 로봇의 방향을 전환
	count = 0;
	while(true)
	{
		go();
		if(count == 1) // 1칸 진행 후, 행 방향으로의 움직임을 위하여 다시 방향을 전환시켜야 한다.
		{
			sleep(300);
			turnLeft(); // 따라서 좌회전을 한 후, 함수를 종료시킨다.
			break;
		}
	}
}

// 열 방향으로 로봇이 +1칸 이동하는 함수. goUp()함수와 반대방향
void goDown()
{
	r++; row--;
	turnLeft();
	count = 0;
	while(true)
	{
		go();
		if(count ==1)
		{
			sleep(300);
			turnRight();
			break;
		}
	}
}
// 행 방향으로 로봇이 -1칸 이동하는 함수
void goLeft()
{
	c--; // 열 번호가 바뀌기 때문에 c를 감소시킨다.
	count = 0;
	while(true)
	{
		go();
		if(count ==1)
		{
			setMotorSpeed(lm, 30);
			setMotorSpeed(rm, 30);
			sleep(400);
			break;
		}
	}
}

task main()
{
//	while(getButtonPress(1) == 0) sleep(10);
	completeSearch();
	setMotorSpeed(lm, 20);
	setMotorSpeed(rm, 20);
	sleep(1000);
	setMotorSpeed(lm, 20);
	setMotorSpeed(rm, -20);
	sleep(1650);

	count = row = 0;

// 그래프를 탐색한 결과를 토대로 최고 득점 경로를 계산
// (0,0)부터 (3, 3)까지의 위치에 대해 다음 계산을 각각 수행. 
// e는 행이고, f는 열
	// 최고 득점 경로 계산 반복문 -> dt와 S배열을 이용 -> 함수로 뺄 수 있는 부분
	for(f = 3; f >= 0; f--)
		for(e = 3; e>= 0; e--)
		{
			if( e == 3 && f == 3) dt[e][f] = S[e][f]; // 만약 (3,3)라면 해당 위치의 dt[e][f] = S[e][f]
			else if( f == 3) dt[e][f] = dt[e+1][f] + S[e][f];
			// 만약 (3,3)아니고, 3열에 있으면 해당 위치의 dt[e][f]는 dt[e+1][f]에 S[e][f]를 더한 값
			else
			{
				// 다른 경우, (e, f)의 모든 이전 열의 위치로부터 해당 위치로 올 때에 얻을 수 있는 dt[][]값을 계산
				dt[e][f] = -10; // dt[e][f]를 매우 작은 값으로 초기화
				for(i = 0; i <= 3; i++)
				{
					d[i] = dt[i][f+1];  // 해당 위치의 이전 열 중 i행 위치(이전위치)의 dt에 저장된 값을 배열 d에 저장
					if(i > e) // 이전 위치의 행이 형재 위치의 행보다 클 경우, 현재 위치의 S값과 현재 위치의 열 중 더 큰 행들의 S값을 d에 더함
					{
						for(j = i; j >= e; j--)
						d[i] += S[j][f];
					}
					else if(i == e)
					{// 이전 위치의 행이 현재 위치의 행과 같을 경우, d에 현재 위치의 S값을 d에 더한다.
						d[i] += S[i][f];
					}
					else
					{// 이전 위치의 행이 현재 위치의 행보다 작을 경우, 현재 위치의 S값과 현재 위치의 열 중 더 작은 행들의 S값을 d에 더한다.
						for(j = i; j <= e; j++)
						d[i] += S[j][f];
					}
					if(dt[e][f] < d[i])
					{// 현재 위치의 dt[][]값을 구하기 위해, d[] 값 중 가장 큰 값을 dt에 저장
						dt[e][f] = d[i];
						J[e][f] = i; // 현재 위치의 dt[][]값을 가지게 해주는 이전 열의 행 번호를 배열 J에 저장
						// 이전 열에서 넘어온 행 번호를 저장하는 J배열 저장
					}
				}
			}
		}
		row =0; r = c = 3;
		d[0] = 0;
		for(i = 0; i <= 3; i++) 
		{ // J배열을 토대로 d배열에 최소 경로로 이동하기 위해 
		// 현재 위치에서 다음 위치로 이동할 행의 번호 저장
			d[i+1]=J[d[i]][i];  // 각 열에서의 가야할 행 J를 배열 d[]에 저장
		}
		// 

		// d배열을 이용한 최대점수&&최소경로 탐색 반복문 -> 함수로 뺄 수 있는 부분
		for(i = 3; i > 0; i--) // 계산한 최고 득점 경로로 로봇을 주행
		{
			if(r > d[i]) 
			// 지금 있는 열이 가야할 열보다 큰 경우 같아질 때까지 
			// 열이 작아지는 방향으로 goUp()함수 실행한 뒤, goLeft()함수를 실행하여 행을 한 칸 이동
			{
				while(r > d[i]) goUp();
				goLeft();
			}
			else if(r == d[i])
			// 지금 있는 열이 가야할 열과 같은 경우 goLeft()함수를 실행하여
			// 행을 한 칸 이동
			{
				goLeft();
			}
			else
			// 지금 있는 열이 가야할 열보다 작은 경우 같아질 때까지
			// 열이 커지는 방향으로 goDown()함수를 실행한 뒤, 
			// goLeft()함수를 실행하여 행을 한 칸 이동
			{
				while(r < d[i]) goDown();
				goLeft();
			}
		}
		playTone(240, 20); sleep(200);
		stopMotor();
		while(getButtonPress(1) == 0) sleep(10);
}
