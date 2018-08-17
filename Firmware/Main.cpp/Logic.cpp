#include "InstrumentsManager.h"
#include "RythmManager.h"
#include "DisplayManager.h"
#include "Logic.h"

static int flagForceScreenUpdate;

void logic_init(void)
{
  flagForceScreenUpdate=0;
}

void logic_loop(void)
{
    rthm_loop();
    inst_loop();
    display_loop();

    // Update screen after all instruments are finished
    if(rthm_isNewStepFinished()==1 || flagForceScreenUpdate==1)
    {  
      // Instruments just started to be played
      // wait until they finish
      if(inst_areAllInstrumentsIdle())
      {
            display_update(); // once all instruments are idle, refresh the screen
            rthm_resetNewStepFinishedFlag();
            flagForceScreenUpdate=0;
      }
    }
    //_________________________________________________
    
}



void logic_forceUpdateScreen(void)
{
    flagForceScreenUpdate=1;
}


