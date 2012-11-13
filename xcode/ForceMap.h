//
//  ForceMap.h
//  This class allows you to place forces in a savable configuration.
//
//  Created by amun on 9/28/11.
//  Copyright 2011 Occurrent Arts. All rights reserved.
//

#include "Force.h"
#include <list>

class ForceMap {
    public:	
        std::list<Force> mForces;
        
        void init();
        void update( ciMsaFluidSolver* fluidSolver );
        void changePower( float _pow );
        void initCenterStream();
        float gPower;
    
};