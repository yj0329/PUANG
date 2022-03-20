#include <bangtal.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

SceneID scene1, scene2, scene3;
ObjectID startbutton, endbutton, homebutton, restartbutton, blank, screen, item;
ObjectID p[4][4];
int lx[4][4] = { 1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4 };
int ly[4][4] = { 1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4 };
TimerID timer1;
int bx = 4, by = 4;
int a = 0;
time_t start;
time_t end;
int count = 0, countItem = 0;
bool useItem = false;

void clear();
void check();
int locationX(int i);
int locationY(int i);
void movePiece(int i, int j);
void setGame();

void clear() {
	end = time(NULL);

	int timetime = difftime(end, start);

	enterScene(scene3);

	showMessage("걸린 시간, 이동 횟수, 아이템 사용 횟수를 콘솔 창에서 확인하세요");
	printf("\nTime : %dsec\nMoves : %d\nItem : %d\n", timetime, count, countItem);
}

void check() {
	if (lx[0][0] == 1 && lx[0][1] == 1 && lx[0][2] == 1 && lx[0][3] == 1
		&& lx[1][0] == 2 && lx[1][1] == 2 && lx[1][2] == 2 && lx[1][3] == 2
		&& lx[2][0] == 3 && lx[2][1] == 3 && lx[2][2] == 3 && lx[2][3] == 3
		&& lx[3][0] == 4 && lx[3][1] == 4 && lx[3][2] == 4

		&& ly[0][0] == 1 && ly[0][1] == 2 && ly[0][2] == 3 && ly[0][3] == 4
		&& ly[1][0] == 1 && ly[1][1] == 2 && ly[1][2] == 3 && ly[1][3] == 4
		&& ly[2][0] == 1 && ly[2][1] == 2 && ly[2][2] == 3 && ly[2][3] == 4
		&& ly[3][0] == 1 && ly[3][1] == 2 && ly[3][2] == 3
		) {
		clear();
	}
}

int locationX(int i) {
	switch (i) {
	case 1:
		return 100;
		break;
	case 2:
		return 260;
		break;
	case 3:
		return 420;
		break;
	default:
		return 580;
	}
}

int locationY(int i) {
	switch (i) {
	case 1:
		return 505;
		break;
	case 2:
		return 345;
		break;
	case 3:
		return 185;
		break;
	default:
		return 25;
	}
}

void movePiece(int i, int j) {
	if ((lx[i][j] + 1 == bx) && (ly[i][j] == by)) {
		lx[i][j] ++;
		bx--;
	}
	else if ((lx[i][j] - 1 == bx) && (ly[i][j] == by)) {
		lx[i][j]--;
		bx++;
	}
	else if ((lx[i][j] == bx) && (ly[i][j] + 1 == by)) {
		ly[i][j]++;
		by--;
	}
	else if ((lx[i][j] == bx) && (ly[i][j] - 1 == by)) {
		ly[i][j]--;
		by++;
	}
	locateObject(p[i][j], scene2, locationX(lx[i][j]), locationY(ly[i][j]));
	showObject(p[i][j]);
}

void setGame() {

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			lx[i][j] = i + 1;
			ly[i][j] = j + 1;
			locateObject(p[i][j], scene2, locationX(lx[i][j]), locationY(ly[i][j]));
			showObject(p[i][j]);
		}
	}

	for (int i = 0; i < 3; i++) {
		lx[i][3] = i + 1;
		ly[i][3] = 4;
		locateObject(p[i][3], scene2, locationX(lx[i][3]), locationY(ly[i][3]));
		showObject(p[i][3]);

		lx[3][i] = 4;
		ly[3][i] = i + 1;
		locateObject(p[3][i], scene2, locationX(lx[3][i]), locationY(ly[3][i]));
		showObject(p[3][i]);
	}

	useItem = false;
	count = 0;
	countItem = 0;
	bx = 4;
	by = 4;

	setTimer(timer1, 0.5f);
	startTimer(timer1);

	start = time(NULL);
}

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) {
		showObject(object);
	}
	return object;
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == startbutton) {
		enterScene(scene2);
		setGame();
	}
	else if (object == endbutton) {
		endGame();
	}
	else if (object == homebutton) {
		enterScene(scene1);
	}
	else if (object == restartbutton) {
		setGame();
	}
	else if (object == screen) {
		enterScene(scene1);
	}
	else if (object == item) {
		showMessage("선택한 퍼즐이 빈칸으로 이동합니다");
		useItem = true;
	}

	if (useItem) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (object == p[i][j]) {
					int num1 = lx[i][j];
					int num2 = ly[i][j];

					lx[i][j] = bx;
					ly[i][j] = by;

					locateObject(p[i][j], scene2, locationX(lx[i][j]), locationY(ly[i][j]));
					showObject(p[i][j]);
					showMessage("아이템 사용 완료");
					bx = num1;
					by = num2;
					countItem++;

					check();

					useItem = false;

				}
			}
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (object == p[i][j]) {
					if ((lx[i][j] == bx && ly[i][j] - 1 == by) || (lx[i][j] == bx && ly[i][j] + 1 == by)
						|| (lx[i][j] + 1 == bx && ly[i][j] == by) || (lx[i][j] - 1 == bx && ly[i][j] == by)) {
						count++;
					}
					movePiece(i, j);


					check();
				}
			}
		}
	}
}

void timerCallback(TimerID timer) {
	if (timer == timer1) {

		srand((unsigned int)time(NULL));


		for (int i = 0; i < 500; i++) {

			int a = rand() % 4;
			int b = rand() % 4;

			if (a != 3 || b != 3) {
				movePiece(a, b);
			}
		}
	}

}

int main() {

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	scene1 = createScene("PUANG", "Images/배경-1.png");
	scene2 = createScene("PUANG", "Images/배경-2.png");
	scene3 = createScene("PUANG", "Images/배경-3.png");

	startbutton = createObject("Images/start.png", scene1, 870, 270, true);
	endbutton = createObject("Images/end.png", scene1, 870, 190, true);
	homebutton = createObject("Images/home.png", scene2, 830, 100, true);
	restartbutton = createObject("Images/restart.png", scene2, 950, 110, true);
	screen = createObject("Images/배경-3.png", scene3, 0, 0, true);
	item = createObject("Images/item.png", scene2, 1040, 115, true);

	p[0][0] = createObject("Images/1.png", scene2, 100, 505, true);
	p[1][0] = createObject("Images/2.png", scene2, 260, 505, true);
	p[2][0] = createObject("Images/3.png", scene2, 420, 505, true);
	p[3][0] = createObject("Images/4.png", scene2, 580, 505, true);
	p[0][1] = createObject("Images/5.png", scene2, 100, 345, true);
	p[1][1] = createObject("Images/6.png", scene2, 260, 345, true);
	p[2][1] = createObject("Images/7.png", scene2, 420, 345, true);
	p[3][1] = createObject("Images/8.png", scene2, 580, 345, true);
	p[0][2] = createObject("Images/9.png", scene2, 100, 185, true);
	p[1][2] = createObject("Images/10.png", scene2, 260, 185, true);
	p[2][2] = createObject("Images/11.png", scene2, 420, 185, true);
	p[3][2] = createObject("Images/12.png", scene2, 580, 185, true);
	p[0][3] = createObject("Images/13.png", scene2, 100, 25, true);
	p[1][3] = createObject("Images/14.png", scene2, 260, 25, true);
	p[2][3] = createObject("Images/15.png", scene2, 420, 25, true);

	scaleObject(homebutton, 0.5f);
	scaleObject(restartbutton, 0.4f);
	scaleObject(item, 0.2f);

	timer1 = createTimer(1.0f);

	startGame(scene1);
}