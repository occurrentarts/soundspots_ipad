//
//  ForceMap.cpp
//  msaFluidParticles
//
//  Created by amun on 9/28/11.
//  Copyright 2011 Occurrent Arts. All rights reserved.
//

#include "ForceMap.h"
using std::list;
using namespace std;
using namespace ci;

void ForceMap::init() {
    
    float winX = 1536;
    float winY = 2048;
	
    gPower = 6.0;
    float widMult = 1536 / 6.0;
    
    Force theIris = Force( Vec2f( winX/2.0f, winY/2.0f - 100.0 ), Force::CLOCKWISE_INWARDS, Vec2f(1.0 , 0.0), 30, 2. * gPower);
    mForces.push_front( theIris );
    
    /**
    Force thePupil = Force( Vec2f( winX/2.0f, winY/2.0f + 0.0), Vec2f(1.0 , 0.0), 5, 4. * gPower);
    thePupil.mOn = -2;
    
    Force theCornea = Force( Vec2f( winX/2.0f, winY/2.0f + 300.0), Vec2f(1.0 , 0.0), 5, 4. * gPower);
    theCornea.mOn = -1;

    mForces.push_front( thePupil );
    mForces.push_front( theCornea );
    **/


}

void ForceMap::changePower( float _pow ) {
    gPower = _pow;
    for( list<Force>::iterator forcesIt = mForces.begin(); forcesIt != mForces.end(); ++forcesIt ){
        (*forcesIt).mPower = _pow;    
    }
}

void ForceMap::update( ciMsaFluidSolver* fluidSolver ) {
    for( list<Force>::iterator forcesIt = mForces.begin(); forcesIt != mForces.end(); ++forcesIt ){
        (*forcesIt).applyForce( fluidSolver );    
    }
}

void ForceMap::initCenterStream() {
     float winX = 1536;
     float winH = 2048;
	
     float widMult = 1536 / 6.0;
     float vertSpacing = 250;
     float hShift = -50;
    
     Force centerLeft = Force( Vec2f(215, 450 ), Vec2f(1.0 , 0.0), 5, 0.4);
     centerLeft.mOn = 0;
     
     Force topLeft    = Force( Vec2f(widMult + hShift - 20, winH / 2.0 - vertSpacing - 50), Vec2f(1.0 , 1.0), 5, 1.1 * gPower);
     Force bottomLeft = Force( Vec2f(widMult + hShift - 20, winH / 2.0 + vertSpacing + 50), Vec2f(1.0 , -1.0), 5, 1.1 * gPower);
     topLeft.mOn = 1;
     bottomLeft.mOn = 2;
     
     Force topOne    = Force( Vec2f(widMult * 2 + hShift, winH / 2.0 - vertSpacing), Vec2f(1.0 , 1.0), 5, 1.1 * gPower);
     Force bottomOne = Force( Vec2f(widMult * 2 + hShift, winH / 2.0 + vertSpacing), Vec2f(1.0 , -1.0), 5, 1.1 * gPower);
     topOne.mOn = 2;
     bottomOne.mOn = 1;
     
     Force topTwo    = Force( Vec2f(widMult * 3 + hShift, winH / 2.0 - vertSpacing), Vec2f(1.0 , 1.0), 5, 1.1 * gPower);
     Force bottomTwo = Force( Vec2f(widMult * 3 + hShift, winH / 2.0 + vertSpacing), Vec2f(1.0 , -1.0), 5, 1.1 * gPower);
     topTwo.mOn = 1;
     bottomTwo.mOn = 2;
     
     Force topThree    = Force( Vec2f(widMult * 4 + hShift, winH / 2.0 - vertSpacing), Vec2f(1.0 , 1.0), 5, 1.1 * gPower);
     Force bottomThree = Force( Vec2f(widMult * 4 + hShift, winH / 2.0 + vertSpacing), Vec2f(1.0 , -1.0), 5, 1.1 * gPower);
     topThree.mOn = 2;
     bottomThree.mOn = 1;
     
     Force topFour    = Force( Vec2f(widMult * 5 + hShift, winH / 2.0 - vertSpacing), Vec2f(1.0 , 1.0), 5, 1.1 * gPower);
     Force bottomFour = Force( Vec2f(widMult * 5 + hShift, winH / 2.0 + vertSpacing), Vec2f(1.0 , -1.0), 5, 1.1 * gPower);
     topFour.mOn = 1;
     bottomFour.mOn = 2;
     
     Force topRight    = Force( Vec2f(widMult * 6 + hShift, winH / 2.0 - vertSpacing - 50), Vec2f(1.0 , 1.0), 5, 1.1 * gPower);
     Force bottomRight = Force( Vec2f(widMult * 6 + hShift, winH / 2.0 + vertSpacing + 50), Vec2f(1.0 , -1.0), 5, 1.1 * gPower);
     topRight.mOn = 2;
     bottomRight.mOn = 1;
     
     mForces.push_front( centerLeft );
     
     mForces.push_front( topLeft );
     mForces.push_front( bottomLeft );
     
     mForces.push_front( topOne );
     mForces.push_front( bottomOne );
     
     mForces.push_front( topTwo );
     mForces.push_front( bottomTwo );
     
     mForces.push_front( topThree );
     mForces.push_front( bottomThree );
     
     mForces.push_front( topFour );
     mForces.push_front( bottomFour );
     
     mForces.push_front( topRight );
     mForces.push_front( bottomRight );
     
}