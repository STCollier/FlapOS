#ifndef KEYBOARD_H
#define KEYBOARD_H

enum Scancode {
   KEY_ERROR = 0x0,    
   KEY_ESC,      
   KEY_ONE,      
   KEY_TWO,      
   KEY_THREE,    
   KEY_FOUR,     
   KEY_FIVE,     
   KEY_SIX,      
   KEY_SEVEN,    
   KEY_EIGHT,    
   KEY_NINE,     
   KEY_ZERO,     
   KEY_MINUS,    
   KEY_PLUS,     
   KEY_BACKSPACE,
   KEY_TAB,      
   KEY_Q,        
   KEY_W,        
   KEY_E,        
   KEY_R,        
   KEY_T,        
   KEY_Y,        
   KEY_U,        
   KEY_I,        
   KEY_O,        
   KEY_P,        
   KEY_BRACE_OPEN,
   KEY_BRACE_CLOSE,
   KEY_ENTER,    
   KEY_LEFT_CTRL,
   KEY_A,       
   KEY_S,       
   KEY_D,       
   KEY_F,       
   KEY_G,       
   KEY_H,       
   KEY_J,       
   KEY_K,       
   KEY_L,       
   KEY_SEMICOLON,        
   KEY_SINGLE_QUOTE,
   KEY_TILDA,
   KEY_LEFT_SHIFT,
   KEY_BACK_SLASH,
   KEY_Z,       
   KEY_X,        
   KEY_C,        
   KEY_V,        
   KEY_B,        
   KEY_N,        
   KEY_M,        
   KEY_COMMA,    
   KEY_PERIOD,   
   KEY_FORWARD_SLASH,
   KEY_RIGHT_SHIFT,  
   KEY_ASTERISK,
   KEY_LEFT_ALT,     
   KEY_SPACE
};

struct Keyboard {
    bool keys[128];
};

void keyboard_init();
bool key_pressed(enum Scancode key);

#endif