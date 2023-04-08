//
//  EffectPlugin.cpp
//  MyEffect Plugin Source Code
//
//  Used to define the bodies of functions used by the plugin, as declared in EffectPlugin.h.
//

#include "EffectPlugin.h"

////////////////////////////////////////////////////////////////////////////
// EFFECT - represents the whole effect plugin
////////////////////////////////////////////////////////////////////////////

// Called to create the effect (used to add your effect to the host plugin)
extern "C" {
    CREATE_FUNCTION createEffect(float sampleRate) {
        ::stk::Stk::setSampleRate(sampleRate);

        //==========================================================================
        // CONTROLS - Use this array to completely specify your UI
        // - tells the system what parameters you want, and how they are controlled
        // - add or remove parameters by adding or removing entries from the list
        // - each control should have an expressive label / caption
        // - controls can be of different types: ROTARY, BUTTON, TOGGLE, SLIDER, or MENU (see definitions)
        // - for rotary and linear sliders, you can set the range of values (make sure the initial value is inside the range)
        // - for menus, replace the three numeric values with a single array of option strings: e.g. { "one", "two", "three" }
        // - by default, the controls are laid out in a grid, but you can also move and size them manually
        //   i.e. replace AUTO_SIZE with { 50,50,100,100 } to place a 100x100 control at (50,50)

        const Parameters CONTROLS = {
            //  name,       type,              min, max, initial, size

            {   "HP CutOff",  Parameter::ROTARY, 0.0, 1.0, 0.129109339,           { 30,150,40,40 } },
            {   "Gain",  Parameter::SLIDER, -60.0, 18.0, 0.0,           { 90,40,20,85 } },
            {   "FREQ",  Parameter::ROTARY, 0.0, 1.0, 0.179730108,           { 80,150,40,40 } },
            {   "Q",     Parameter::ROTARY, 0.0, 1.0, 0.39,              { 80,210,40,40 } },
            {   "Gain",  Parameter::SLIDER, -60.0, 18.0, 0.0,           { 140,40,20,85 } },
            {   "FREQ",  Parameter::ROTARY, 0.0, 1.0, 0.239360064,           { 130,150,40,40 } },
            {   "Q",     Parameter::ROTARY, 0.0, 1.0, 0.39,              { 130,210,40,40 } },
            {   "Gain",  Parameter::SLIDER, -60.0, 18.0, 0.0,           { 190,40,20,85 } },
            {   "FREQ",  Parameter::ROTARY, 0.0, 1.0, 0.31288541,           { 180,150,40,40 } },
            {   "Q",     Parameter::ROTARY, 0.0, 1.0, 0.39,              { 180,210,40,40 } },
            {   "Gain",  Parameter::SLIDER, -60.0, 18.0, 0.0,           { 240,40,20,85 } },
            {   "FREQ",  Parameter::ROTARY, 0.0, 1.0, 0.406593261,           { 230,150,40,40 } },
            {   "Q",     Parameter::ROTARY, 0.0, 1.0, 0.39,              { 230,210,40,40 } },
            {   "Gain",  Parameter::SLIDER, -60.0, 18.0, 0.0,           { 290,40,20,85 } },
            {   "FREQ",  Parameter::ROTARY, 0.0, 1.0, 0.5264987510,           { 280,150,40,40 } },
            {   "Q",     Parameter::ROTARY, 0.0, 1.0, 0.39,              { 280,210,40,40 } },
            {   "LP CutOff",  Parameter::ROTARY, 0.0, 1.0, 0.680780185,           { 330,150,40,40 } },
        };

        const Presets PRESETS = {
            { "Default", { 0.129109339, 0.0, 0.179730108, 0.39, 0.0,0.239360064,0.39, 0.0,0.31288541, 0.39, 0.0, 0.406593261, 0.39, 0.0, 0.5264987510, 0.39, 0.680780185} },
            { "20Hz + 18kHz cut", { 0.263609992, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0.920198048 } },
            { "kick", { 0.129109339, -2.7, 0.0, 0.21, 2.6, 0.263609992,0.20, -18.0, 0.482619378, 0.39, 0.0, 0.406593261, 0.39, 0.0, 0.5264987510, 0.39, 0.680780185} },
            { "spikey", { 0.129109339, 18.0, 0.289730108, 1.0, -60.0, 0.239360064,1.0, 18.0, 0.538541, 1.0, -60.0, 0.406593261, 1.0, 18.0, 0.8264987510, 1.0, 0.680780185} },
        };

        return (APDI::Effect*)new MyEffect(CONTROLS, PRESETS);
    }
}

// Constructor: called when the effect is first created / loaded
MyEffect::MyEffect(const Parameters& parameters, const Presets& presets)
    : Effect(parameters, presets)
{
    // Initialise member variables, etc.
}

// Destructor: called when the effect is terminated / unloaded
MyEffect::~MyEffect()
{
    //clean up code here (e.g. free memory)
}

// EVENT HANDLERS: handle different user input (button presses, preset selection, drop menus)

void MyEffect::presetLoaded(int iPresetNum, const char* sPresetName)
{
    // A preset has been loaded, so you could perform setup, such as retrieving parameter values
    // using getParameter and use them to set state variables in the plugin
}

void MyEffect::optionChanged(int iOptionMenu, int iItem)
{
    // An option menu, with index iOptionMenu, has been changed to the entry, iItem
}

void MyEffect::buttonPressed(int iButton)
{
    // A button, with index iButton, has been pressed
}

float cube(float x) {
    float y = (x * x * x);
    return y;
}


float MyEffect::setupBPFFilters(int indexOfFreqParam, int indexOfFilter, BPF* bpFilters)
{
   // setting up band pass filter controls
    float fGain = pow(10, (parameters[indexOfFreqParam - 1]) / 20);
    float fCentre = cube(parameters[indexOfFreqParam]) * (20000 - 20) + 20;
    float fQ = cube(parameters[indexOfFreqParam + 1]) * (20 - 0.5) + 0.5;
    
    //left and right bandpass filters
    bpFilters[indexOfFilter].setQ(fCentre, fQ);
    bpFilters[indexOfFilter + 1].setQ(fCentre, fQ);

    return fGain;
}


// Applies audio processing to a buffer of audio
// (inputBuffer contains the input audio, and processed samples should be stored in outputBuffer)
void MyEffect::process(const float** inputBuffers, float** outputBuffers, int numSamples)
{
    float Ldry, Rdry, LOut = 0, ROut = 0;
    float Lwet[5];
    float Rwet[5];
    float fCutoff[2];
    float fGain[5];
    for(int i = 0; i < 5; i++) fGain[i] = 1;
  
    const float* pfInBuffer0 = inputBuffers[0], * pfInBuffer1 = inputBuffers[1];
    float* pfOutBuffer0 = outputBuffers[0], * pfOutBuffer1 = outputBuffers[1];

    // high pass filter parameters
    fCutoff[0] = cube(parameters[0]) * (24000 - 20) + 20;

   // low pass filter parameters
    fCutoff[1] = cube(parameters[16]) * (20000 - 20) + 20;

    //low and high pass filter setup
    for (int i = 0; i < 2; i++) {
        hpfilter[i].setCutoff(fCutoff[0]);
        lpfilter[i].setCutoff(fCutoff[1]);
    }
    //bandpass filters
    for (int i = 0; i < 5; i++) { 
        fGain[i] = setupBPFFilters(2 + (i * 3), i * 2, bpfilter); 
    }
    
    while (numSamples--)
    {
        // Get sample from input
        Ldry = *pfInBuffer0++;
        Rdry = *pfInBuffer1++;

        //white noise generator for testing
       // float noise = rand() / (float)RAND_MAX * 2.f - 1.f;
       // Ldry = Rdry = noise; 
       // LOut = ROut = noise;
       
       //effects processing

       //highpass filtering
        Ldry = hpfilter[0].tick(Ldry);
        Rdry = hpfilter[1].tick(Rdry);
       
        //lowpass filtering 
        Ldry = lpfilter[0].tick(Ldry);
        Rdry = lpfilter[1].tick(Rdry);
       
        //Left and Right band passs / band reject filtering in parallel
        for (int i = 0; i < 5; i++) {
            Lwet[i] = bpfilter[i * 2    ].tick(Ldry) * fGain[i]; //even # filters are Left
            Rwet[i] = bpfilter[i * 2 + 1].tick(Rdry) * fGain[i]; //odd # filters are right
        }
        //paralel bandpass filters output
        LOut = Lwet[0] + Lwet[1] + Lwet[2] + Lwet[3] + Lwet[4];
        ROut = Rwet[0] + Rwet[1] + Rwet[2] + Rwet[3] + Rwet[4];
        

        // Copy result to output
        *pfOutBuffer0++ = LOut;
        *pfOutBuffer1++ = ROut;

    }
}
