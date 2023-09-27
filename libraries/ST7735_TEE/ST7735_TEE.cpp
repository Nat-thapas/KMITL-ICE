// Graphics library by ladyada/adafruit with init code from Rossum 
// MIT license
// include ThaiFont Engine By ThaiEasyElec.com

#include "ST7735_TEE.h"
#include "glcdfont.c"
#if IF_ARDUINO
	#include "Arduino.h"
	#include <avr/pgmspace.h>
	#include "pins_arduino.h"
	#include "wiring_private.h"
	#define AO_H  *portOutputRegister(rsport) |= rspin
	#define AO_L  *portOutputRegister(rsport) &= ~rspin
	#define CS_H  *portOutputRegister(csport) |= cspin
	#define CS_L  *portOutputRegister(csport) &= ~cspin
#else
	#define AO_H  digitalWrite(AO, HIGH)
	#define AO_L  digitalWrite(AO, LOW)
	#define CS_H  digitalWrite(CS, HIGH)
	#define CS_L  digitalWrite(CS, LOW)
#endif

unsigned char SCL_,SDA_,AO,RST,CS;
int k=0;   
unsigned char px,py;  
unsigned char flag_up = 0; 

TEE_ST7735::TEE_ST7735(unsigned char csk,unsigned char sda,unsigned char A0,unsigned char rst,unsigned char cs)
{
	SCL_ = csk;
	SDA_ = sda;
	AO  = A0;
	RST = rst;
	CS = cs;
	#if IF_ARDUINO
		pinMode(SCL_,OUTPUT);
		pinMode(SDA_,OUTPUT);
		pinMode(AO,OUTPUT);
		pinMode(RST,OUTPUT);
		pinMode(CS,OUTPUT);
	#else
		pinMode(SCL_,OUTPUT);
		pinMode(SDA_,OUTPUT);
		pinMode(AO,OUTPUT);
		pinMode(RST,OUTPUT);
		pinMode(CS,OUTPUT);
	
		digitalWrite(CS,0);
		digitalWrite(RST,0);
		digitalWrite(AO,0);
		digitalWrite(SDA_,0);
		digitalWrite(SCL_,0);	
	#endif
	
}
void TEE_ST7735::init( uint8_t hor_ver)
{
	 #if IF_ARDUINO
		pinMode(SCL_, OUTPUT);
		sclkport = digitalPinToPort(SCL_);
		sclkpin = digitalPinToBitMask(SCL_);
		
		pinMode(SDA_, OUTPUT);
		sidport = digitalPinToPort(SDA_);
		sidpin = digitalPinToBitMask(SDA_);
		
		pinMode(CS, OUTPUT);
		digitalWrite(CS, LOW);
		cspin = digitalPinToBitMask(CS);
		csport = digitalPinToPort(CS);

		rspin = digitalPinToBitMask(AO);
		rsport = digitalPinToPort(AO);
  
	 #endif
	 
	 
	 initR(hor_ver);
	 writecommand(ST7735_DISPON);
	
}
void TEE_ST7735::spiwrite(uint8_t SPI_byte)
{    
	#if IF_ARDUINO
		volatile uint8_t *sclkportreg = portOutputRegister(sclkport);
		volatile uint8_t *sidportreg = portOutputRegister(sidport);
		int8_t i;

		*sclkportreg |= sclkpin;

		for (i=7; i>=0; i--) {
			*sclkportreg &= ~sclkpin;
			//SCLK_PORT &= ~_BV(SCLK);
		if (SPI_byte & _BV(i)) {
			*sidportreg |= sidpin;
			//digitalWrite(_sid, HIGH);
			//SID_PORT |= _BV(SID);
		} 
		else {
			*sidportreg &= ~sidpin;
			//digitalWrite(_sid, LOW);
			//SID_PORT &= ~_BV(SID);
		}	
    
		*sclkportreg |= sclkpin;
		//SCLK_PORT |= _BV(SCLK);
	}
	#else
	unsigned char SPI_count;
	for (SPI_count = 8; SPI_count > 0; SPI_count--)
	{	
		digitalWrite(SCL_,0);		
		digitalWrite(SDA_,SPI_byte & 0x80);
		SPI_byte = SPI_byte << 1; 
		digitalWrite(SCL_,1);
		//os_delay_us(10);
		
	}
	#endif
}
void TEE_ST7735::writecommand(uint8_t c) 
{
  AO_L;
  CS_L;
  spiwrite(c);
  CS_H;
 }
void TEE_ST7735::writedata(uint8_t c) 
{
  AO_H;
  CS_L;
  spiwrite(c);
  CS_H;
} 
void TEE_ST7735::setAddrWindow(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1)
{
  writecommand(ST7735_CASET);  // column addr set
  writedata(0x00);
  writedata(x0+0);   // XSTART 
  writedata(0x00);
  writedata(x1+0);   // XEND

  writecommand(ST7735_RASET);  // row addr set
  writedata(0x00);
  writedata(y0+0);    // YSTART
  writedata(0x00);
  writedata(y1+0);    // YEND

  writecommand(ST7735_RAMWR);  // write to RAM
}
void TEE_ST7735::pushColor(uint16_t color) 
{
  AO_H;
  CS_L;
  spiwrite(color >> 8);    
  spiwrite(color);   
  CS_H;
}
void TEE_ST7735::drawPixel(uint8_t x, uint8_t y,uint16_t color) 
{
  if ((x >= width) || (y >= height)) return;

  setAddrWindow(x,y,x+1,y+1);

  // setup for data
  AO_H;
  CS_L;
  spiwrite(color >> 8);    
  spiwrite(color);   
  CS_H;
}
void TEE_ST7735::fillScreen(uint16_t color) 
{
  uint8_t x,y;
  setAddrWindow(0, 0, width-1, height-1);

  // setup for data
  AO_H;
  CS_L;
 
  for (x=0; x < width; x++) {
    for (y=0; y < height; y++) {
      spiwrite(color >> 8);    
      spiwrite(color);    
    }
  }
  CS_H;
}

void TEE_ST7735::initR(unsigned char hor_ver) {
  
  #if IF_ARDUINO
	digitalWrite(CS, LOW);
	digitalWrite(RST, HIGH);
	delay(500);
	digitalWrite(RST, LOW);
	delay(500);
	digitalWrite(RST, HIGH);
	delay(500);
  #else
	digitalWrite(CS, LOW);
	digitalWrite(RST, HIGH);
	delay(500);
	digitalWrite(RST, LOW);
	delay(500);
	digitalWrite(RST, HIGH);
	delay(500);
  #endif
 writecommand(ST7735_SWRESET); // software reset
  delay(150);

  writecommand(ST7735_SLPOUT);  // out of sleep mode
  delay(500);

  writecommand(ST7735_FRMCTR1);  // frame rate control - normal mode
  writedata(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
  writedata(0x2C); 
  writedata(0x2D); 

  writecommand(ST7735_FRMCTR2);  // frame rate control - idle mode
  writedata(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
  writedata(0x2C); 
  writedata(0x2D); 

  writecommand(ST7735_FRMCTR3);  // frame rate control - partial mode
  writedata(0x01); // dot inversion mode
  writedata(0x2C); 
  writedata(0x2D); 
  writedata(0x01); // line inversion mode
  writedata(0x2C); 
  writedata(0x2D); 
  
  writecommand(ST7735_INVCTR);  // display inversion control
  writedata(0x07);  // no inversion

  writecommand(ST7735_PWCTR1);  // power control
  writedata(0xA2);      
  writedata(0x02);      // -4.6V
  writedata(0x84);      // AUTO mode

  writecommand(ST7735_PWCTR2);  // power control
  writedata(0xC5);      // VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD

  writecommand(ST7735_PWCTR3);  // power control
  writedata(0x0A);      // Opamp current small 
  writedata(0x00);      // Boost frequency

  writecommand(ST7735_PWCTR4);  // power control
  writedata(0x8A);      // BCLK/2, Opamp current small & Medium low
  writedata(0x2A);     

  writecommand(ST7735_PWCTR5);  // power control
  writedata(0x8A);    
  writedata(0xEE);     

  writecommand(ST7735_VMCTR1);  // power control
  writedata(0x0E);  

  writecommand(ST7735_INVOFF);    // don't invert display

  writecommand(ST7735_MADCTL);  // memory access control (directions)
 // writedata(0xC0);  // row address/col address, bottom to top refresh
 // madctl = 0xC0;
  
   if(hor_ver)
  {
	  writedata(0x60);  // row address/col address, bottom to top refresh
      madctl = 0x60;
	  width = 160;//128;
	  height = 128;//160;
  
  }
  else
  {
	  writedata(0xC0);  // row address/col address, bottom to top refresh
      madctl = 0xC0;
	  width = 128;
	  height =160;
  
  }
  
  
  writecommand(ST7735_COLMOD);  // set color mode
  writedata(0x05);        // 16-bit color

  writecommand(ST7735_CASET);  // column addr set
  writedata(0x00);
  writedata(0x00);   // XSTART = 0
  writedata(0x00);
  writedata(0x7F);   // XEND = 127

  writecommand(ST7735_RASET);  // row addr set
  writedata(0x00);
  writedata(0x00);    // XSTART = 0
  writedata(0x00);
  writedata(0x9F);    // XEND = 159

  writecommand(ST7735_GMCTRP1);
  writedata(0x0f);
  writedata(0x1a);
  writedata(0x0f);
  writedata(0x18);
  writedata(0x2f);
  writedata(0x28);
  writedata(0x20);
  writedata(0x22);
  writedata(0x1f);
  writedata(0x1b);
  writedata(0x23);
  writedata(0x37);
  writedata(0x00);
  writedata(0x07);
  writedata(0x02);
  writedata(0x10);
  writecommand(ST7735_GMCTRN1);
  writedata(0x0f); 
  writedata(0x1b); 
  writedata(0x0f); 
  writedata(0x17); 
  writedata(0x33); 
  writedata(0x2c); 
  writedata(0x29); 
  writedata(0x2e); 
  writedata(0x30); 
  writedata(0x30); 
  writedata(0x39); 
  writedata(0x3f); 
  writedata(0x00); 
  writedata(0x07); 
  writedata(0x03); 
  writedata(0x10); 
  

  writecommand(ST7735_DISPON);
  delay(100);

  writecommand(ST7735_NORON);  // normal display on
  delay(10);
}
void TEE_ST7735::fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		      uint16_t color) {
  // smarter version

  setAddrWindow(x, y, x+w-1, y+h-1);

  // setup for data
  AO_H;
  CS_L;
  for (x=0; x < w; x++) {
    for (y=0; y < h; y++) {
      spiwrite(color >> 8);    
      spiwrite(color);    
    }
  }
  CS_H;
}
void TEE_ST7735::drawFastLine(uint8_t x, uint8_t y, uint8_t length, 
			  uint16_t color, uint8_t rotflag)
{
  if (rotflag) {
    setAddrWindow(x, y, x, y+length);
  } else {
    setAddrWindow(x, y, x+length, y+1);
  }
  // setup for data
  AO_H;
  CS_L;
  
  while (length--) {
    spiwrite(color >> 8);    
    spiwrite(color);    
  }
  CS_H;
}


void TEE_ST7735::drawVerticalLine(uint8_t x, uint8_t y, uint8_t length, uint16_t color)
{
  if (x >= width) return;
  if (y+length >= height) length = height-y-1;

  drawFastLine(x,y,length,color,1);
}

void TEE_ST7735::drawHorizontalLine(uint8_t x, uint8_t y, uint8_t length, uint16_t color)
{
  if (y >= height) return;
  if (x+length >= width) length = width-x-1;

  drawFastLine(x,y,length,color,0);
}

// bresenham's algorithm - thx wikpedia
void TEE_ST7735:: drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color)
{
	 uint16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  uint16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;}

  for (; x0<=x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void TEE_ST7735::fillCircle(uint8_t x0, uint8_t y0, uint8_t r, uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawVerticalLine(x0, y0-r, 2*r+1, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    drawVerticalLine(x0+x, y0-y, 2*y+1, color);
    drawVerticalLine(x0-x, y0-y, 2*y+1, color);
    drawVerticalLine(x0+y, y0-x, 2*x+1, color);
    drawVerticalLine(x0-y, y0-x, 2*x+1, color);
  }
}

// draw a circle outline
void TEE_ST7735::drawCircle(uint8_t x0, uint8_t y0, uint8_t r, uint16_t color) 
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0, y0+r, color);
  drawPixel(x0, y0-r, color);
  drawPixel(x0+r, y0, color);
  drawPixel(x0-r, y0, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
    
  }
}

uint8_t TEE_ST7735::getRotation() 
{
  return madctl;
}

void TEE_ST7735::setRotation(uint8_t m) 
{
  madctl = m;
  writecommand(ST7735_MADCTL);  // memory access control (directions)
  writedata(madctl);  // row address/col address, bottom to top refresh
}

// draw a rectangle
void TEE_ST7735::drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) 
{
  // smarter version
  drawHorizontalLine(x, y, w, color);
  drawHorizontalLine(x, y+h-1, w, color);
  drawVerticalLine(x, y, h, color);
  drawVerticalLine(x+w-1, y, h, color);
}

// draw a string from memory

// draw a character
void TEE_ST7735::drawChar(uint8_t x, uint8_t y, char c, uint16_t color, uint8_t size) 
{
uint8_t i,j,line;
  for ( i =0; i<5; i++ ) 
{
	#if IF_ARDUINO
			 uint8_t line = pgm_read_byte(font+(c*5)+i);
	#else
			 line = font[(c*5)+i];
	#endif
    
    for ( j = 0; j<8; j++) {
      if (line & 0x1) {
	if (size == 1) // default size
	  drawPixel(x+i, y+j, color);
	else {  // big size
	  fillRect(x+i*size, y+j*size, size, size, color);
	} 
      }
      line >>= 1;
    }
  }
}

void TEE_ST7735::drawString(uint8_t x, uint8_t y, char *c, uint16_t color, uint8_t size) 
{
  while (c[0] != 0) {
    drawChar(x, y, c[0], color, size);
    x += size*6;
    c++;
    if (x + 5 >= width) {
      y += 10;
      x = 0;
    }
  }
}


void TEE_ST7735::drawChar_th(uint8_t x, uint8_t y,char c,uint16_t color)
{
  int i;
  int j;
  
  uint8_t xx=0,yy=0;
  uint8_t flag_en_th=1;
  uint8_t line; 
  uint8_t line2; 
  unsigned char cc = c;
	  if(cc>0x80)
	  {
		cc = cc-0x80;
		flag_en_th=1;
	 }
	 else
	 {
		if(cc == 0x09)
		cc = 0x20;
		cc = cc - 0x20;
		flag_en_th=0;
	 }
	 
	  for(i=(cc*26);i<(26*(cc+1));i+=2)
	  {
		 if(flag_en_th)
		 {
			#if IF_ARDUINO
			 line = pgm_read_byte(thai13x16+i);
			 line2 = pgm_read_byte(thai13x16+i+1);
			#else
			line = thai13x16[i];
			line2 = thai13x16[i+1];
			#endif
		}
		else
		{
			#if IF_ARDUINO
			 line = pgm_read_byte(eng13x16+i);
			 line2 = pgm_read_byte(eng13x16+i+1);
			#else
				line = eng13x16[i];
				line2 = eng13x16[i+1];
			#endif		
		}
		
		 int line_16 = (line<<8)|line2;
		 yy=0;
		 for(j=0;j<16;j++)
		 {
			 if(line_16&0x8000)
			 {
				drawPixel(x+xx,y+yy, color);
			 }
			 line_16=line_16<<1;
			 yy++;
		 }
		 xx++;
	     
	  }
}

void TEE_ST7735::Printstr(uint8_t x, uint8_t y,char *c,uint16_t color)
{
	unsigned char i=0;
    unsigned char v;
	unsigned char flag_th_utf;
	px=x;
	py=y;
	
	unsigned char mode,mode_next;
	int len;
	while(c[i])
	{
		i++;
	}
	
	len = i;
	i=0;
	
	while(c[i])
	{
				 
		if(conv_to_uchar(c[i]) ==0xE0)
		{
			i++;
			if(conv_to_uchar(c[i])==0xB8)
			{
				flag_th_utf=0;
			}
			if(conv_to_uchar(c[i])==0xB9)
			{
				flag_th_utf=64;
			}
			i++;
			mode = up_state(c[i-1],c[i]);
			switch(up_state(c[i-1],c[i]))
			{
				case 0:
				if((x+13)>width)
					{
						px=x;
						py=y;
						x=0;
						y=y+20;
					}
					if((i+2)<len)
					{
						mode_next = up_state(c[i+1],c[i+2]);
						if((mode_next == 1)&&((x+15)>width))
						{
							px=x;
							py=y;
							x=0;
							y=y+20;
						}
						else if((mode_next == 2)&&((x+15)>width))
						{
							px=x;
							py=y;
							x=0;
							y=y+20;
						}
						else if((mode_next == 3)&&((x+15)>width))
						{
							px=x;
							py=y;
							x=0;
							y=y+20;
						}
					}

					drawChar_th(x,y,c[i]+flag_th_utf,color);
					flag_up = 0;
				break;
				case 1:
					x=px;
					y=py;
					drawChar_th(x,y,c[i]+flag_th_utf,color);
					flag_up = 1;
				break;
				case 2:
					x = px+4;
					drawChar_th(x,py,c[i]+flag_th_utf,color);
					flag_up = 1;
				break;
				case 3:
						if(flag_up)
							y = py-4;
						x = px+2;
						drawChar_th(x,y,c[i]+flag_th_utf,color);
						y = py;
						x = px;
					break;
			}			
		}
		else
		{
			if((c[i]!='\r')&&(c[i]!='\n'))
			{
				drawChar_th(x,y,c[i],color);
			}
			
		}
		px=x;
		py=y;
		x=x+8;
		if(c[i]=='\n')
		{
			x=0;	
			y=y+20;
		}
	/*	if(x >= width)
		{
			px=x;
			py=y;
			x=0;
			y=y+20;
		}
		*/
		i++;
		
	}
}
unsigned char TEE_ST7735::up_state(unsigned char c1,unsigned char c2)
{
//  Return 0 = normal
//  Return 1 = back 1 char
//  Return 2 = back haft char
//  Return 3 = back 1 char and upper

	if(c1 == 0xB8)
	{
		if(((c2>=0xB4)&&(c2<=0xB9)))
			return(1);
		if(c2==0xB1)
			return(3);
		if(c2==0xB3)
			return(2);
	}
	if(c1 == 0xB9)
	{
		if(c2==0x87)
			return(1);
		if((c2>0x87)&&(c2<=0x8E))
		    return(3);
	}
	return(0);
}

unsigned char TEE_ST7735:: conv_to_uchar(char x)
{
	unsigned char v;
	v = x;
	return(v= v&0x0000FF);
}

void TEE_ST7735::showpic(const uint16_t *pic,uint8_t org_x,uint8_t org_y,uint8_t width,uint8_t high) 
{
  uint8_t x,y;
  k=0;
  setAddrWindow(org_x,org_y,(org_x+width)-1,(org_y+high)-1);

  // setup for data
   AO_H;
  CS_L;
  for (x=0; x < width; x++) {
    for (y=0; y < high; y++) 
   {
		#if IF_ARDUINO
			spiwrite(pgm_read_byte((*pic+k) >> 8));
            spiwrite(pgm_read_byte(*pic+k));			
		#else
			spiwrite(pic[k] >> 8);    
			spiwrite(pic[k]);
		#endif 
      k++;    
   }
  }
  CS_H;
}



