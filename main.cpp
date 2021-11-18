#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

bool isClose(double n1, double n2, double eps);
double getRad(sf::CircleShape p, sf::CircleShape obst);
double getAng(sf::CircleShape p, sf::CircleShape obst, double rad);
int spin(double omega, bool dir, double vels[]);

//TODO:

//Picks closest obstacle
//make latching on clean (coinsistently picks right direction, goes to smallest circle, etc.)
//latching on draws line
//new circles spawn, random color and shape
// border lines change color, inside or out
//impact with borders, obstacles is game over
//Start button, timer, game over screen


int main()
{



    //"map" instantiation
    sf::CircleShape player(9.d);
    sf::RectangleShape border1(sf::Vector2f(700.d, 5.d));
    sf::RectangleShape border2(sf::Vector2f(700.d, 5.d));
    sf::CircleShape obst1(15.d);
    sf::CircleShape obst2(15.d);

    //map setup
    border1.rotate(90.d);
    border2.rotate(90.d);
    border1.setFillColor(sf::Color(170.d,10.d,50.d));
    border2.setFillColor(sf::Color(170.d,10.d,50.d));
    border1.setPosition(35.d, 0.d);
    border2.setPosition(370.d, 0.d);
    player.setFillColor(sf::Color(250.d, 250.d, 250.d));
    player.setPosition(191.d, 460.d);
    obst1.setFillColor(sf::Color(250.d, 0.d, 126.d));
    obst1.setPosition(230.d, 350.d);
    obst2.setFillColor(sf::Color(50.d, 200.d, 26.d));
    obst2.setPosition(90.d, 170.d);
    double stepSize = 3;
    double mapVel[]= {0, stepSize};
    double cRad = -1;
    double cAng = -1;
    double angVel = 0;
    bool cDir = true; //true is clockwise

    // create the window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(400, 700), "SFML shapes", sf::Style::Default, settings);
    sf::Clock clock;


    //Temporary support code
    double tracerRad = 2;
    std::vector<sf::CircleShape> tracer(1);
    sf::CircleShape firstShape(tracerRad);
    tracer[0] = firstShape;
    tracer[0].setPosition(191.d+player.getRadius(), 460.d+player.getRadius());
    int tracerLen = 1; //Number of tracer dots placed
    int tracerMaxLen = 700; //Maximum number of tracer dots to write at a time
    int tracerCounter = 0; //Number of frames since last placement
    int tracerRate = 5; //Number of frames between placement

    // run the program as long as the window is open
    while (window.isOpen())
    {

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time elapsed = clock.getElapsedTime();
        if (elapsed.asMilliseconds() >= 16.666) //Time to do a new frame
        {
          clock.restart();

          //Support Code
          tracerCounter++;
          if (tracerCounter == tracerRate){
            tracerCounter = 0;
            sf::CircleShape tempShape(tracerRad);
            tracer.push_back(tempShape);
            tracerLen = tracerLen + 1;
            tracer[tracerLen-1].setPosition(191.d+player.getRadius(), 460.d+player.getRadius());
            if (tracerLen > tracerMaxLen){
              tracer.erase(tracer.begin());
              tracerLen = tracerLen - 1;
            }
          }

          //Logic and motion
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
          {
            // on spacebar, do something. For now, move up

            if (cRad == -1){ //The first time we detect a pressed spacebar
              cRad = getRad(player, obst1); //radius of circle around obstacle
              cAng = getAng(player, obst1, cRad); // angle of current position
              mapVel[0] = stepSize*cos(cAng+((3*M_PI)/2));
              mapVel[1] = stepSize*sin(cAng+((3*M_PI)/2));
              angVel = stepSize/(cRad); //angular velocity
              std::cout << cAng;
              std::cout << '\n';

              if ((cAng >= 0 && cAng < M_PI && mapVel[0] < 0) || (cAng >= M_PI && mapVel[0] >= 0)){ //needs to be better, gets direction
                cDir = false;
              }
              else {
                cDir = true;
              }

            }

          }
          else{ //Reset when we let go of spacebar
            cRad = -1;
            border1.setFillColor(sf::Color(170.d,10.d,50.d));
            border2.setFillColor(sf::Color(170.d,10.d,50.d));
          }

          if (cRad >= 0){

            spin(angVel, cDir, mapVel);

            border1.setFillColor(sf::Color(0.d,60.d,250.d));
            border2.setFillColor(sf::Color(0.d,60.d,250.d));

          }

          obst1.move(mapVel[0], mapVel[1]);
          obst2.move(mapVel[0], mapVel[1]);
          border1.move(mapVel[0], 0);
          border2.move(mapVel[0], 0);

          //Support code
          for (int i = 0; i < tracerLen; i++){
            tracer[i].move(mapVel[0], mapVel[1]);
          }

          // clear and draw
          window.clear(sf::Color::Black);
          window.draw(player);
          window.draw(border1);
          window.draw(border2);
          window.draw(obst1);
          window.draw(obst2);

          for (int i = 0; i < tracerLen; i++){
            window.draw(tracer[i]);
          }

          // end the current frame
          window.display();
      }
  }
    return 0;
}


bool isClose(double n1, double n2, double eps){
  return fabs(n1-n2)<=eps;
}

double getRad(sf::CircleShape p, sf::CircleShape obst){
  double distx = (p.getPosition().x + p.getRadius()) - (obst.getPosition().x + obst.getRadius());
  double disty = (p.getPosition().y + p.getRadius()) - (obst.getPosition().y + obst.getRadius()); //get distance
  return sqrt(distx*distx + disty*disty); //radius of circle around obstacle
}
double getAng(sf::CircleShape p, sf::CircleShape obst, double rad){
  double distx = (p.getPosition().x + p.getRadius()) - (obst.getPosition().x + obst.getRadius());
  double disty = (p.getPosition().y + p.getRadius()) - (obst.getPosition().y + obst.getRadius());
  //double yAng = asin(disty/rad);
  double xAng = acos(distx/rad);
  if (disty < 0){ //player is above obstacle, negative y distance
    xAng = (2*M_PI) - xAng;
  }
  return xAng;
}

int spin(double omega, bool dir, double vels[]){
  double c = 0;
  double s = 0;
  if (dir){
    c = cos(omega); //First, rotate by some amount, but you will get rounding errors
    s = sin(omega);
  }
  else{
    c = cos(-omega); //If we need to rotate the other way, reverse the velocity
    s = sin(-omega);
  }
  double x_temp = vels[0];
  double y_temp = vels[1];
  vels[0] = c*x_temp - s*y_temp; //Rotation matrix, Velocity should rotate the same speed as the player
  vels[1] = s*x_temp + c*y_temp;
  return 0;
}
