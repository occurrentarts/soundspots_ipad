#pragma once

#include "cinder/Perlin.h"
#include "ciMsaFluidSolver.h"
#include "cinder/Vector.h"
#include "cinder/Surface.h"
#include <list>

class Particle {
  public:	
    ci::Vec2f	pos, vel;
    std::list<ci::Vec2f> mPosHistory;
    
    float		alpha;
    float		mass;
    int         mCounter;
    float       redMult, greenMult, blueMult;
    
    float		mRadius;
	int			mAge;
    int			mLifespan;
    float		mAgePer;
    float       mScale;
    int         mDetail;
    
    const int   VELOCITY   = 1;
    const int   BACKGROUND = 0;
        
    void init(float x, float y, int detail);
    void update( const ci::Perlin &perlin, const ciMsaFluidSolver &solver, const ci::Vec2f &windowSize, const ci::Vec2f &invWindowSize, ci::Vec3f _accel );
	void updateVertexArrays( const ci::Vec2f &invWindowSize, int i, float* posBuffer, float* colBuffer, ci::Surface &bgSurface, int colMode );
    void changeDetail( int _detail );
    void changeColor( float _r, float _g, float _b);
    void applyAccel( ci::Vec3f _accel );
    
};

