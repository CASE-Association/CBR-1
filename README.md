# CBR-1
Case Balancing Robot 1

![top](https://raw.githubusercontent.com/CASE-Association/CBR-1/main/Pictures/IMG_20210428_215750.jpg)

Beginner balancing robot developed by CASE Association.

# Bill of materials
  - 1 x Arduino Nano
  - 1 x TB6612FNG (DC motor driver)
  - 1 x MPU6050 (Inertial measurement unit)
  - 1 x CBR-1 3D-printed chassi
  - 2 x 6V DC motors (with wheels)
  - 1 x Rockerswitch or other switch (between 9V battery and PCB)
  - 1 x 4xAA batteryholder
  - 1 x 9V batteryholder
  - 4 x M3x10 screw (M3x12 might also work)
  - 4 x M3 nut
  - Wires
  - Heatshrink

Optional:

  - 1 x Rockerswitch or other switch (now between the 4xAA batteryholder and PCB)
  - 2 x LED
  - 2 x Resistors R1 and R2
  - Various pin-connectors
For R1 and R2, use whatever resistance you prefer for LEDs in these applications, but around 300 ohm works fine. We used 100 Ohm for a red LED and 300 Ohm for a blue LED.

All of the components are available to purchase in the iZettle, but feel free to use your own components if you for instance want a more powerful battery to power the motors. The most important thing is that you are able to supply 9-12 V to the Arduino and at least 6 V to the motors (a somewhat higher voltage will work, you'll get more power!). Using 4xAA (4 in series) is the absolute minimum battery solution, considering the max current you get from them.


The 3D-printed chassi can, if not bought through iZettle, be redesigned and 3D-printed if you like. The STL file is provided in the CAD folder, and you can also design things to add to the robot! The picture below shows an example of a way to increase the distance between the ground and the centrum of mass by placing the batteries on top. This makes it easier for the robot to balance!

![top](https://raw.githubusercontent.com/CASE-Association/CBR-1/main/Pictures/Andreas%20dubbeldeckare.jpg)

The STL file for this extra piece can also be found in the CAD folder.

# Soldering
There's quite a bit of it.

## Soldering components to the PCB

![top](https://raw.githubusercontent.com/CASE-Association/CBR-1/main/Pictures/IMG_20210429_180747__01.jpg)

The three main components to solder to the PCB are:

  - Arduino Nano
  - TB6612FNG
  - MPU6050

The PCB is marked where to solder each component, but you can attach them in two ways:

### Directly to the PCB
This gives the most clean look, but can be quite troublesome if something goes wrong. It's very important that you orient the components in the correct way (they fit both ways, but only one works)! Double check that the ports on the components match the ones on the PCB before soldering.

### With female pin connectors to the PCB
Solder female pin connectors to the PCB and then attach your components to them. This way you can reuse components if you don't feel like buying the same component for another project in the future. If a component fails, you can also replace them much easier.

Connectors can be soldered to the ports for 6-12 V, powersupply, motor A and motor B if you like. This makes it easier to detach parts of the robot without needing to desolder anything. In the example on page 1, 90° male pin connectors are used on the PCB while the motors and batteries all have female pin connectors soldered to their respective cables.



