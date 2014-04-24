#define MAX_7SEG    4

#define KBD_CONTROL DDRJ
#define KBD_INPUT   PINJ
#define KBD_OUTPUT  PORTJ


/* 
 * =================
 * 7 segment display
 * =================
 */
void display_7seg(int pos, uint8_t data);
void clear_7seg(void);

/* 
 * ========
 * Keyboard
 * ========
 */

/*
 * Read keyboard coordinates. It blocks until a key is pressed and released.
 */
void read_keyboard_coords(int* row, int* column);

/* 
 * ==================
 * Temperature sensor
 * ==================
 */

/*
 * Return the temperature in Celsius.
 */
int read_temperature(void);
