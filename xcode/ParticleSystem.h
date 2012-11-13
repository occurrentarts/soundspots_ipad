/*
 *  ParticleSystem.h
 *
 *  Created by Mehmet Akten on 02/05/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */
#pragma once

#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "Particle.h"
#include "cinder/Vector.h"
#include "cinder/Surface.h"

#define MAX_PARTICLES		5 * 5500
#define TRAIL_LENGTH        2

class ParticleSystem {

    public:	
        
        float posArray[MAX_PARTICLES * TRAIL_LENGTH * 2]; 
        float colArray[MAX_PARTICLES * TRAIL_LENGTH * 4];
        ci::Vec2i	windowSize;
        ci::Vec2f	invWindowSize;
        const ciMsaFluidSolver	*solver;
        
        int curIndex;
        int pDetail;
        
        Particle particles[MAX_PARTICLES];
        
        ParticleSystem();
        void setFluidSolver( const ciMsaFluidSolver *aSolver ) { solver = aSolver; }
        
    void updateAndDraw( const ci::Perlin &perlin, int _pDetail, ci::Surface &bgSurface, int colMode, ci::ColorA _color , ci::Vec3f _accel );
        void addParticles( const ci::Vec2f &pos, int count );
        void addParticle( const ci::Vec2f &pos );
        void setWindowSize( ci::Vec2i winSize );
        void changeRadius( int _rad );
        void changeParticleColor( float _r, float _g, float _b );
        void applyAccel( ci::Vec3f _accel );
        int getNum();
};




