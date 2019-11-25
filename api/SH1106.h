#ifndef SH1106_H
#define SH1106_H

#define SH1106_I2C_ADDRESS 0x3C
#define SH1106_I2C_CLOCK_FREQUENCY 400000

#define SH1106_WIDTH 128
#define SH1106_HEIGHT 64

#define SH1106_BUFFER_SIZE 1024
#define SH1106_PAGES 8
#define SH1106_COLUMNS 128

#define SH1106_LOW_COLUMN 0x00
#define SH1106_HIGH_COLUMN 0x10

#define SH1106_PUMP_VOLTAGE 0x30

#define SH1106_START_LINE 0x40

#define SH1106_CONTRAST_CONTROL 0x81
#define SH1106_CONTRAST_DATA 0x80

#define SH1106_SEGMENT_REMAP 0xA1

#define SH1106_ENTIRE_OFF 0xA4
#define SH1106_ENTIRE_ON 0xA5

#define SH1106_NORMAL 0xA6
#define SH1106_REVERSE 0xA7

#define SH1106_MULTIPLEX_MODE 0xA8
#define SH1106_MULTIPLEX_DATA 0x3F

#define SH1106_DC_DC_CONTROL 0xAD
#define SH1106_DC_DC_DATA 0x8B

#define SH1106_DISPLAY_ON 0xAF
#define SH1106_DISPLAY_OFF 0xAE

#define SH1106_PAGE 0xB0

#define SH1106_SCAN_DIRECTION 0xC8

#define SH1106_OFFSET_MODE 0xD3
#define SH1106_OFFSET_DATA 0x00

#define SH1106_FREQUENCY_MODE 0xD5
#define SH1106_FREQUENCY_DATA 0x80

#define SH1106_DIS_PRE_CHARGE_MODE 0xD9
#define SH1106_DIS_PRE_CHARGE_DATA 0x22

#define SH1106_COMMON_PADS_MODE 0xDA
#define SH1106_COMMON_PADS_DATA 0x12

#define SH1106_VCOM_MODE 0xDB
#define SH1106_VCOM_DATA 0x35

extern const PROGMEM unsigned char font5x8[][5];
extern const PROGMEM unsigned char font5x8Width[];

class SH1106
{
private:
	unsigned char buffer[SH1106_BUFFER_SIZE];
	inline void writeCommand(unsigned char data);

public:
	void begin();
	void clear();
	void update();
	void setPixel(int x, int y);
	void clearPixel(int x, int y);
	void drawVerticalLine(int x, int y, int y2);
	void drawHorizontalLine(int x, int y, int x2);
	void drawRectangle(int x, int y, unsigned int width, unsigned int height);
	void drawFilledRectangle(int x, int y, unsigned int width, unsigned int height);
	void drawString(const char *str, unsigned char spacing, int xPos, int yPos);
	void drawString(const char *str, unsigned char spacing, unsigned char outlined, int xPos, int yPos);
	void drawChar(char value, int x, int y);
	void drawInt(int value, unsigned char spacing, int x, int y);
	unsigned int getCharWidth(char value);
	unsigned int getStringWidth(const char *str, unsigned char spacing);
	unsigned int getIntWidth(int value, unsigned char spacing);
};

#endif