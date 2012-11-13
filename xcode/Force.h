//
//  Force.h
//  msaFluidParticles
//
//  Created by Andrew Schoneweis on 9/27/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef msaFluidParticles_Force_h
#define msaFluidParticles_Force_h

//
//  Force.h
//  energyFlow
//
//  Created by amun on 9/26/11.
//  Copyright 2011 Occurrent Arts. All rights reserved.
//

#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "Particle.h"

using namespace ci;

#define MAX_FORCE_THRESHOLD 50000 //  300 x 300 pixels away

class Force {
    
public:
    Force();
    Force( Vec2f _pos, Vec2f _dir, int _size, float _power );
    Force( Vec2f _pos, int _type, Vec2f _dir, int _size, float _power );
   
    void init( Vec2f _pos, Vec2f _direction, int _size, float _power );
    
    void applyForce( ciMsaFluidSolver*	_fluidSolver );
    void applyLinearForce( ciMsaFluidSolver*	_fluidSolver);
    void applyClockwiseRotationForce( ciMsaFluidSolver*	_fluidSolver );
    void applyCounterClockwiseRotationForce( ciMsaFluidSolver*	_fluidSolver );
    void applyClockwiseInwardForce( ciMsaFluidSolver*	_fluidSolver );
    void applyCounterClockwiseInwardForce( ciMsaFluidSolver*	_fluidSolver );
    void draw();
    
    static const int LINEAR = 0;
    static const int COUNTER_CLOCKWISE_INWARDS = -2;
    static const int COUNTER_CLOCKWISE_OUTER = 2;
    static const int CLOCKWISE_INWARDS = -1;
    static const int CLOCKWISE_OUTER = 1;
    
    Vec2f mPos, mAngle;
    float mPower;
    int mSize, mOn; // -1 == reverse , 0 == stopped, 1 == on
    
};

#endif
