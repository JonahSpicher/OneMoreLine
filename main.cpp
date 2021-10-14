#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>


//TODO:

//Can latch onto circles
//make latching on clean (coinsistenyl picks right direction, goes to smallest circle, etc.)
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

          //Logic and motion

          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
          {
            // on spacebar, do something. For now, move up
            if (cRad == -1){
              double distx = (player.getPosition().x + player.getRadius()) - (obst1.getPosition().x + obst1.getRadius());
              double disty = (player.getPosition().y + player.getRadius()) - (obst1.getPosition().y + obst1.getRadius()); //get distance

              cRad = sqrt(distx*distx + disty*disty); //radius of circle around obstacle
              cAng = asin(disty/cRad); // angle of current position
              std::cout << "Angle0: ";
              std::cout << cAng;
              std::cout << "\n";
              angVel = stepSize/(cRad); //angular velocity
              // if (cAng <= 0){ //fix angle
              //   cAng = cAng + 360;
              // }
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
            //Update mapVel to stay in circle
            // std::cout << "Angle2: ";
            // std::cout << cAng;
            // std::cout << "\n";
            cAng = cAng + angVel;
            std::cout << "Angle1: ";
            std::cout << cAng;
            std::cout << "\n";



            // double c = cos(cAng*M_PI/180); //Sad this was a fun idea
            // double s = sin(cAng*M_PI/180);
            //
            // std::cout << "c, s: ";
            // std::cout << c;
            // std::cout << ", ";
            // std::cout << s;
            // std::cout << "\n";
            // double x_temp = mapVel[0];
            // double y_temp = mapVel[1];
            // mapVel[0] = c*x_temp - s*y_temp; //Rotation matrix, Velocity should rotate the same speed as the player
            // mapVel[1] = s*x_temp + c*y_temp;


            double newx = (obst1.getPosition().x+obst1.getRadius()) + (cRad*cos(cAng)); //should work?
            double newy = (obst1.getPosition().y+obst1.getRadius()) + (cRad*sin(cAng));

            std::cout << "Current x: ";
            std::cout << obst1.getPosition().x;
            std::cout << "\n";

            std::cout << "newx: ";
            std::cout << newx;
            std::cout << "\n";

            mapVel[0] = (player.getPosition().x + player.getRadius()) - newx;
            mapVel[1] = (player.getPosition().y + player.getRadius()) - newy;

            std::cout << "Map velocity: ";
            std::cout << mapVel[0];
            std::cout << ", ";
            std::cout << mapVel[1];
            std::cout << "\n";


            border1.setFillColor(sf::Color(0.d,60.d,250.d));
            border2.setFillColor(sf::Color(0.d,60.d,250.d));

          }

          obst1.move(mapVel[0], mapVel[1]);
          border1.move(mapVel[0], 0);
          border2.move(mapVel[0], 0);

          // clear and draw
          window.clear(sf::Color::Black);
          window.draw(player);
          window.draw(border1);
          window.draw(border2);
          window.draw(obst1);

          // end the current frame
          window.display();
      }
  }
    return 0;
}
