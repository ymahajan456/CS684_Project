
#ifndef __GLCD_H

void glcd_cmd(unsigned char cmd);
void glcd_data(unsigned char data);
void glcd_config();
void glcd_init();
void glcd_setpage(unsigned char page);
void glcd_setcolumn(unsigned char column);
void glcd_printimage(unsigned char* image);
void glcd_cleardisplay();

#endif
