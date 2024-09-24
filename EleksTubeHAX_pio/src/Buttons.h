#ifndef BUTTONS_H
#define BUTTONS_H

#include "GLOBAL_DEFINES.h"

/*
 * A simple class to keep track of button states.  It's asynchronous, only checking
 * for changes between calls to .loop().  If a button changes states multiple times 
 * between calls to .loop(), only the state when .loop() is called is registered.
 */

// For HIGH and LOW
#include <Arduino.h>

class Button {
public:
  Button( 
      uint8_t bpin,
      uint8_t active_state=LOW,
      uint32_t long_press_ms=500,
      uint32_t double_click_ms=300) // Add double_click_ms parameter
    : bpin(bpin), active_state(active_state), long_press_ms(long_press_ms), double_click_ms(double_click_ms),
      down_last_time(false), state_changed(false), millis_at_last_transition(0), millis_at_last_release(0), button_state(idle) {}

  /*
   * States:
   * idle: button was up (not pressed) last time, and is still up now.
   * down_edge: button was up last time, but is down now. ie: it has just been pressed.
   * down: button was down last time, and is still down, but for less than long_press_ms.
   * down_long_edge: button has been down for at least long_press_ms, the first time.
   * down_long: button has been down for at least long_press_ms, after the first time.
   * up_edge: button was down last time, but is up now.  ie: it has just been released from a short press.
   * up_long_edge: button was down_long last time, but is up now. ie: just released from a long press.
   * 
   * Note: There is no up_long state, that's just idle and it goes into idle immediately
   * after up_edge.
   */
  enum state {  idle, 
                down_edge, 
                down, 
                down_long_edge, 
                down_long, 
                up_edge, 
                up_long_edge, 
                single_click, // Add single_click state
                double_click, // Add double_click state
                triple_click, // Add triple_click state
                long_click, // Add long_click state
                num_states
             };

  static const String state_str[num_states];
  void begin();
  void loop();
  
  // These are only updated when loop() is called, not when the getters are called.
  state getState()            { return button_state; }
  String getStateStr()        { return state_str[button_state]; }
  bool stateChanged()         { return state_changed; }
  void setDownEdgeState()     { button_state = down_edge; }
  void setDownLongEdgeState() { button_state = down_long_edge; }
  void setUpEdgeState()       { button_state = up_edge; }
  void setUpLongEdgeState()   { button_state = up_long_edge; }
  uint32_t millisInState()    { return millis_at_last_loop - millis_at_last_transition; }
  bool isIdle()               { return button_state == idle; }
  bool isDownEdge()           { return button_state == down_edge; }
  bool isDown()               { return button_state == down; }
  bool isDownLongEdge()       { return button_state == down_long_edge; }
  bool isDownLong()           { return button_state == down_long; }
  bool isUpEdge()             { return button_state == up_edge; }
  bool isUpLongEdge()         { return button_state == up_long_edge; }
  bool isSingleClick()        { return button_state == single_click; } // Add isSingleClick method
  bool isDoubleClick()        { return button_state == double_click; } // Add isDoubleClick method
  bool isTripleClick()        { return button_state == triple_click; } // Add isTripleClick method
  bool isLongClick()          { return button_state == long_click; }   // Add isLongClick method
  bool isDownLongy()          { return button_state == down_long_edge || button_state == down_long; }
  bool isDowny()              { return button_state == down_edge || button_state == down || isDownLongy(); }
  bool isUpy()                { return button_state == idle || button_state == up_edge || button_state == up_long_edge; }
  
private:
  // Config
  const uint8_t     bpin;
  const uint8_t     active_state;
  const uint32_t    long_press_ms;
  const uint32_t    double_click_ms; // Add double_click_ms member
  
  // Internal state
  bool              down_last_time;
  bool              state_changed;
  uint32_t          millis_at_last_transition;
  uint32_t          millis_at_last_release; // Add millis_at_last_release member
  uint32_t          millis_at_last_press; // Add millis_at_last_press member
  bool              single_click_pending; // Add single_click_pending flag
  uint32_t          millis_at_last_loop;
  state             button_state;

  bool isButtonDown() { return digitalRead(bpin) == active_state; }
};

#ifdef ONE_BUTTON_ONLY_MENU

class Buttons {
public:
  Buttons() : mode(BUTTON_MODE_PIN) {}

  void begin();
  void loop();
  bool stateChanged();
    
  // Just making them public, so we don't have to proxy everything.
  Button mode;
private: 
};

#endif

/*
 * A simple helper class to call common functions on all buttons at once.
 */
#ifndef ONE_BUTTON_ONLY_MENU

class Buttons {
public:
  Buttons() : left(BUTTON_LEFT_PIN), mode(BUTTON_MODE_PIN), right(BUTTON_RIGHT_PIN), power(BUTTON_POWER_PIN) {}

  void begin();
  void loop();
  bool stateChanged();
    
  // Just making them public, so we don't have to proxy everything.
  Button left, mode, right, power;
private: 
};

#endif

#endif // BUTTONS_H
