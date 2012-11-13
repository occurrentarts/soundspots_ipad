/*
 *  ParticleSystem.cpp
 *
 *  Created by Mehmet Akten on 02/05/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "ParticleSystem.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/app/App.h"

using namespace ci;

ParticleSystem::ParticleSystem() 
{
	curIndex = 0;
	setWindowSize( Vec2i( 1, 1 ) );
}

void ParticleSystem::setWindowSize( Vec2i winSize )
{
	windowSize = winSize;
	invWindowSize = Vec2f( 1.0f / winSize.x, 1.0f / winSize.y );
}

void ParticleSystem::updateAndDraw( const Perlin &perlin, int _pDetail, Surface &bgSurface, int colMode, ColorA colorMult, Vec3f _accel ){
    
	
    
    glEnable(GL_BLEND);
	//glDisable( GL_TEXTURE_2D );
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_LINE_SMOOTH);    
    
    int numActive = 0;
	
    //update vertex arrays
	for(int i=0; i < MAX_PARTICLES; i++) {
        
        
		if(particles[i].alpha > 0) {
                     
			particles[i].update( perlin, *solver, windowSize, invWindowSize, _accel );
            particles[i].changeColor(colorMult.r, colorMult.g, colorMult.b);
			particles[i].updateVertexArrays( invWindowSize, i, posArray, colArray, bgSurface, colMode );
            numActive++;
		}
        
	}    
    
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, posArray);
	
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, colArray);
    
    if ( pDetail != _pDetail) {
        pDetail = _pDetail;
        
        for(int i=0; i < MAX_PARTICLES; i++) {
            //particles[i].alpha = 0;
            particles[i].changeDetail( pDetail );
        }
    }  
    
    if( pDetail == 1) glDrawArrays(GL_TRIANGLE_STRIP, 0, numActive * pDetail);
    else {
        for(int i = 0; i < numActive * pDetail; i += pDetail) {
            
                
            if(pDetail > 2) glDrawArrays(GL_TRIANGLE_FAN, i, pDetail);
            else glDrawArrays(GL_LINES, i, pDetail);
            
        }
    }
    
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
    
    glDisable(GL_BLEND);
    
    
    /** Immediate Mode
    for(int i = 0; i < ( MAX_PARTICLES * 2); i += TRAIL_LENGTH) {
        //glDrawArrays(GL_LINE_STRIP, i, TRAIL_LENGTH);
        
        glBegin( GL_LINE_STRIP );
        glColor4f( 1.0, 1.0, 1.0, 1.0 );
        for( int j = 0; j < TRAIL_LENGTH; j += 2 ){
            glVertex2f( Vec2f( posArray[j], posArray[j+1] ) );
            app::console() << "drawing ." << posArray[j] << " " << posArray[j+1] << std::endl;
            
        }
        glEnd();
        
    }
    **/
}

int ParticleSystem::getNum() {
    
    int numActive = 0;
    for(int i=0; i<MAX_PARTICLES; i++) {
		if (particles[i].alpha > 0 )
            numActive++;
    }
    return numActive;
    
}

void ParticleSystem::changeRadius( int _rad ) {
    for(int i=0; i<MAX_PARTICLES; i++) {
        
		particles[i].mRadius = _rad * 2 ;
                
    }
}

void ParticleSystem::changeParticleColor( float _r, float _g, float _b ) {
    for(int i=0; i<MAX_PARTICLES; i++) {
        
		particles[i].changeColor(_r,_g,_b);
        
    }
}

void ParticleSystem::applyAccel( Vec3f _accel ) {
    for(int i=0; i<MAX_PARTICLES; i++) {
        
		particles[i].applyAccel( _accel );
        
    }
}

void ParticleSystem::addParticles( const Vec2f &pos, int count ){
	for(int i=0; i<count; i++)
		addParticle( pos );
}

void ParticleSystem::addParticle( const Vec2f &pos ) {
    
	particles[curIndex].init( pos.x, pos.y, pDetail  );
	curIndex++;
    //app::console() << curIndex << std::endl;
	if(curIndex >= MAX_PARTICLES / pDetail ) {
        curIndex = 0;
    }
    
}
