#include "Joystick.h"

Joystick::Joystick(PinName vertPin, PinName horizPin, PinName clickPin) {
  vert = new AnalogIn(vertPin);
  horiz = new AnalogIn(horizPin);
  click = new InterruptIn(clickPin);
}

void Joystick::init() {
  // read centred values of joystick
  _x0 = horiz->read();
  _y0 = vert->read();

  // this assumes that the joystick is centred when the init function is called
  // if perfectly centred, the pots should read 0.5, but this may
  // not be the case and x0 and y0 will be used to calibrate readings

  // turn on pull-down for button -> this assumes the other side of the button
  // is connected to +3V3 so we read 1 when pressed and 0 when not pressed
  click->mode(PullUp);
  // we therefore need to fire the interrupt on a rising edge
  click->fall(callback(this, &Joystick::click_isr));
  // need to use a callback since mbed-os5 - basically tells it to look in this
  // class for the ISR
  _click_flag = 0;
}

Direction Joystick::get_direction() {
  float angle = get_angle(); // 0 to 360, -1 for centred

  Direction dir;
  // partition 360 into segments and check which segment the angle is in
  if (angle < 0.0f) {
    dir = CENTRE;               // check for -1.0 angle
  } else if (angle < 45.0f) { // then keep going in 45 degree increments
    dir = u;
  } else if (angle < 135.0f) {
    dir = r;
  } else if (angle < 225.0f) {
    dir = d;
  } else if (angle < 315.0f) {
    dir = l;
  } else {
    dir = u;
  }

  return dir;
}

// this method gets the magnitude of the joystick movement
float Joystick::get_mag() {
  Polar p = get_polar();
  return p.mag;
}

// this method gets the angle of joystick movement (0 to 360, 0 North)
float Joystick::get_angle() {
  Polar p = get_polar();
  return p.angle;
}

// get raw joystick coordinate in range -1 to 1
// Direction (x,y)
// North     (0,1)
// East      (1,0)
// South     (0,-1)
// West      (-1,0)
Vector2D Joystick::get_coord() {
  // read() returns value in range 0.0 to 1.0 so is scaled and centre value
  // substracted to get values in the range -1.0 to 1.0
  float x = 2.0f * (horiz->read() - _x0);
  float y = 2.0f * (vert->read() - _y0);

  // Note: the x value here is inverted to ensure the positive x is to the
  // right. This is simply due to how the potentiometer on the joystick
  // I was using was connected up. It could have been corrected in hardware
  // by swapping the power supply pins. Instead it is done in software so may
  // need to be changed depending on your wiring setup

  Vector2D coord = {-x, y};
  return coord;
}

// This maps the raw x,y coord onto a circular grid.
// See:  http://mathproofs.blogspot.co.uk/2005/07/mapping-square-to-circle.html
Vector2D Joystick::get_mapped_coord() {
  Vector2D coord = get_coord();

  // do the transformation
  float x = coord.x * sqrt(1.0f - pow(coord.y, 2.0f) / 2.0f);
  float y = coord.y * sqrt(1.0f - pow(coord.x, 2.0f) / 2.0f);

  Vector2D mapped_coord = {x, y};
  return mapped_coord;
}

// this function converts the mapped coordinates into polar form
Polar Joystick::get_polar() {
  // get the mapped coordinate
  Vector2D coord = get_mapped_coord();

  // at this point, 0 degrees (i.e. x-axis) will be defined to the East.
  // We want 0 degrees to correspond to North and increase clockwise to 359
  // like a compass heading, so we need to swap the axis and invert y
  float x = coord.y;
  float y = coord.x;

  float mag = sqrt(x * x + y * y); // pythagoras
  float angle = RAD2DEG * atan2(y, x);
  // angle will be in range -180 to 180, so add 360 to negative angles to
  // move to 0 to 360 range
  if (angle < 0.0f) {
    angle += 360.0f;
  }

  // the noise on the ADC causes the values of x and y to fluctuate slightly
  // around the centred values. This causes the random angle values to get
  // calculated when the joystick is centred and untouched. This is also when
  // the magnitude is very small, so we can check for a small magnitude and then
  // set the angle to -1. This will inform us when the angle is invalid and the
  // joystick is centred

  if (mag < TOL) {
    mag = 0.0f;
    angle = -1.0f;
  }

  Polar p = {mag, angle};
  return p;
}

bool Joystick::button_pressed() {
  // ISR must have been triggered
  if (_click_flag) {
    _click_flag = 0; // clear flag
    return true;
  } else {
    return false;
  }
}

void Joystick::click_isr() { 
    _click_flag = 1; 
}