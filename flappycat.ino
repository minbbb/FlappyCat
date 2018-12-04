/*
	FlappyCat
	author: minbbb
*/

#include <EEPROM.h>
#include <TVout.h>
#include "fontALL.h"
#include "cat.h"
#include "cloud.h"
#include "cloud2.h"
#include "catFly.h"

#define CONTROL_PIN 2	//pin to control
#define X_CLOUD 13	//x coordinate of the first cloud
#define Y_CLOUD 0	//y coordinate of the first cloud
#define X_CLOUD2 96	//x coordinate of the second cloud
#define Y_CLOUD2 79	//y coordinate of the second cloud
#define ACCEL_DOWN 0.1	//fall acceleration
#define ACCEL_UP 0.4	//takeoff acceleration

#define HEIGHT_BLOCK_DOWN 9	//height of the second pipe block
#define WIDTH_BLOCK_DOWN 23	//width of the second pipe block
#define WIDTH_BLOCK_UP 17	//width of the first pipe block
#define HEIGHT_HOLE 40	//pipe spacing in height
#define WIDTH_HOLE 80	//pipe spacing wide
#define COUNT_BLOCK 2	//number of pipes drawn at once

TVout TV;
int xBlock[COUNT_BLOCK];  //x coordinates of the pipes 
int yBlock[COUNT_BLOCK];  //y coordinates of the pipes
int xCat;	//x coordinate of the cat
int yCat;	//y coordinate of the cat
float aCat;	//acceleration of the cat
int score;

void setup() {
	TV.begin(NTSC, 128, 96);
	TV.select_font(font8x8);
	randomSeed(analogRead(0));
	preview();
}

void loop() {
	TV.clear_screen();
	for (int i = 0; i <= COUNT_BLOCK - 1; i++) {
		xBlock[i] = 0;
		yBlock[i] = 0;
	}
	xBlock[0] = 128;
	for (int i = 0; i < COUNT_BLOCK; i++) {
		yBlock[i] = random(2 + HEIGHT_BLOCK_DOWN, 94 - HEIGHT_BLOCK_DOWN - HEIGHT_HOLE);
		if (i >= 1) {
			xBlock[i] = xBlock[i - 1] + WIDTH_HOLE;
		}
	}

	startDemo();
	startGame();
}

void preview() {
	int c = 2;
	for (int i = 0; i <= 24;) {
		TV.bitmap(X_CLOUD, c + Y_CLOUD, cloud);
		TV.bitmap(X_CLOUD2, c + Y_CLOUD2, cloud2);
		for (int ii = 0; ii <= 6; ii++) {
			if (i >= 0)
				TV.bitmap(i, 21, cat);
			else {
				TV.bitmap(i, 19, cat, 79, 56);
			}

			delay(50);
			i += 1;
		}
		if (i > 24)
			break;
		TV.draw_rect(X_CLOUD, c + Y_CLOUD, pgm_read_byte_near(cloud), pgm_read_byte_near(cloud + 1), 0, 0);
		TV.draw_rect(X_CLOUD2, c + Y_CLOUD2, pgm_read_byte_near(cloud2), pgm_read_byte_near(cloud2 + 1), 0, 0);
		c -= 2;

		TV.bitmap(X_CLOUD, c + Y_CLOUD, cloud);
		TV.bitmap(X_CLOUD2, c + Y_CLOUD2, cloud2);
		for (int ii = 0; ii <= 6; ii++) {
			if (i >= 0)
				TV.bitmap(i, 21, cat);
			else {
				TV.bitmap(i, 19, cat, 79, 56);
			}
			delay(50);
			i += 1;
		}
		if (i > 24)
			break;
		TV.draw_rect(X_CLOUD, c + Y_CLOUD, pgm_read_byte_near(cloud), pgm_read_byte_near(cloud + 1), 0, 0);
		TV.draw_rect(X_CLOUD2, c + Y_CLOUD2, pgm_read_byte_near(cloud2), pgm_read_byte_near(cloud2 + 1), 0, 0);
		c += 2;

		TV.bitmap(X_CLOUD, c + Y_CLOUD, cloud);
		TV.bitmap(X_CLOUD2, c + Y_CLOUD2, cloud2);
		for (int ii = 0; ii <= 6; ii++) {
			if (i >= 0)
				TV.bitmap(i, 21, cat);
			else {
				TV.bitmap(i, 19, cat, 79, 56);
			}
			delay(50);
			i += 1;
		}
		if (i > 24)
			break;
		TV.draw_rect(X_CLOUD, c + Y_CLOUD, pgm_read_byte_near(cloud), pgm_read_byte_near(cloud + 1), 0, 0);
		TV.draw_rect(X_CLOUD2, c + Y_CLOUD2, pgm_read_byte_near(cloud2), pgm_read_byte_near(cloud2 + 1), 0, 0);
		c += 2;

		TV.bitmap(X_CLOUD, c + Y_CLOUD, cloud);
		TV.bitmap(X_CLOUD2, c + Y_CLOUD2, cloud2);
		for (int ii = 0; ii <= 6; ii++) {
			if (i >= 0)
				TV.bitmap(i, 21, cat);
			else {
				TV.bitmap(i, 19, cat, 79, 56);
			}
			delay(50);
			i += 1;
		}
		if (i > 24)
			break;
		TV.draw_rect(X_CLOUD, c + Y_CLOUD, pgm_read_byte_near(cloud), pgm_read_byte_near(cloud + 1), 0, 0);
		TV.draw_rect(X_CLOUD2, c + Y_CLOUD2, pgm_read_byte_near(cloud2), pgm_read_byte_near(cloud2 + 1), 0, 0);
		c -= 2;
	}

	TV.print(45, 3, "FlappyCat");
	TV.select_font(font4x6);
	TV.print(7, 85, "Best:");
	String stringVar = String(EEPROM.read(0), DEC);
	char charBufVar[50];
	stringVar.toCharArray(charBufVar, 50);
	TV.print(27, 85, charBufVar);
	TV.select_font(font8x8);
	pinMode(2, INPUT);
	unsigned int music[]	=	 {500, 600, 700, 400, 200, 300, 600, 200, 250, 400};
	unsigned int musicDelay[] = {100, 200, 100, 50 , 150, 100, 150, 100, 100, 400};
	unsigned int previewtime = millis();
	for (unsigned int t = 300, st = 3, pt = 0, mus = 0;;) {
		pt = millis() - previewtime;
		if (pt / t >= 1) {
			mus++;
			if (mus >= sizeof(music) / sizeof(unsigned int)) {
				mus = 0;
			}
			TV.tone(music[mus], musicDelay[mus]);
			previewtime = millis();
			TV.draw_rect(X_CLOUD, c + Y_CLOUD, 18, 13, 0, 0);
			TV.draw_rect(X_CLOUD2, c + Y_CLOUD2, 18, 13, 0, 0);
			st++;
			if (st >= 4)
				st = 0;
			switch (st) {
				case 0: {
						c = 2;
						break;
					}
				case 1: {
						c = 0;
						break;
					}
				case 2: {
						c = 2;
						break;
					}
				case 3: {
						c = 4;
						break;
					}
			}
			TV.bitmap(X_CLOUD, c + Y_CLOUD, cloud);
			TV.bitmap(X_CLOUD2, c + Y_CLOUD2, cloud2);
		}
		if (digitalRead(CONTROL_PIN) == HIGH) {
			return;
		}
	}
}

void startDemo() {
	int checkButton = 0;
	xCat = 25;
	yCat = 29;	//coordinate to which the cat will fall at the start
	aCat = 0;
	score = 0;
	TV.select_font(font4x6);
	TV.print(65, 40, "Press any key");
	TV.print(7, 85, "Best:");
	String stringVar = String(EEPROM.read(0), DEC);
	char charBufVar[50];
	stringVar.toCharArray(charBufVar, 50);
	TV.print(27, 85, charBufVar);
	TV.select_font(font8x8);
	TV.draw_line(0, 0, 128, 0, 1);
	TV.draw_line(0, 95, 128, 95, 1);
	for (;;) {
		flyCat();
		if (yCat >= 47) {//coordinate to which the cat will bounce at the start
			jump();
		}
		if (digitalRead(CONTROL_PIN) == LOW) {
			checkButton = 1;
		}
		if (digitalRead(CONTROL_PIN) == HIGH && checkButton == 1) {
			TV.draw_rect(65, 40, 52, 6, 0, 0);
			return;
		}
	}
}

int flyCat() {
	TV.delay_frame(1);
	TV.draw_rect(xCat, yCat, 20, 14, 0, 0);
	yCat = yCat - aCat;
	if (yCat < 1) {
		yCat = 1;
		aCat = 0;
	}
	//tracking fall to the floor
	if (yCat > 94 - pgm_read_byte_near(catFly + 1)) {
		yCat = 94 - pgm_read_byte_near(catFly + 1);
		TV.bitmap(xCat, yCat, catFly);
		TV.tone(300, 100);
		delay(100);
		TV.tone(200, 100);
		delay(100);
		return 1;
	}
	//pipe collision tracking
	for (int i = 0; i < COUNT_BLOCK; i++) {
		if ((xCat + pgm_read_byte_near(catFly) >= xBlock[i] && xCat <= xBlock[i] + WIDTH_BLOCK_DOWN) && (yCat <= yBlock[i] || yCat + pgm_read_byte_near(catFly + 1) >= yBlock[i] + HEIGHT_HOLE)) {
			TV.bitmap(xCat, yCat, catFly);
			TV.tone(300, 100);
			delay(100);
			TV.tone(200, 100);
			delay(100);
			return 1;
		}
	}
	TV.bitmap(xCat, yCat, catFly);
	aCat = aCat - ACCEL_DOWN;
	return 0;
}

void jump() {
	aCat += ACCEL_UP;
}

void startGame() {
	for (;;) {
		TV.print(4, 4, score);
		moveBlock();
		if (flyCat()) {
			TV.draw_rect(45, 3, 67, 11, 0, 1);
			TV.print(47, 5, "You Lose");
			TV.print(4, 4, score);
			if (EEPROM.read(0) < score) {
				EEPROM.write(0, score);
			}
			delay(300);
			for (;;) {
				if (digitalRead(CONTROL_PIN) == HIGH) {
					//go to start
					return;
				}
			}
		}
		if (digitalRead(CONTROL_PIN) == HIGH) {
			jump();
		}
	}
}

void moveBlock() {
	//calculation of coordinates
	for (int i = 0; i < COUNT_BLOCK; i++) {
		xBlock[i] = xBlock[i] - 1;
		if (xBlock[i] <= -WIDTH_BLOCK_DOWN) {
			switch (i) {
				case 0: {
						xBlock[i] = xBlock[1] + WIDTH_HOLE;
						break;
					}
				case 1: {
						xBlock[i] = xBlock[0] + WIDTH_HOLE;
						break;
					}
			}
			yBlock[i] = random(2 + HEIGHT_BLOCK_DOWN, 94 - HEIGHT_BLOCK_DOWN - HEIGHT_HOLE);

		}
	}

	//drawing
	for (int i = 0; i < COUNT_BLOCK; i++) {
		if (xBlock[i] <= 127 - WIDTH_BLOCK_DOWN && xBlock[i] >= 0) {
			TV.draw_rect(xBlock[i] + ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)), 0, WIDTH_BLOCK_UP, yBlock[i] - HEIGHT_BLOCK_DOWN, 0, 1);
			TV.draw_rect(xBlock[i] + ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)), yBlock[i] + HEIGHT_HOLE + HEIGHT_BLOCK_DOWN, WIDTH_BLOCK_UP, 95 - yBlock[i] - HEIGHT_HOLE - HEIGHT_BLOCK_DOWN, 0, 1);

			TV.draw_rect(xBlock[i], yBlock[i] - HEIGHT_BLOCK_DOWN, WIDTH_BLOCK_DOWN, HEIGHT_BLOCK_DOWN, 0, 1);
			TV.draw_rect(xBlock[i], yBlock[i] + HEIGHT_HOLE, WIDTH_BLOCK_DOWN, HEIGHT_BLOCK_DOWN, 0, 1);
			continue;
		}


		//if the pipe is x >= 128
		if (xBlock[i] <= 127 && xBlock[i] >= 127 - WIDTH_BLOCK_DOWN) {
			if (xBlock[i] <= 127 - ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)) && xBlock[i] >= 127 - (((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)) + WIDTH_BLOCK_UP)) { //
				TV.draw_rect(xBlock[i] + ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)), 0, 127 - xBlock[i] - ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)), yBlock[i] - HEIGHT_BLOCK_DOWN, 0, 1);
				TV.draw_rect(xBlock[i] + ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)), yBlock[i] + HEIGHT_HOLE + HEIGHT_BLOCK_DOWN, 127 - xBlock[i] - ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)), 95 - yBlock[i] - HEIGHT_HOLE - HEIGHT_BLOCK_DOWN, 0, 1);
			} else {
				if (xBlock[i] <= 127 - ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2))) {
					TV.draw_rect(xBlock[i] + ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)), 0, WIDTH_BLOCK_UP, yBlock[i] - HEIGHT_BLOCK_DOWN, 0, 1);
					TV.draw_rect(xBlock[i] + ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)), yBlock[i] + HEIGHT_HOLE + HEIGHT_BLOCK_DOWN, WIDTH_BLOCK_UP, 95 - yBlock[i] - HEIGHT_HOLE - HEIGHT_BLOCK_DOWN, 0, 1);
				}
			}
			TV.draw_rect(xBlock[i], yBlock[i] - HEIGHT_BLOCK_DOWN, 127 - xBlock[i], HEIGHT_BLOCK_DOWN, 0, 1);
			TV.draw_rect(xBlock[i], yBlock[i] + HEIGHT_HOLE, 127 - xBlock[i], HEIGHT_BLOCK_DOWN, 0, 1);
			continue;
		}

		//if the pipe is x <= 0
		if (xBlock[i] >= -WIDTH_BLOCK_DOWN && xBlock[i] <= 0) {
			if (xBlock[i] <= -((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)) && xBlock[i] >= -(((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)) + WIDTH_BLOCK_UP) ) {
				TV.draw_rect(0, 0, WIDTH_BLOCK_UP + (xBlock[i] + ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2))), yBlock[i] - HEIGHT_BLOCK_DOWN, 0, 1);
				TV.draw_rect(0, yBlock[i] + HEIGHT_HOLE + HEIGHT_BLOCK_DOWN, WIDTH_BLOCK_UP + (xBlock[i] + ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2))), 95 - yBlock[i] - HEIGHT_HOLE - HEIGHT_BLOCK_DOWN, 0, 1);
			} else {
				if (xBlock[i] >= -(WIDTH_BLOCK_DOWN - ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)))) {
					TV.draw_rect(xBlock[i] + ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)), 0, WIDTH_BLOCK_UP, yBlock[i] - HEIGHT_BLOCK_DOWN, 0, 1);
					TV.draw_rect(xBlock[i] + ((WIDTH_BLOCK_DOWN / 2) - (WIDTH_BLOCK_UP / 2)), yBlock[i] + HEIGHT_HOLE + HEIGHT_BLOCK_DOWN, WIDTH_BLOCK_UP, 95 - yBlock[i] - HEIGHT_HOLE - HEIGHT_BLOCK_DOWN, 0, 1);
				}
			}
			TV.draw_rect(0, yBlock[i] - HEIGHT_BLOCK_DOWN, WIDTH_BLOCK_DOWN + xBlock[i], HEIGHT_BLOCK_DOWN, 0, 1);
			TV.draw_rect(0, yBlock[i] + HEIGHT_HOLE, WIDTH_BLOCK_DOWN + xBlock[i], HEIGHT_BLOCK_DOWN, 0, 1);
			continue;
		}
	}

	//scoring
	for (int i = 0; i < COUNT_BLOCK; i++) {
		if (xCat + pgm_read_byte_near(catFly) - (WIDTH_BLOCK_DOWN / 2) == xBlock[i]) {
			score++;
			TV.tone(500, 100);
		}
	}


	//drawing the top and bottom lines
	TV.draw_line(0, 0, 128, 0, 1);
	TV.draw_line(0, 95, 128, 95, 1);
}
