# Turntable
A supplemental program for displaying a rotating visualization of PolySets compatible with NIU's rd_view program.

## Usage
./turntable filepath  
--Pipe to rd_view--  
./turntable filepath | ./rd_view -

## Options
- **-f frame_count**: Will cause the animation to end after a the specified number of frames.
- **-a**: Will render axis lines at the origin.
- **-l**: Will enable 3 axis-aligned far lights to better display angles, faces, and reflections.
- **-r X | Y**: Changes the axis of rotation to the one specified
- **-v velocity**: Changes the speed of the rotation.
