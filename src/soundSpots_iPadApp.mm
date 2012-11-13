#include "cinder/Cinder.h"

#include "cinder/app/AppCocoaTouch.h"
#include "cinder/app/Renderer.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"
#include "cinder/ImageIo.h"

#include "cinder/Rand.h"

#include "cinder/Perlin.h"
#include "ciMsaFluidSolver.h"
#include "ciMsaFluidDrawerGl.h"
#include "cinder/gl/Fbo.h"

#include "Particle.h"
#include "ParticleSystem.h"
#include "Force.h"
#include "ForceMap.h"

#include "cinder/audio/Input.h"
#include "SimpleGUI.h"

#include <iostream>
#include <vector>

#define SCREEN_WIDTH 1536
#define SCREEN_HEIGHT 2048
#define CONFIG_FILE "settings.sgui.txt"
#define RENDER_TYPE_GROUP 1

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace mowa::sgui;

class soundSpots_iPadApp : public AppCocoaTouch {
  public:
    
    void	prepareSettings( Settings *settings );
    
    void	fadeToColor( float r, float g, float b, float speed );
    void	addToFluid( Vec2f pos, Vec2f vel, bool addColor, bool addForce, bool addParticles );
    void	touchesBegan( TouchEvent event );
	void	touchesMoved( TouchEvent event );
    void	touchesEnded( TouchEvent event );
    void    drawWaveForm( float height );
    void    initGUI( bool display );
    
    Vec3f   currentAccel;
    
	virtual void        setup();
	virtual void        resize( ResizeEvent event );
	virtual void        update();
	virtual void        draw();
    virtual void        accelerated( AccelEvent event );
    
    int					fluidCellsX;
	bool				resizeFluid;
    bool                moveMode;
    bool                drawParticles;
    bool                drawFluid;
    bool                enableAccel;
    bool                updateSolver;
    
    int                 pDetail;
    Vec2i				pMouse;
    
    bool                displayGUI;
    
    SimpleGUI* gui;
	PanelControl* strokePanel;
    
    ciMsaFluidSolver	fluidSolver;
	ciMsaFluidDrawerGl	fluidDrawer;

	ParticleSystem		particleSystem;
    ForceMap            forceField;
    Perlin              mPerlin;
    
    audio::Input mInput;
	std::shared_ptr<float> mFftDataRef;
	audio::PcmBuffer32fRef mPcmBuffer;
    
    ColorA        bgColor, parColor;
    int           index, minParticles;
    float         micScalar;
    float         gain, prevGain;
    Surface       bgSurface; 
    bool          drawBGImage;
    bool          useAccel;
    bool          applyCenterForce;
    
    //color mode
    bool          velocityColorMode;
    bool          backgroundColorMode;
    bool          definedColorMode;
    int           colorMode;
    
    protected:
    
        gl::Fbo			mFboScene;
        gl::Texture     bgImage;
        


};

void soundSpots_iPadApp::prepareSettings( Settings *settings )
{
	//settings->enableMultiTouch( true );
    settings->enableMultiTouch();
}

void soundSpots_iPadApp::setup()
{
    
    enableAccelerometer();
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    
    fluidCellsX			= 100;
    fluidSolver.setup( 100, 150);
    fluidSolver.enableRGB(false).setFadeSpeed(0.08).setDeltaT(0.65).setVisc(0.0001).enableVorticityConfinement(false).setColorDiffusion(0).setWrap(true,true);
    fluidSolver.setSize( fluidCellsX, fluidCellsX * getWindowAspectRatio() );
	fluidDrawer.setup( &fluidSolver );
    fluidDrawer.setDrawMode(FLUID_DRAW_VECTORS);
	particleSystem.setFluidSolver( &fluidSolver );
	particleSystem.setWindowSize( Vec2f( SCREEN_WIDTH, SCREEN_HEIGHT) );
    
    bgSurface = loadImage( loadResource("flower2.jpg") );
    bgImage = gl::Texture( bgSurface );
    
    forceField = ForceMap();
    forceField.init();
	
	mPerlin             = Perlin();
    moveMode            = true;
	setFrameRate( 60.0f );
	
    pMouse = getWindowCenter();
	resizeFluid			= false;
    drawParticles		= true;
    drawFluid			= true;
    updateSolver        = true;
    
    drawBGImage         = false;
    useAccel            = false;
    applyCenterForce    = true;
    
    velocityColorMode   = true;
    backgroundColorMode = false;
    definedColorMode    = false;
    colorMode           = 1;
    
    bgColor = ColorA( 0,0,0,1 );
    parColor = ColorA( .4,.4,.4,.7 );
    
    index       = 0;
    micScalar   = 25.0;
    gain        = 0.0f;
    prevGain    = 0.0f;
    minParticles = 3000;
    pDetail        = 2;
    
    //init gui, default display turned off
    initGUI( false );
	
	gl::enableAlphaBlending();
    glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    
    // setup our Fbo's
	mFboScene = gl::Fbo(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    //iterate input devices and print their names to the console
	const std::vector<audio::InputDeviceRef>& devices = audio::Input::getDevices();
	for( std::vector<audio::InputDeviceRef>::const_iterator iter = devices.begin(); iter != devices.end(); ++iter ) {
		console() << (*iter)->getName() << std::endl;
	}
    
	//initialize the audio Input, using the default input device
	mInput = audio::Input();
	
	//tell the input to start capturing audio
	mInput.start();
        
}

void soundSpots_iPadApp::initGUI(bool _display)
{
    displayGUI = _display;
    gui = new SimpleGUI(this);
	gui->lightColor = ColorA(1, 1, 0, 1);	
	gui->addColumn();
	gui->addLabel("Play With These To Change the Particles:");
	
    //particle details
	gui->addParam("Mic Sensitivity", &micScalar, 0.0, 100.0, 25.0);
 	gui->addParam("Particle Detail", &pDetail, 1, 5, 5); 	
    
    gui->addLabel("Color Mode:");
	gui->addParam("Background", &backgroundColorMode, 0, RENDER_TYPE_GROUP); 
	gui->addParam("Velocity", &velocityColorMode, 1, RENDER_TYPE_GROUP); 
    gui->addParam("Uniform", &definedColorMode, 0, RENDER_TYPE_GROUP);
    gui->addParam("Particle Color ( RGB )", &parColor, ColorA(.4,.4,.4), SimpleGUI::RGB); //use R,G,B,A sliders
    
    
    //gui->addColumn();
    gui->addLabel("Play With These To Change the Background:");
    
    //force details
    gui->addParam("Apply Central Force", &applyCenterForce, true); //use R,G,B,A sliders
    gui->addParam("Accelerometer On", &useAccel, false); //use R,G,B,A sliders
    
    //background details
    gui->addParam("Draw BG Image", &drawBGImage, false); //use R,G,B,A sliders
    gui->addParam("Background Color ( RGB )", &bgColor, ColorA(0,0,0), SimpleGUI::RGB); //use R,G,B,A sliders
    gui->load(CONFIG_FILE);
    gui->setEnabled(displayGUI);
        
}

void soundSpots_iPadApp::fadeToColor( float r, float g, float b, float speed )
{
	glColor4f( r, g, b, speed );
	gl::drawSolidRect( getWindowBounds() );
}

void soundSpots_iPadApp::resize( ResizeEvent event )
{
	resizeFluid = true;
}

void soundSpots_iPadApp::update()
{
    
    if (applyCenterForce)
        forceField.update( &fluidSolver );

    if ( particleSystem.getNum() < MAX_PARTICLES / 5 ) 
        addToFluid( Vec2f( randFloat() , randFloat() ), Vec2f( 0.0, 20.0 ), false, false, true );
    
    if( resizeFluid ) {
		fluidSolver.setSize(fluidCellsX, fluidCellsX / getWindowAspectRatio() );
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
		fluidSolver.randomizeColor();
	}
	
    mPcmBuffer = mInput.getPcmBuffer();
	if( ! mPcmBuffer ) {
		return;
	}
    
    if( pDetail > 2) {
        if (index > 2) {
            fluidSolver.update();
            index = 0;
        } else {
            index++;
        }
    } else {
        fluidSolver.update();
    }
    
}








void soundSpots_iPadApp::draw()
{    
    
    
    if (velocityColorMode) {
        colorMode = 1;
    } else if ( backgroundColorMode ) {
        colorMode = 0;
    } else {
        colorMode = 2;
    }
    
    //gl::clear( Color::white() );
    
    float waveFormHeight = getWindowWidth() / 2;
    drawWaveForm( waveFormHeight );
    
    //fluidDrawer.draw(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
    gl::clear( bgColor );
    
    
    particleSystem.updateAndDraw( mPerlin, pDetail, bgSurface, colorMode, parColor, currentAccel );

    if (drawBGImage) {
        bgImage.enableAndBind();
        gl::draw( bgImage, getWindowBounds() );
        bgImage.disable();
    }
    
    if (displayGUI) gui->draw();

}









void soundSpots_iPadApp::addToFluid( Vec2f pos, Vec2f vel, bool addColor, bool addForce, bool addParticles )
{
    float speed = vel.x * vel.x  + vel.y * vel.y * getWindowAspectRatio() * getWindowAspectRatio();    
    // balance the x and y components of speed with the screen aspect ratio
    
    if( speed > 0 ) {
		pos.x = constrain( pos.x, 0.0f, 1.0f );
		pos.y = constrain( pos.y, 0.0f, 1.0f );
		
        const float colorMult = 100;
        const float velocityMult = 10;
		
		if( addColor ) {
			Color drawColor( CM_HSV, ( getElapsedFrames() % 360 ) / 360.0f, 1, 1 );
			fluidSolver.addColorAtPos( pos, drawColor * colorMult );
		}
        
        if( addParticles ) {
            particleSystem.addParticles( pos * Vec2f( SCREEN_WIDTH, SCREEN_HEIGHT ), 150 );
        }
        
		if( addForce )
			fluidSolver.addForceAtPos( pos, vel * velocityMult );
		
        
		if( !drawFluid && getElapsedFrames()%10==0 )
            fadeToColor( 0, 0, 0, 0.1f );
    }
}








void soundSpots_iPadApp::touchesBegan( TouchEvent event )
{
	for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
		Vec2f mouseNorm = Vec2f( touchIt->getPos() ) / getWindowSize();
		Vec2f mouseVel = Vec2f( touchIt->getPos() - touchIt->getPrevPos() ) / getWindowSize();
		//addToFluid( mouseNorm, mouseVel, touchIt->getId(), true, true );
        addToFluid( mouseNorm, mouseVel, false, true, true );
	}
}






void soundSpots_iPadApp::touchesMoved( TouchEvent event )
{
    
    //more than one touch
    if ( event.getTouches().size() > 1 ) {
        
        //addToFluid( mouseNorm, mouseVel, false, true, false );
        
        Vec2f thisCursor, curVel;
        
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
            thisCursor = Vec2f( touchIt->getPos() );
            curVel = Vec2f( touchIt->getPos() - touchIt->getPrevPos() );
            
            Force tmp = Force( thisCursor, curVel, 5, 28.2);//450.0 * (*cursor).getSize().x );
            tmp.mOn = -2;
            tmp.applyForce( &fluidSolver );
        }
        
    } else { // one touch
        
        Vec2f thisCursor, curVel;
        
        for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
            thisCursor = Vec2f( touchIt->getPos() );
            curVel = Vec2f( touchIt->getPos() - touchIt->getPrevPos() );
            
            Force tmp = Force( thisCursor, curVel, 5, 28.2);//450.0 * (*cursor).getSize().x );
            tmp.mOn = -2;
            tmp.applyForce( &fluidSolver );
        }
        
    }
    
    updateSolver = true;
}





void soundSpots_iPadApp::touchesEnded( TouchEvent event ) {
    
    std::vector<TouchEvent::Touch>::const_iterator touchIter = event.getTouches().begin();
    if ( [(UITouch*)touchIter->getNative() tapCount] == 2 ) {
        displayGUI = !displayGUI;
        gui->setEnabled(displayGUI);
    }
    
    updateSolver = false;
    
}





void soundSpots_iPadApp::accelerated( AccelEvent event )
{
    if ( useAccel == true )
        currentAccel = event.getData();
    else
        currentAccel = Vec3f ( 0.0, 0.0, 0.0 );
}





void soundSpots_iPadApp::drawWaveForm( float height )
{
	if( ! mPcmBuffer ) {
		return;
	}
	
	uint32_t bufferSamples = mPcmBuffer->getSampleCount();
	audio::Buffer32fRef leftBuffer = mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_LEFT );
	//audio::Buffer32fRef rightBuffer = mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_RIGHT );
    
	int displaySize = getWindowWidth();
	int endIdx = bufferSamples;
	
	//only draw the last 1024 samples or less
	int32_t startIdx = ( endIdx - 1024 );
	startIdx = math<int32_t>::clamp( startIdx, 0, endIdx );
	
	float scale = displaySize / (float)( endIdx - startIdx );
	int baseLine = SCREEN_HEIGHT / 2 - 100;
    prevGain = gain;
    gain = 0.0;
    
	//PolyLine<Vec2f>	line;
	//gl::color( Color( 1.0f, 0.5f, 0.25f ) );
	for( uint32_t i = startIdx, c = 0; i < endIdx; i++, c++ ) {
		float y = ( ( leftBuffer->mData[i] - 1 ) * - 100 ) + baseLine;
        
		//line.push_back( Vec2f( ( c * scale ), y ) );
        
        if ( leftBuffer->mData[i] > gain ) gain = leftBuffer->mData[i];
	}
	//gl::draw( line );
    //float gainChange = (gain - prevGain);
    int sizeMin = 2;
    
    
    if( pDetail != 2 ) {
        particleSystem.changeRadius(int( gain * micScalar + sizeMin ));
    } else {
        glLineWidth( gain * micScalar * 2 );
    }
    //} else {
        //gainChange *= .5;
        //particleSystem.changeRadius(int( gainChange * micScalar + sizeMin ));
    //}
    
    forceField.changePower(gain * 112.0);
    
}


CINDER_APP_COCOA_TOUCH( soundSpots_iPadApp, RendererGl )
