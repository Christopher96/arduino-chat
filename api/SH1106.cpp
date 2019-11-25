#include <Wire.h>
#include "SH1106.h"

inline void SH1106::writeCommand(unsigned char data)
{
	Wire.beginTransmission(SH1106_I2C_ADDRESS);
	Wire.write(0x00); // 0x00 indicates that the following data byte is for a command operation.
	Wire.write(data);
	Wire.endTransmission();
}

void SH1106::begin()
{
	Wire.begin();
	Wire.setClock(SH1106_I2C_CLOCK_FREQUENCY); // Setting I2C clock frequency to 400 KHz.

	writeCommand(SH1106_DISPLAY_OFF);

	writeCommand(SH1106_LOW_COLUMN);
	writeCommand(SH1106_HIGH_COLUMN);
	writeCommand(SH1106_PUMP_VOLTAGE);
	writeCommand(SH1106_START_LINE);
	writeCommand(SH1106_CONTRAST_CONTROL);
	writeCommand(SH1106_CONTRAST_DATA);
	writeCommand(SH1106_SEGMENT_REMAP);
	writeCommand(SH1106_ENTIRE_OFF);
	writeCommand(SH1106_NORMAL);
	writeCommand(SH1106_MULTIPLEX_MODE);
	writeCommand(SH1106_MULTIPLEX_DATA);
	writeCommand(SH1106_DC_DC_CONTROL);
	writeCommand(SH1106_DC_DC_DATA);
	writeCommand(SH1106_SCAN_DIRECTION);
	writeCommand(SH1106_OFFSET_MODE);
	writeCommand(SH1106_OFFSET_DATA);
	writeCommand(SH1106_FREQUENCY_MODE);
	writeCommand(SH1106_FREQUENCY_DATA);
	writeCommand(SH1106_DIS_PRE_CHARGE_MODE);
	writeCommand(SH1106_DIS_PRE_CHARGE_DATA);
	writeCommand(SH1106_COMMON_PADS_MODE);
	writeCommand(SH1106_COMMON_PADS_DATA);
	writeCommand(SH1106_VCOM_MODE);
	writeCommand(SH1106_VCOM_DATA);

	writeCommand(SH1106_DISPLAY_ON);
}

void SH1106::clear()
{
	for (unsigned short byte = 0; byte < SH1106_BUFFER_SIZE; byte++)
	{
		buffer[byte] = 0;
	}
}

void SH1106::update()
{
	const unsigned char *buff = this->buffer;

	for (unsigned char page = 0; page < SH1106_PAGES; page++)
	{

		writeCommand(SH1106_PAGE | page);

		for (unsigned char segment = 0; segment < 8; segment++)
		{
			unsigned char columnAddress = (segment << 4) + 2;

			writeCommand(columnAddress & 0x0F);
			writeCommand(SH1106_HIGH_COLUMN | (columnAddress >> 4));

			Wire.beginTransmission(SH1106_I2C_ADDRESS);
			Wire.write(0x40);

			for (unsigned char column = 0; column < 16; column++)
			{
				Wire.write(*buff++);
			}

			Wire.endTransmission();
		}
	}
}

void SH1106::setPixel(int x, int y)
{
	if (x < 0 || x >= SH1106_WIDTH || y < 0 || y >= SH1106_HEIGHT)
		return;
	unsigned char page = y / 8;
	unsigned short byte = x + page * SH1106_WIDTH;
	unsigned char bit = 1 << (y % 8);
	buffer[byte] |= bit;
}

void SH1106::clearPixel(int x, int y)
{
	if (x < 0 || x >= SH1106_WIDTH || y < 0 || y >= SH1106_HEIGHT)
		return;
	unsigned char page = y / 8;
	unsigned short byte = x + page * SH1106_WIDTH;
	unsigned char bit = 1 << (y % 8);
	buffer[byte] &= ~bit;
}

void SH1106::drawVerticalLine(int x, int y, int y2)
{
	int minY = y < y2 ? y : y2;
	int maxY = y > y2 ? y : y2;

	while (minY <= maxY)
	{
		setPixel(x, minY++);
	}
}

void SH1106::drawHorizontalLine(int x, int y, int x2)
{
	int minX = x < x2 ? x : x2;
	int maxX = x > x2 ? x : x2;

	while (minX <= maxX)
	{
		setPixel(minX++, y);
	}
}

void SH1106::drawRectangle(int x, int y, unsigned int width, unsigned int height)
{
	for (int xCopy = x; xCopy < x + width; xCopy++)
	{
		setPixel(xCopy, y);
		setPixel(xCopy, y + height - 1);
	}
	for (int yCopy = y; yCopy < y + height; yCopy++)
	{
		setPixel(x, yCopy);
		setPixel(x + width - 1, yCopy);
	}
}

void SH1106::drawFilledRectangle(int x, int y, unsigned int width, unsigned int height)
{
	for (int yCopy = y; yCopy < y + height; yCopy++)
	{
		for (int xCopy = x; xCopy < x + width; xCopy++)
		{
			setPixel(xCopy, yCopy);
		}
	}
}

const PROGMEM unsigned char font5x8[][5] = {
	{0x00, 0x00, 0x00, 0x00, 0x00}, // 20
	{0x5f, 0x00, 0x00, 0x00, 0x00}, // 21 !
	{0x07, 0x00, 0x07, 0x00, 0x00}, // 22 "
	{0x14, 0x7f, 0x14, 0x7f, 0x14}, // 23 #
	{0x24, 0x2a, 0x7f, 0x2a, 0x12}, // 24 $
	{0x23, 0x13, 0x08, 0x64, 0x62}, // 25 %
	{0x36, 0x49, 0x55, 0x22, 0x50}, // 26 &
	{0x05, 0x03, 0x00, 0x00, 0x00}, // 27 '
	{0x1c, 0x22, 0x41, 0x00, 0x00}, // 28 (
	{0x41, 0x22, 0x1c, 0x00, 0x00}, // 29 )
	{0x14, 0x08, 0x3e, 0x08, 0x14}, // 2a *
	{0x08, 0x08, 0x3e, 0x08, 0x08}, // 2b +
	{0x50, 0x30, 0x00, 0x00, 0x00}, // 2c ,
	{0x08, 0x08, 0x08, 0x08, 0x08}, // 2d -
	{0x60, 0x60, 0x00, 0x00, 0x00}, // 2e .
	{0x20, 0x10, 0x08, 0x04, 0x02}, // 2f /
	{0x3e, 0x51, 0x49, 0x45, 0x3e}, // 30 0
	{0x42, 0x7f, 0x40, 0x00, 0x00}, // 31 1
	{0x42, 0x61, 0x51, 0x49, 0x46}, // 32 2
	{0x21, 0x41, 0x45, 0x4b, 0x31}, // 33 3
	{0x18, 0x14, 0x12, 0x7f, 0x10}, // 34 4
	{0x27, 0x45, 0x45, 0x45, 0x39}, // 35 5
	{0x3c, 0x4a, 0x49, 0x49, 0x30}, // 36 6
	{0x01, 0x71, 0x09, 0x05, 0x03}, // 37 7
	{0x36, 0x49, 0x49, 0x49, 0x36}, // 38 8
	{0x06, 0x49, 0x49, 0x29, 0x1e}, // 39 9
	{0x36, 0x36, 0x00, 0x00, 0x00}, // 3a :
	{0x56, 0x36, 0x00, 0x00, 0x00}, // 3b ;
	{0x08, 0x14, 0x22, 0x41, 0x00}, // 3c <
	{0x14, 0x14, 0x14, 0x14, 0x14}, // 3d =
	{0x41, 0x22, 0x14, 0x08, 0x00}, // 3e >
	{0x02, 0x01, 0x51, 0x09, 0x06}, // 3f ?
	{0x32, 0x49, 0x79, 0x41, 0x3e}, // 40 @
	{0x7e, 0x11, 0x11, 0x11, 0x7e}, // 41 A
	{0x7f, 0x49, 0x49, 0x49, 0x36}, // 42 B
	{0x3e, 0x41, 0x41, 0x41, 0x22}, // 43 C
	{0x7f, 0x41, 0x41, 0x22, 0x1c}, // 44 D
	{0x7f, 0x49, 0x49, 0x49, 0x41}, // 45 E
	{0x7f, 0x09, 0x09, 0x09, 0x01}, // 46 F
	{0x3e, 0x41, 0x49, 0x49, 0x7a}, // 47 G
	{0x7f, 0x08, 0x08, 0x08, 0x7f}, // 48 H
	{0x41, 0x7f, 0x41, 0x00, 0x00}, // 49 I
	{0x20, 0x40, 0x41, 0x3f, 0x01}, // 4a J
	{0x7f, 0x08, 0x14, 0x22, 0x41}, // 4b K
	{0x7f, 0x40, 0x40, 0x40, 0x40}, // 4c L
	{0x7f, 0x02, 0x0c, 0x02, 0x7f}, // 4d M
	{0x7f, 0x04, 0x08, 0x10, 0x7f}, // 4e N
	{0x3e, 0x41, 0x41, 0x41, 0x3e}, // 4f O
	{0x7f, 0x09, 0x09, 0x09, 0x06}, // 50 P
	{0x3e, 0x41, 0x51, 0x21, 0x5e}, // 51 Q
	{0x7f, 0x09, 0x19, 0x29, 0x46}, // 52 R
	{0x46, 0x49, 0x49, 0x49, 0x31}, // 53 S
	{0x01, 0x01, 0x7f, 0x01, 0x01}, // 54 T
	{0x3f, 0x40, 0x40, 0x40, 0x3f}, // 55 U
	{0x1f, 0x20, 0x40, 0x20, 0x1f}, // 56 V
	{0x3f, 0x40, 0x38, 0x40, 0x3f}, // 57 W
	{0x63, 0x14, 0x08, 0x14, 0x63}, // 58 X
	{0x07, 0x08, 0x70, 0x08, 0x07}, // 59 Y
	{0x61, 0x51, 0x49, 0x45, 0x43}, // 5a Z
	{0x7f, 0x41, 0x41, 0x00, 0x00}, // 5b [
	{0x02, 0x04, 0x08, 0x10, 0x20}, // 5c backslash
	{0x41, 0x41, 0x7f, 0x00, 0x00}, // 5d ]
	{0x04, 0x02, 0x01, 0x02, 0x04}, // 5e ^
	{0x40, 0x40, 0x40, 0x40, 0x40}, // 5f _
	{0x01, 0x02, 0x04, 0x00, 0x00}, // 60 `
	{0x20, 0x54, 0x54, 0x54, 0x78}, // 61 a
	{0x7f, 0x48, 0x44, 0x44, 0x38}, // 62 b
	{0x38, 0x44, 0x44, 0x44, 0x20}, // 63 c
	{0x38, 0x44, 0x44, 0x48, 0x7f}, // 64 d
	{0x38, 0x54, 0x54, 0x54, 0x18}, // 65 e
	{0x08, 0x7e, 0x09, 0x01, 0x02}, // 66 f
	{0x0c, 0x52, 0x52, 0x52, 0x3e}, // 67 g
	{0x7f, 0x08, 0x04, 0x04, 0x78}, // 68 h
	{0x44, 0x7d, 0x40, 0x00, 0x00}, // 69 i
	{0x20, 0x40, 0x44, 0x3d, 0x00}, // 6a j
	{0x7f, 0x10, 0x28, 0x44, 0x00}, // 6b k
	{0x41, 0x7f, 0x40, 0x00, 0x00}, // 6c l
	{0x7c, 0x04, 0x18, 0x04, 0x78}, // 6d m
	{0x7c, 0x08, 0x04, 0x04, 0x78}, // 6e n
	{0x38, 0x44, 0x44, 0x44, 0x38}, // 6f o
	{0x7c, 0x14, 0x14, 0x14, 0x08}, // 70 p
	{0x08, 0x14, 0x14, 0x18, 0x7c}, // 71 q
	{0x7c, 0x08, 0x04, 0x04, 0x08}, // 72 r
	{0x48, 0x54, 0x54, 0x54, 0x20}, // 73 s
	{0x04, 0x3f, 0x44, 0x40, 0x20}, // 74 t
	{0x3c, 0x40, 0x40, 0x20, 0x7c}, // 75 u
	{0x1c, 0x20, 0x40, 0x20, 0x1c}, // 76 v
	{0x3c, 0x40, 0x30, 0x40, 0x3c}, // 77 w
	{0x44, 0x28, 0x10, 0x28, 0x44}, // 78 x
	{0x0c, 0x50, 0x50, 0x50, 0x3c}, // 79 y
	{0x44, 0x64, 0x54, 0x4c, 0x44}, // 7a z
	{0x08, 0x36, 0x41, 0x00, 0x00}, // 7b {
	{0x7f, 0x00, 0x00, 0x00, 0x00}, // 7c |
	{0x41, 0x36, 0x08, 0x00, 0x00}, // 7d }
	{0x10, 0x08, 0x08, 0x10, 0x08}, // 7e ~
};

const PROGMEM unsigned char font5x8Width[] = {
	3, // 20
	1, // 21 !
	3, // 22 "
	5, // 23 #
	5, // 24 $
	5, // 25 %
	5, // 26 &
	2, // 27 '
	3, // 28 (
	3, // 29 )
	5, // 2a *
	5, // 2b +
	2, // 2c ,
	5, // 2d -
	2, // 2e .
	5, // 2f /
	5, // 30 0
	3, // 31 1
	5, // 32 2
	5, // 33 3
	5, // 34 4
	5, // 35 5
	5, // 36 6
	5, // 37 7
	5, // 38 8
	5, // 39 9
	2, // 3a :
	2, // 3b ;
	4, // 3c <
	5, // 3d =
	4, // 3e >
	5, // 3f ?
	5, // 40 @
	5, // 41 A
	5, // 42 B
	5, // 43 C
	5, // 44 D
	5, // 45 E
	5, // 46 F
	5, // 47 G
	5, // 48 H
	3, // 49 I
	5, // 4a J
	5, // 4b K
	5, // 4c L
	5, // 4d M
	5, // 4e N
	5, // 4f O
	5, // 50 P
	5, // 51 Q
	5, // 52 R
	5, // 53 S
	5, // 54 T
	5, // 55 U
	5, // 56 V
	5, // 57 W
	5, // 58 X
	5, // 59 Y
	5, // 5a Z
	3, // 5b [
	5, // 5c backslash
	3, // 5d ]
	5, // 5e ^
	5, // 5f _
	3, // 60 `
	5, // 61 a
	5, // 62 b
	5, // 63 c
	5, // 64 d
	5, // 65 e
	5, // 66 f
	5, // 67 g
	5, // 68 h
	3, // 69 i
	4, // 6a j
	4, // 6b k
	3, // 6c l
	5, // 6d m
	5, // 6e n
	5, // 6f o
	5, // 70 p
	5, // 71 q
	5, // 72 r
	5, // 73 s
	5, // 74 t
	5, // 75 u
	5, // 76 v
	5, // 77 w
	5, // 78 x
	5, // 79 y
	5, // 7a z
	3, // 7b {
	1, // 7c |
	3, // 7d }
	5, // 7e ~
};

void SH1106::drawString(const char *str, unsigned char spacing, int xPos, int yPos)
{
	while (*str != '\0')
	{
		for (int x = 0; x < 5; x++)
		{
			unsigned char column = pgm_read_byte(&font5x8[*str - 0x20][x]);
			for (int y = 0; y < 8; y++)
			{
				if (column & (1 << y))
				{
					setPixel(x + xPos, y + yPos);
				}
			}
		}
		unsigned char width = pgm_read_byte(&font5x8Width[*str - 0x20]);
		xPos += width + spacing;
		str++;
	}
}

void SH1106::drawString(const char *str, unsigned char spacing, unsigned char outlined, int xPos, int yPos)
{
	int count = 0;

	while (*str != '\0')
	{
		if(count == outlined){
			drawRectangle(xPos - 3, yPos - 3, 6 + getCharWidth(*str), 13);
		}

		for (int x = 0; x < 5; x++)
		{
			unsigned char column = pgm_read_byte(&font5x8[*str - 0x20][x]);
			for (int y = 0; y < 8; y++)
			{
				if (column & (1 << y))
				{
					setPixel(x + xPos, y + yPos);
				}
			}
		}
		unsigned char width = pgm_read_byte(&font5x8Width[*str - 0x20]);
		xPos += width + spacing;
		str++;
		count++;
	}
}


void SH1106::drawChar(char value, int x, int y)
{
	const char str[] = {value, '\0'};
	drawString(str, 0, x, y);
}

void SH1106::drawInt(int value, unsigned char spacing, int x, int y)
{
	char buffer[11];
	itoa(value, buffer, 10);
	drawString(buffer, spacing, x, y);
}

unsigned int SH1106::getCharWidth(char value)
{
	return pgm_read_byte(&font5x8Width[value - 0x20]);
}

unsigned int SH1106::getStringWidth(const char *str, unsigned char spacing)
{
	unsigned int totalWidth = 0;
	while (*str != '\0')
	{
		totalWidth += getCharWidth(*str) + spacing;
		str++;
	}
	totalWidth -= spacing;
	return totalWidth;
}

unsigned int SH1106::getIntWidth(int value, unsigned char spacing)
{
	char buffer[11];
	itoa(value, buffer, 10);
	return getStringWidth(buffer, spacing);
}
