//  Force.cpp
//  energyFlow
//
//  Created by amun on 9/26/11.
//  Copyright 2011 Occurrent Arts. All rights reserved.
//

#include "Force.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Vector.h"
#include <iostream>

using namespace std;
using namespace ci;

#define WID 1536
#define HEI 2048

Force::Force() {
    
}

//default linear constructor
Force::Force( Vec2f _pos, Vec2f _direction, int _size, float _power ) {
    
    Force::init(_pos, _direction, _size, _power );
    mOn = 0;
    
}

//define type
Force::Force( Vec2f _pos, int _type, Vec2f _direction, int _size, float _power ) {
    
    Force::init(_pos, _direction, _size, _power );
    mOn = _type;
    
}

void Force::init( Vec2f _pos, Vec2f _direction, int _size, float _power ) {
    mPos = _pos;
    mAngle = _direction; 
    mSize = _size;
    mPower = _power; // value b/w 0-1
}

void Force::applyForce( ciMsaFluidSolver* fluidSolver ) {
    
    switch (mOn) {
        case -2:
            applyCounterClockwiseInwardForce(fluidSolver); 
            break;    
        case -1:
            applyClockwiseInwardForce(fluidSolver); 
            break;
        case 0:
            applyLinearForce(fluidSolver);
            break;
        case 1:
            applyClockwiseRotationForce(fluidSolver);  
            break;
        case 2:
            applyCounterClockwiseRotationForce(fluidSolver);  
            break;    
        default:
            break;
    }
}

void Force::applyLinearForce( ciMsaFluidSolver* fluidSolver ) {
    
    //(*fluidSolver).addForceAtPos( mPos / app::getWindowSize(),  Vec2f( mAngle.x, mAngle.y ) * mPower);
    
    for(int i=1; i <= mSize * 10 ; i++){
        
        float x = 100 * mPos.x/WID + i;
        float y = 150 * mPos.y/HEI;
        
        int index = (*fluidSolver).getIndexForCellPosition(x,y);
        (*fluidSolver).uv[index].x = mAngle.x * mPower/112.0;
        (*fluidSolver).uv[index].y = mAngle.y * mPower/112.0;
        
    }

}

void Force::applyCounterClockwiseRotationForce( ciMsaFluidSolver* fluidSolver ) {
    
    float coils = mSize;
    float rotation = 0;
    float sides = 360;
    
    // How far to rotate around center for each side.
    float aroundStep = coils/sides;// 0 to 1 based.
    
    // Convert aroundStep to radians.
    float aroundRadians = aroundStep * 2 * M_PI;
    
    // Convert rotation to radians.
    rotation *= 2 * M_PI;
    
    float xPrev = 0;
    float yPrev = 0;
    // For every side, step around and away from center.
    for(int i=1; i<=sides; i++){
        
        // How far away from center
        float away = pow(mPower, i/sides);
        
        // How far around the center.
        float around = i * aroundRadians + rotation;
        
        // Convert 'around' and 'away' to X and Y.
        float x = 100 * mPos.x/WID + cos(around) * away;
        float y = 150 * mPos.y/HEI + sin(around) * away;
        
        int index = (*fluidSolver).getIndexForCellPosition(x,y);
        (*fluidSolver).uv[index].x = (xPrev - x)/100;
        (*fluidSolver).uv[index].y = (yPrev - y)/150;
        
        //(*fluidSolver).uv[index].x = (mPos.x/1440 - x/100);
        //(*fluidSolver).uv[index].y = (mPos.y/900 - y/100);
        
        //(*fluidSolver).uv[index].x = (x/100 - mPos.x/1440);
        //(*fluidSolver).uv[index].y = (y/100 - mPos.y/900);
        
        xPrev = x;
        yPrev = y;
    }

    
}

void Force::applyClockwiseInwardForce( ciMsaFluidSolver* fluidSolver ) {
    
    float coils = mSize;
    float rotation = 0;
    float sides = 360;
    
    // How far to rotate around center for each side.
    float aroundStep = coils/sides;// 0 to 1 based.
    
    // Convert aroundStep to radians.
    float aroundRadians = aroundStep * 2 * M_PI;
    
    // Convert rotation to radians.
    rotation *= 2 * M_PI;
    
    float xPrev = mPos.x;
    float yPrev = mPos.y;
    // For every side, step around and away from center.
    for(int i=1; i<=sides; i++){
        
        // How far away from center
        float away = pow(mPower, i/sides);
        
        // How far around the center.
        float around = i * aroundRadians + rotation;
        
        // Convert 'around' and 'away' to X and Y.
        float x = 100 * mPos.x/WID + cos(around) * away;
        float y = 150 * mPos.y/HEI + sin(around) * away;
        
        int index = (*fluidSolver).getIndexForCellPosition(x,y);  
        (*fluidSolver).uv[index].x = (mPos.x/WID - xPrev/100);
        (*fluidSolver).uv[index].y = (mPos.y/HEI - yPrev/150);
        
        xPrev = x;
        yPrev = y;
    }
    
    
}


void Force::applyClockwiseRotationForce( ciMsaFluidSolver* fluidSolver ) {
    
    float coils = mSize;
    float rotation = 0;
    float sides = 360;
    
    // How far to rotate around center for each side.
    float aroundStep = coils/sides;// 0 to 1 based.
    
    // Convert aroundStep to radians.
    float aroundRadians = aroundStep * 2 * M_PI;
    
    // Convert rotation to radians.
    rotation *= 2 * M_PI;
    
    float xPrev = 0;
    float yPrev = 0;
    // For every side, step around and away from center.
    for(int i=1; i<=sides; i++){
        
        // How far away from center
        float away = pow(mPower, i/sides);
        
        // How far around the center.
        float around = i * aroundRadians + rotation;
        
        // Convert 'around' and 'away' to X and Y.
        float x = 100 * mPos.x/WID + cos(around) * away;
        float y = 150 * mPos.y/HEI + sin(around) * away;
        
        int index = (*fluidSolver).getIndexForCellPosition(x,y);
        (*fluidSolver).uv[index].x = (xPrev - x)/100 * -1;
        (*fluidSolver).uv[index].y = (yPrev - y)/150 * -1;
        
        //(*fluidSolver).uv[index].x = (mPos.x/1440 - x/100);
        //(*fluidSolver).uv[index].y = (mPos.y/900 - y/100);
        
        //(*fluidSolver).uv[index].x = (x/100 - mPos.x/1440);
        //(*fluidSolver).uv[index].y = (y/100 - mPos.y/900);
        
        xPrev = x;
        yPrev = y;
    }
    
    
}

void Force::applyCounterClockwiseInwardForce( ciMsaFluidSolver* fluidSolver ) {
    
    float coils = mSize;
    float rotation = 0;
    float sides = 360;
    
    // How far to rotate around center for each side.
    float aroundStep = coils/sides;// 0 to 1 based.
    
    // Convert aroundStep to radians.
    float aroundRadians = aroundStep * 2 * M_PI;
    
    // Convert rotation to radians.
    rotation *= 2 * M_PI;
    
    float xPrev = mPos.x;
    float yPrev = mPos.y;
    // For every side, step around and away from center.
    for(int i=1; i<=sides; i++){
        
        // How far away from center
        float away = pow(mPower, i/sides);
        
        // How far around the center.
        float around = i * aroundRadians + rotation;
        
        // Convert 'around' and 'away' to X and Y.
        float x = 100 * mPos.x/WID + cos(around) * away;
        float y = 150 * mPos.y/HEI + sin(around) * away;
        
        int index = (*fluidSolver).getIndexForCellPosition(x,y);  
        (*fluidSolver).uv[index].x = (mPos.x/WID - xPrev/100);
        (*fluidSolver).uv[index].y = (mPos.y/HEI - yPrev/150);
        
        xPrev = x;
        yPrev = y;
    }
    
    
}