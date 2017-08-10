#include"rxon_grid.h"
#include"BGFieldTools.hh"
#include<iostream>
using std::cout;
using std::endl;
#include<sstream>
using std::ostringstream;

#include"TFile.h"
#include"TH1D.h"
#include"TF1.h"

void make_patchwork()
{
    TFile* file = new TFile("panel_file.root");
    TF1* unity = new TF1("unity", "1", -1.0, 50.0);
    BGFieldTools BFT;
    BFT.SetOutputFile("rxon_patches.root");
    float center[3];
    float halfwidth[3];
    double rate = 0.0;
    for(int i=0; i<46; ++i)
    {
        cout<<"Patch number: " << i <<endl;
        ostringstream namer;
        namer << "Panel_"<<i;
        TH1D* temp = (TH1D*)file->Get(namer.str().c_str());
        double tempRate = temp->Integral(1,1400);
        rate += tempRate;
        center[0] = xpos[i];
        center[1] = ypos[i];
        center[2] = zpos[i];
        halfwidth[0] = xwidth[i];
        halfwidth[1] = ywidth[i];
        halfwidth[2] = zwidth[i];
        temp->Multiply(unity, 1.0/areas[i]);
        BFT.AddPatch(temp, center, halfwidth);
        delete temp;
    }
    delete unity;
    delete file;
    cout<<"Total Rate (Hz): "<<rate<<endl;
}
