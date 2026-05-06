import processing.serial.*;
import java.awt.event.KeyEvent;
import java.io.IOException;

Serial myPort;

String angle="";
String distance="";
String data="";
String noObject;
float pixsDistance;
int iAngle, iDistance;
int index=0;
float maxDistance = 75;

void setup() {
  size(1200, 700);
  smooth();
  delay(2000);
  try {
    myPort = new Serial(this, "COM3", 115200);
    myPort.bufferUntil('.');
  } catch (Exception e) {
    println("Could not open COM3: " + e.getMessage());
  }
}

void draw() {
  fill(98,245,31);
  noStroke();
  fill(0,4); 
  rect(0, 0, width, height-height*0.065); 
  
  fill(98,245,31); // green color
  drawRadar(); 
  drawLine();
  drawObject();
  drawText();
}

void serialEvent(Serial myPort) {
  try {
    data = myPort.readStringUntil('.');
    if (data != null) {
      data = data.substring(0,data.length()-1);
      index = data.indexOf(",");
      angle= data.substring(0, index);
      distance= data.substring(index+1, data.length());
      iAngle = int(angle);
      iDistance = int(distance);
    }
  } catch (Exception e) {
    println("Serial read error: " + e.getMessage());
  }
}

boolean outOfRange() {
  return iDistance < 0 || iDistance > maxDistance;
}

void drawRadar() {
  pushMatrix();
  translate(width/2,height-height*0.074);
  noFill();
  strokeWeight(2);
  stroke(98,245,31);
  float[] arcSizes = {0.0625, 0.27, 0.479, 0.687};
  for (float s : arcSizes) {
    arc(0, 0, width - width*s, width - width*s, PI, TWO_PI);
  }
  line(-width/2,0,width/2,0);
  int[] angles = {30, 60, 90, 120, 150};
  for (int a : angles) {
    line(0, 0, (-width/2)*cos(radians(a)), (-width/2)*sin(radians(a)));
  }
  line((-width/2)*cos(radians(30)),0,width/2,0);
  popMatrix();
}

void drawObject() {
  pushMatrix();
  translate(width/2,height-height*0.074);
  strokeWeight(9);
  
  if(!outOfRange()) {
    stroke(255,10,10); //red color
    pixsDistance = iDistance*((height-height*0.1666)*(1.0/maxDistance)); 
    line(pixsDistance*cos(radians(iAngle)),-pixsDistance*sin(radians(iAngle)),(width-width*0.505)*cos(radians(iAngle)),-(width-width*0.505)*sin(radians(iAngle)));
  }
  else {
    stroke(98,245,31); //green color
    line(0,0,(width-width*0.505)*cos(radians(iAngle)),-(width-width*0.505)*sin(radians(iAngle)));
  }
  
  popMatrix();
}

void drawLine() {
  pushMatrix();
  strokeWeight(9);
  stroke(30,250,60);
  translate(width/2,height-height*0.074);
  line(0,0,(height-height*0.12)*cos(radians(iAngle)),-(height-height*0.12)*sin(radians(iAngle)));
  popMatrix();
}

void drawText() {
  pushMatrix();
  
  fill(0,0,0);
  noStroke();
  rect(0, height-height*0.0648, width, height);
  fill(98,245,31);
  textSize(25);
  
  float[] labelPositions = {0.395, 0.29, 0.185, 0.081};
  for (int i = 0; i < 4; i++) {
    text((int)(maxDistance * (i+1) * 0.25) + "cm", width - width*labelPositions[i], height - height*0.0833);
  }
  
  textSize(40);
  
  if(outOfRange()) {
    text("Distance: " + "      " + "cm", width-width * 0.6, height-height*0.015);
  } else {
    text("Distance: " + iDistance + " cm", width-width * 0.6, height-height*0.015);
  }
  
  textSize(25);
  fill(98,245,60);
  translate((width-width*0.4994)+width/2*cos(radians(30)),(height-height*0.0907)-width/2*sin(radians(30)));
  rotate(-radians(-60));
  text("30°",0,0);
  resetMatrix();
  translate((width-width*0.503)+width/2*cos(radians(60)),(height-height*0.0888)-width/2*sin(radians(60)));
  rotate(-radians(-30));
  text("60°",0,0);
  resetMatrix();
  translate((width-width*0.507)+width/2*cos(radians(90)),(height-height*0.0833)-width/2*sin(radians(90)));
  rotate(radians(0));
  text("90°",0,0);
  resetMatrix();
  translate(width-width*0.513+width/2*cos(radians(120)),(height-height*0.07129)-width/2*sin(radians(120)));
  rotate(radians(-30));
  text("120°",0,0);
  resetMatrix();
  translate((width-width*0.5104)+width/2*cos(radians(150)),(height-height*0.0574)-width/2*sin(radians(150)));
  rotate(radians(-60));
  text("150°",0,0);
  popMatrix(); 
}
