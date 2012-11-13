/*
 *  Particle.cpp
 *
 *  Created by Mehmet Akten on 02/05/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "Particle.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"

static const float MOMENTUM = 0.992f;
static const float FLUID_FORCE = 0.4f;

#define TRAIL_LENGTH        2

using std::list;
using namespace std;
using namespace ci;

void Particle::init( float x, float y, int _detail ) {
	pos = Vec2f( x, y );
	vel.x = vel.y = 0;
	alpha  = Rand::randFloat( 0.5f, 1 );
    //mCounter = 0;
	mass = Rand::randFloat( 0.1f, 1 );
    
    mAgePer			= 1.0f;
    mAge			= 0;
	mLifespan		= Rand::randInt( 10, 1100 );
    mRadius			= Rand::randInt( 6, 15 );
    
    mScale			= 3.0f;
    mDetail         = _detail;
    
    //mColorHandle = colorHandle;
    
    redMult = 0.4; //mColorHandle.r;
    greenMult =  0.4; //mColorHandle.g;
    blueMult =  0.4;
    
    for (int i = 0; i < TRAIL_LENGTH; i++) {
        mPosHistory.push_front( pos );
    }
    
}

void Particle::update( const Perlin &perlin, const ciMsaFluidSolver &solver, const Vec2f &windowSize, const Vec2f &invWindowSize, Vec3f _accel ){
    
	// only update if particle is visible
	if( alpha == 0 )
		return;
    
    //mCounter++;
    mPosHistory.push_front( pos );
    
    while( mPosHistory.size() > TRAIL_LENGTH ) {
        mPosHistory.pop_back();
    }
    
    //vel *= 1.08;// - mRadius/15.0;
    //vel.y = 8.0f;
    
    vel = solver.getVelocityAtPos( pos * invWindowSize) * (mass * FLUID_FORCE ) * windowSize + vel * MOMENTUM;
	//vel = solver.getVelocityAtPos( pos ); // * (mass * FLUID_FORCE ) * windowSize + vel * MOMENTUM;
    
    applyAccel( _accel );
    
    /**
    float nX = pos.x * 0.002f;
	float nY = pos.y * 0.002f;
	float nZ = app::getElapsedSeconds() * 0.06f; // fade out
	
    float noise = perlin.fBm( nX, nY, nZ );
	float angle = noise * 1.0f;
	Vec2f noiseVector( cos( angle ), sin( angle ) );
    vel += noiseVector * 0.35f * ( 1.0f - mAgePer );
    **/
    
    //mRadius	= sin(mAge * 3.9) * 8;
    
    //alpha = sin( mAge * 3.9 );
    
    //mRadius += sin( cinder::app::getElapsedSeconds() );
    
    /**
    mRadius	-= 0.01;
    if ( mRadius <= 0.0 ) {
        alpha = 0.0;
    }
    **/
    
	/**
    Vec3f deriv = Vec3f( cos(M_PI/5), sin(M_PI/5), mCounter) * .000001;
    Vec2f deriv2( deriv.x, deriv.y );
    deriv2.normalize();
    vel *= deriv2 * Rand::randFloat(1.8,2.0);
    **/
    
    //app::console() << "vel: " << vel.x << " " << vel.y << std::endl;
    pos += vel;
    
    // hackish way to make particles glitter when the slow down a lot
	if( vel.lengthSquared() < 1 ) {
		vel += Rand::randVec2f() * 0.5f;
	}
    
    int velBoundery = 15;
    if (vel.x > velBoundery || vel.x < -1 * velBoundery) vel.x *= .8;
    //else { vel.x *= 2.001; }
    
    if (vel.y > velBoundery || vel.y < -1 * velBoundery) vel.y *= .8;
    //else { vel.y *= 2.001; }
    
    
    int xChng, yChng;
    
    if( mDetail != 1 ) {
        xChng = 35; yChng = 35;
    } else  {
        xChng = 1536 / 6; 
        yChng = 1536 / 6;
    }
    
    
	if ( pos.y < 0 - yChng) {
		pos.y = windowSize.y;
        vel *= .1;
        for( int i = 0; i < TRAIL_LENGTH; i++ ) {
            mPosHistory.pop_back();
            mPosHistory.push_front( Vec2f( pos.x , pos.y) );
        }
		//vel.y *= -0.88;
	} else if( pos.y > windowSize.y  + yChng ) {
		pos.y = 0;
        vel *= .1;
        for( int i = 0; i < TRAIL_LENGTH; i++ ) {
            mPosHistory.pop_back();
            mPosHistory.push_front( Vec2f( pos.x , pos.y) );
        }
		//vel.y *= -0.88;
	}
	
	if ( pos.x < 0 - xChng ) {
        
		pos.x = windowSize.x;
        for( int i = 0; i < TRAIL_LENGTH; i++ ) {
            mPosHistory.pop_back();
            mPosHistory.push_front( Vec2f( pos.x , pos.y) );
        }
        //vel.x *= 0.5;
            
	} else if( pos.x > 1536 + xChng ) { //windowSize.x
		
        pos.x = 0.0f;
        //pos.y = Rand::randFloat(200,600);
        for( int i = 0; i < TRAIL_LENGTH; i++ ) {
            mPosHistory.pop_back();
            mPosHistory.push_front( Vec2f( pos.x , pos.y) );
        }
        vel *= .1;
        
	}
    
    mAge++;
    mAgePer = 1.0f - ( (float)mAge/(float)mLifespan );
    
    //vel *= .95;
    
	// fade out a bit (and kill if alpha == 0);
    
    /**
	alpha *= 0.999f;
    
	if( alpha < 0.2f ) {
        
        alpha = 0;
        
        alpha = .999;
        pos.x = 0.0f;
        for( int i = 0; i < TRAIL_LENGTH; i++ ) {
            mPosHistory.pop_back();
            mPosHistory.push_front( Vec2f( pos.x , pos.y) );
        }
    }
    **/
    
    //app::console() << "counter: " << mCounter << std::endl;
}

void Particle::changeDetail( int _detail ) {
    mDetail = _detail;
}

void Particle::changeColor( float _r, float _g, float _b ) {
    redMult = _r;
    greenMult = _g;
    blueMult = _b;
}

void Particle::applyAccel( Vec3f _accel ) {
    vel.x += _accel.x * 10.5;
    vel.y -= _accel.y * 10.5;
}

void Particle::updateVertexArrays( const Vec2f &invWindowSize, int i, float* posBuffer, float* colBuffer, Surface &bgSurface, int colMode ) {
    
    int vi = i * mDetail * 2; 
    int ci = i * mDetail * 4;
    
    ColorA particleColor;
    
    //white
    //particleColor = ColorA( 1.0f,  1.0f,  1.0f, 0.7f); 
    
    //black
    //particleColor = ColorA( 0.0f,  0.0f,  0.0f, 0.7f); 
    
    if (colMode == 0) {
        //background
        particleColor = (bgSurface).getPixel( pos );
    } else if (colMode == 1) {
        //velocity
        //particleColor = ColorA( 0.9f, .2 + ( 0.03 * vel.y), .2 + ( 0.01 * vel.x), alpha );
        particleColor = ColorA( redMult, greenMult + ( 0.03 * vel.y), blueMult+ ( 0.01 * vel.x), alpha );
    } else  {
        //defined
        particleColor = ColorA( redMult,  greenMult,  blueMult, 0.7f);
    }
        
    if ( mDetail > 2 ) {
    
        list<Vec2f>::iterator positions = mPosHistory.begin();
        for(  int n = mDetail; n > 0; n-- ){
            
            posBuffer[vi++] = positions->x + cos(n * M_PI * 2 / mDetail) * mRadius;
            posBuffer[vi++] = positions->y  + sin(n * M_PI * 2 / mDetail) * mRadius;
            //app::console() << "poz: " << posBuffer[vi - 1] << " " << posBuffer[vi] << std::endl;
            
            colBuffer[ci++] = particleColor.r;
            colBuffer[ci++] = particleColor.g;
            colBuffer[ci++] = particleColor.b;
            colBuffer[ci++] = particleColor.a;

        }

    } else {
    
    
        for( list<Vec2f>::iterator positions = mPosHistory.begin(); positions != mPosHistory.end(); ++positions ){
            posBuffer[vi++] = positions->x;
            posBuffer[vi++] = positions->y;
            //app::console() << "poz: " << posBuffer[vi - 1] << " " << posBuffer[vi] << std::endl;
            
            colBuffer[ci++] = particleColor.r;
            colBuffer[ci++] = particleColor.g;
            colBuffer[ci++] = particleColor.b;
            colBuffer[ci++] = particleColor.a;
        }
        
    }
    
        
}
