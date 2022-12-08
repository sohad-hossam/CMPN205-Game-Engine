#version 330 core
///---
out vec4 frag_color;
///-----
// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the top-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];
//imagine the the screen is divided into number of sqaures on the x and y 
//go get the number of square of each pixel on the x and y 
//the summation of this number will give either even the color it by a particular color or odd
//color it by the opsite color

//the idea is that we imagin that every pixel will be in a square and every square will have a number on x and number on y 
//how to get the number of that sqaure of every pixel?
//make an int division between the x coordinate and size , same for y 
//sum those division values it will be a color for even number and the opposite for odd 
float which_sqaure_x=gl_FragCoord.x/size;  // the integear value of the division between x.coordinate of the pixel 
//and the size of the square
float which_sqaure_y=gl_FragCoord.y/size; //the same for y
void main(){

//when you sum the divison value of x,size and y,size you will find that all even numbers will be with same color and the same for the odd 
 if (mod((int(which_sqaure_x)+int(which_sqaure_y)),2)!=0)
 {
    frag_color = vec4(colors[1], 1.0);
 }

 if (mod((int(which_sqaure_x)+int(which_sqaure_y)),2)==0)
 {
    frag_color = vec4(colors[0], 1.0);
 }
 }