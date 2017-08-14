#include"rxon_grid.h"
#include"BGFieldTools.hh"
#include<iostream>
using std::cout;
using std::endl;
#include<sstream>
#include<string>
using std::ostringstream;

#include"TFile.h"
#include"TH1D.h"
#include"TF1.h"

void add_panel_spectra(TFile* file, TH1D* result, int start, int stop)
{
    for(int i=start; i<=stop; ++i)
    {
        ostringstream namer;
        namer << "Panel_"<<i;
        TH1D* temp = (TH1D*)file->Get(namer.str().c_str());
        result->Add(temp, 1.0);
        delete temp;
    }
}

void make_side_spectra()
{
    TFile* file = new TFile("panel_file.root", "UPDATE");
    //handle the back of the detector
    TH1D* temp = (TH1D*)file->Get("Panel_0");
    TH1D* sidespec = (TH1D*)temp->Clone();
    sidespec->SetNameTitle("rear_hist", "Detector Rear Side - Total Incident Spectrum");
    delete temp;
    add_panel_spectra(file, sidespec, 1, 7);
    sidespec->Write();
    file->Flush();
    delete sidespec;
    temp = (TH1D*)file->Get("Panel_8");
    sidespec = (TH1D*)temp->Clone();
    sidespec->SetNameTitle("front_hist", "Detector Front Side - Total Incident Spectrum");
    delete temp;
    add_panel_spectra(file, sidespec, 9, 15);
    sidespec->Write();
    file->Flush();
    delete sidespec;
    temp = (TH1D*)file->Get("Panel_16");
    sidespec = (TH1D*)temp->Clone();
    sidespec->SetNameTitle("right_hist", "Detector Right Side - Total Incident Spectrum");
    delete temp;
    add_panel_spectra(file, sidespec, 17, 27);
    sidespec->Write();
    file->Flush();
    delete sidespec;
    temp = (TH1D*)file->Get("Panel_28");
    sidespec = (TH1D*)temp->Clone();
    sidespec->SetNameTitle("left_hist", "Detector Left Side - Total Incident Spectrum");
    delete temp;
    add_panel_spectra(file, sidespec, 29, 39);
    sidespec->Write();
    file->Flush();
    delete sidespec;
    temp = (TH1D*)file->Get("Panel_40");
    sidespec = (TH1D*)temp->Clone();
    sidespec->SetNameTitle("bottom_hist", "Detector Bottom Side - Total Incident Spectrum");
    delete temp;
    add_panel_spectra(file, sidespec, 41, 45);
    sidespec->Write();
    file->Flush();
    delete sidespec;
    temp = (TH1D*)file->Get("Panel_46");
    sidespec = (TH1D*)temp->Clone();
    sidespec->SetNameTitle("top_hist", "Detector Top Side - Total Incident Spectrum");
    delete temp;
    add_panel_spectra(file, sidespec, 47, 51);
    sidespec->Write();
    file->Flush();
    delete sidespec;
    delete file;
}

void make_patchwork(int start, int stop, const std::string& outname = "rxon_patches.root")
{
    TFile* file = new TFile("panel_file.root");
    BGFieldTools BFT;
    BFT.SetOutputFile(outname.c_str());
    float center[3];
    float halfwidth[3];
    double rate = 0.0;
    for(int i=start; i<=stop; ++i)
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
        //normalize the spectra to the area of the patch
        BFT.AddPatch(temp, center, halfwidth);
        delete temp;
    }
    delete file;
    cout<<"Total Rate (Hz): "<<rate<<endl;
}
