# Class Reference 
To use M5Stack-Avatar.

Note that **mruby/c does not distingush between class methods and instance methods.**

## Implemented classes

### Avatar (c_avatar.cpp)
- new
  > Creates a new instance of Avatar
  > The font in speech baloon is set to fonts::lgfxJapanGothic_12.
- start
  > Starts the avatar. Not blocked.
- set_position(x,y)
  > Sets the center position to ('x','y')
- scale= _scale_
  > Sets the drawing scale. The center position is the basis point.
- set_speech_text(text)
  > Opens speech baloon and write `text` in it.
  > To close the baloon, set "" as `text`.
- set_rotation(deg)
  > Sets the rotation of the face in `deg` degree.
- set_expression(expr)
  > Sets expression to `expr` in number.
  > `expr`is one of 1:Happy, 2:Angry, 3:Sad, 4:Doubt, 5:Sleepy, 6:Neutral

