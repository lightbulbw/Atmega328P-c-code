/*************
*D-FLIFE 1.28 Inch TFT LCD Display Module Round RGB 240 x 240 GC9A01 Driver 4 Wire SPI Interface 240x240 PCB for Arduino

*vcc= 3.3v
*#define TFT_CS     10
*#define TFT_RST    9
*#define TFT_DC     8
*#define TFT_BL     6
*#define TFT_MOSI  D11
*#define TFT_SCK   D13
***********/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define WIDTH   240
#define HEIGHT  240
#define X_OFFSET  0
#define Y_OFFSET  80

//pin macros
#define CS_LOW()    PORTB &= ~(1 << PB2)
#define CS_HIGH()   PORTB |=  (1 << PB2)
#define DC_LOW()    PORTB &= ~(1 << PB0)
#define DC_HIGH()   PORTB |=  (1 << PB0)
#define RST_LOW()   PORTB &= ~(1 << PB1)
#define RST_HIGH()  PORTB |=  (1 << PB1)

void spi_init(void) {
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2) | (1 << PB1) | (1 << PB0);
    DDRD |= (1 << PD6);
    PORTD |= (1 << PD6); // Backlight ON
    
    //SPI:Enable, Master, MODE 3 (CPOL=1, CPHA=1), F_CPU / 2 = 8 MHz
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPOL) | (1 << CPHA);
    //SPCR = (1 << SPE) | (1 << MSTR);   // MODE 0
    SPSR = (1 << SPI2X);


    CS_HIGH();
}

void spi_write(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
}

void tft_cmd(uint8_t c) {
    DC_LOW();
    CS_LOW();
    spi_write(c);
    CS_HIGH();
}

void tft_data(uint8_t d) {
    DC_HIGH();
    CS_LOW();
    spi_write(d);
    CS_HIGH();
}

void gc9a01_init(void) {
    RST_HIGH(); _delay_ms(10);
    RST_LOW();  _delay_ms(20);
    RST_HIGH(); _delay_ms(150);

    tft_cmd(0x01); // SWRESET
    _delay_ms(150);

    tft_cmd(0xEF);
    tft_cmd(0xEB); tft_data(0x14);
    tft_cmd(0xFE);
    tft_cmd(0xEF);

    tft_cmd(0xEB); tft_data(0x14);
    tft_cmd(0x84); tft_data(0x40);
    tft_cmd(0x85); tft_data(0xFF);
    tft_cmd(0x86); tft_data(0xFF);
    tft_cmd(0x87); tft_data(0xFF);
    tft_cmd(0x88); tft_data(0x0A);
    tft_cmd(0x89); tft_data(0x21);
    tft_cmd(0x8A); tft_data(0x00);
    tft_cmd(0x8B); tft_data(0x80);
    tft_cmd(0x8C); tft_data(0x01);
    tft_cmd(0x8D); tft_data(0x01);
    tft_cmd(0x8E); tft_data(0xFF);
    tft_cmd(0x8F); tft_data(0xFF);

    tft_cmd(0xB6); tft_data(0x00); tft_data(0x00);

    tft_cmd(0x36); tft_data(0x48); // MADCTL MX, RGB usually 48
    tft_cmd(0x3A); tft_data(0x05); // 16-bit

    tft_cmd(0x90); 
    tft_data(0x08); tft_data(0x08);
    tft_data(0x08); tft_data(0x08);

    tft_cmd(0xBD); tft_data(0x06);
    tft_cmd(0xBC); tft_data(0x00);

    tft_cmd(0xFF);
    tft_data(0x60); tft_data(0x01); tft_data(0x04);

    tft_cmd(0xC3); tft_data(0x13);
    tft_cmd(0xC4); tft_data(0x13);
    tft_cmd(0xC9); tft_data(0x22);

    tft_cmd(0xBE); tft_data(0x11);
    tft_cmd(0xE1); tft_data(0x10); tft_data(0x0E);
    tft_cmd(0xDF); tft_data(0x21); tft_data(0x0C); tft_data(0x02);

    // === COLUMN DRIVE / PORCH / INVERSION (CRITICAL) ===
    tft_cmd(0xED); tft_data(0x1B); tft_data(0x0B);
    tft_cmd(0xAE); tft_data(0x77);
    tft_cmd(0xCD); tft_data(0x63);

    tft_cmd(0xE8); tft_data(0x34); // Frame rate

    tft_cmd(0x62);
    tft_data(0x18); tft_data(0x0D); tft_data(0x71);
    tft_data(0xED); tft_data(0x70); tft_data(0x70);
    tft_data(0x18); tft_data(0x0F); tft_data(0x71);
    tft_data(0xEF); tft_data(0x70); tft_data(0x70);

    tft_cmd(0x63);
    tft_data(0x18); tft_data(0x11); tft_data(0x71);
    tft_data(0xF1); tft_data(0x70); tft_data(0x70);
    tft_data(0x18); tft_data(0x13); tft_data(0x71);
    tft_data(0xF3); tft_data(0x70); tft_data(0x70);

    tft_cmd(0x64);
    tft_data(0x28); tft_data(0x29); tft_data(0xF1);
    tft_data(0x01); tft_data(0xF1); tft_data(0x00);
    tft_data(0x07);

    tft_cmd(0x66);
    tft_data(0x3C); tft_data(0x00); tft_data(0xCD);
    tft_data(0x67); tft_data(0x45); tft_data(0x45);
    tft_data(0x10); tft_data(0x00); tft_data(0x00);
    tft_data(0x00);

    tft_cmd(0x67);
    tft_data(0x00); tft_data(0x3C); tft_data(0x00);
    tft_data(0x00); tft_data(0x00); tft_data(0x01);
    tft_data(0x54); tft_data(0x10); tft_data(0x32);
    tft_data(0x98);

    tft_cmd(0x74);
    tft_data(0x10); tft_data(0x85); tft_data(0x80);
    tft_data(0x00); tft_data(0x00); tft_data(0x4E);
    tft_data(0x00);

    tft_cmd(0x98); tft_data(0x3E); tft_data(0x07);

    // === GAMMA ===
    tft_cmd(0xF0);
    tft_data(0x45); tft_data(0x09); tft_data(0x08);
    tft_data(0x08); tft_data(0x26); tft_data(0x2A);

    tft_cmd(0xF1);
    tft_data(0x43); tft_data(0x70); tft_data(0x72);
    tft_data(0x36); tft_data(0x37); tft_data(0x6F);

    tft_cmd(0xF2);
    tft_data(0x45); tft_data(0x09); tft_data(0x08);
    tft_data(0x08); tft_data(0x26); tft_data(0x2A);

    tft_cmd(0xF3);
    tft_data(0x43); tft_data(0x70); tft_data(0x72);
    tft_data(0x36); tft_data(0x37); tft_data(0x6F);

    tft_cmd(0x21); // INVON

    tft_cmd(0x11); // SLPOUT
    _delay_ms(120);

    tft_cmd(0x29); // DISPON
    _delay_ms(20);
}

void tft_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    CS_LOW();

    DC_LOW(); spi_write(0x2A);  // CASET
    DC_HIGH();
    spi_write((x0 + X_OFFSET) >> 8);
    spi_write((x0 + X_OFFSET) & 0xFF);
    spi_write((x1 + X_OFFSET) >> 8);
    spi_write((x1 + X_OFFSET) & 0xFF);

    DC_LOW(); spi_write(0x2B);  // RASET
    DC_HIGH();
    spi_write((y0 + Y_OFFSET) >> 8);
    spi_write((y0 + Y_OFFSET) & 0xFF);
    spi_write((y1 + Y_OFFSET) >> 8);
    spi_write((y1 + Y_OFFSET) & 0xFF);

    DC_LOW(); spi_write(0x2C);  // RAMWR
    DC_HIGH();
}


void fillScreen(uint16_t color) {
    // Set full window ONCE
    CS_LOW();
    DC_LOW();
    spi_write(0x2A);              // CASET
    DC_HIGH();
    spi_write(0x00); spi_write(0x00);
    spi_write(0x00); spi_write(WIDTH - 1);

    DC_LOW();
    spi_write(0x2B);              // RASET
    DC_HIGH();
    spi_write(0x00); spi_write(0x00);
    spi_write(0x00); spi_write(HEIGHT - 1);

    DC_LOW();
    spi_write(0x2C);              // RAMWR
    DC_HIGH();

    //continuous pixel stream (NO CS TOGGLING)
    for (uint32_t i = 0; i < (uint32_t)WIDTH * HEIGHT; i++) {
        SPDR = color >> 8;
        while (!(SPSR & (1 << SPIF)));
        SPDR = color & 0xFF;
        while (!(SPSR & (1 << SPIF)));
    }

    CS_HIGH();
}
//=========draw ASCII characters=======

#define CHAR_WIDTH 5
#define CHAR_HEIGHT 7

//only uppercase letters A-Z
const uint8_t font[36][5] = {
  {0x7E,0x11,0x11,0x11,0x7E}, // A
  {0x7F,0x49,0x49,0x49,0x36}, // B
  {0x3E,0x41,0x41,0x41,0x22}, // C
  {0x7F,0x41,0x41,0x22,0x1C}, // D
  {0x7F,0x49,0x49,0x49,0x41}, // E
  {0x7F,0x09,0x09,0x09,0x01}, // F
  {0x3E,0x41,0x49,0x49,0x7A}, // G
  {0x7F,0x08,0x08,0x08,0x7F}, // H
  {0x00,0x41,0x7F,0x41,0x00}, // I
  {0x20,0x40,0x41,0x3F,0x01}, // J
  {0x7F,0x08,0x14,0x22,0x41}, // K
  {0x7F,0x40,0x40,0x40,0x40}, // L
  {0x7F,0x02,0x04,0x02,0x7F}, // M
  {0x7F,0x04,0x08,0x10,0x7F}, // N
  {0x3E,0x41,0x41,0x41,0x3E}, // O
  {0x7F,0x09,0x09,0x09,0x06}, // P
  {0x3E,0x41,0x51,0x21,0x5E}, // Q
  {0x7F,0x09,0x19,0x29,0x46}, // R
  {0x46,0x49,0x49,0x49,0x31}, // S
  {0x01,0x01,0x7F,0x01,0x01}, // T
  {0x3F,0x40,0x40,0x40,0x3F}, // U
  {0x1F,0x20,0x40,0x20,0x1F}, // V
  {0x3F,0x40,0x38,0x40,0x3F}, // W
  {0x63,0x14,0x08,0x14,0x63}, // X
  {0x07,0x08,0x70,0x08,0x07}, // Y
  {0x61,0x51,0x49,0x45,0x43},  // Z
  {0x3E,0x45,0x49,0x51,0x3E}, // 0
  {0x00,0x42,0x7F,0x40,0x00}, // 1
  {0x62,0x51,0x49,0x49,0x46}, // 2
  {0x22,0x41,0x49,0x49,0x36}, // 3
  {0x18,0x14,0x12,0x7F,0x10}, // 4
  {0x2F,0x49,0x49,0x49,0x31}, // 5
  {0x3E,0x49,0x49,0x49,0x30}, // 6
  {0x01,0x71,0x09,0x05,0x03}, // 7
  {0x36,0x49,0x49,0x49,0x36}, // 8
  {0x06,0x49,0x49,0x49,0x3E}  // 9


};

#define CHAR_WIDTH  5
#define CHAR_HEIGHT 7
#define CHAR_SPACING 1
#define BG_COLOR 0xFFE0  // yellow background

void drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint8_t size) {
    //if size=2 then text is 2x bigger
    int index;
    // Allow A–Z
    if (c >= 'A' && c <= 'Z') {
  	index = c - 'A';
    }
    // Allow 0–9
    else if (c >= '0' && c <= '9') {
 	index = 26 + (c - '0');
    }
    else {
  	return; // unsupported character
    }

    const uint8_t *bitmap = font[index];

    // 5x7 window is default
    tft_window(x, y, x + CHAR_WIDTH * size - 1, y + CHAR_HEIGHT * size - 1);
    //CS_LOW();
    DC_HIGH();
   
    for (uint8_t row = 0; row < CHAR_HEIGHT; row++) {
        for (uint8_t yscale = 0; yscale < size; yscale++) {
            for (uint8_t col = 0; col < CHAR_WIDTH; col++) {
                uint8_t line = bitmap[col];
                uint16_t px = (line & (1 << row)) ? color : BG_COLOR;

                for (uint8_t xscale = 0; xscale < size; xscale++) {
                    SPDR = px >> 8; while (!(SPSR & (1 << SPIF)));
                    SPDR = px & 0xFF; while (!(SPSR & (1 << SPIF)));
                }
            }
        }
    }


    CS_HIGH();
}


void drawString(uint8_t x, uint8_t y, const char *str, uint16_t color, uint8_t size) {
    while (*str) {
        drawChar(x, y, *str, color, size);
        x += (CHAR_WIDTH + CHAR_SPACING) * size;
        str++;
    }
}
//=======draw filled rect
void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    uint32_t pixels = (uint32_t)w * h;

    tft_window(x, y, x + w - 1, y + h - 1);

    while (pixels--) {
        SPDR = color >> 8; while (!(SPSR & (1 << SPIF)));
        SPDR = color & 0xFF; while (!(SPSR & (1 << SPIF)));
    }

    CS_HIGH();
}


//============
int main(void) {
    spi_init();
    gc9a01_init();
    
        fillScreen(0xF800); // Red
        _delay_ms(1000);
        fillScreen(0x07E0); // Green
        _delay_ms(1000);
        fillScreen(0x001F); // Blue
        _delay_ms(1000);
	fillScreen(0xFFE0); // yellow
        _delay_ms(1000);
	drawString(10, 5, "ABCD EFGH", 0x001F, 3); // blue text
	drawString(10, 30, "IJKLMNOPQRSTUVWXYZ", 0x001F, 2); // blue text
	drawString(70, 50, "01234 56789", 0x001F, 2); // blue text
	fillRect(10, 50, 50, 50, 0xF800); // red rect
	while(1) {
        // Keep the MCU here so it doesn't reset or crash
    }

}
