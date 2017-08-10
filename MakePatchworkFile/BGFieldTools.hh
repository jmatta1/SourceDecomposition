// BGFIELDTOOLS.H
#pragma once
#ifndef BGFIELDTOOLS_H
#define BGFIELDTOOLS_H

#include "TROOT.h"
#include "TRint.h"
#include "TTree.h"
#include "TH1.h"
#include "TFile.h"
#include <iostream>
#include <vector>
#include <string>
#include "TMath.h"

using namespace std;

template <class myType>
myType* ThreeVector (myType x, myType y, myType z) {
  myType* TV = new myType[3];
  TV[0] = x;  TV[1]=y; TV[2]=z; 
  return TV;
}

class BGFieldTools{
public:
  BGFieldTools();       //constructor
  ~BGFieldTools(){};      //destructor
  
  Float_t* Coord_Transfer_MtoD(Float_t*);
  Float_t* Coord_Transfer_DtoM(Float_t*);
  vector<Float_t> Coord_Transfer_MtoD(vector<Float_t>);
  vector<Float_t> Coord_Transfer_DtoM(vector<Float_t>);
  void AddPatch(TH1D*, Float_t* Center, Float_t* Half, int);
  void SetOutputFile(const string& oname);
  void SetMeasurementOrigin(Float_t* OO);
  void SetDetectorOrigin(Float_t* OO);
  void SetMeasurementPositiveX(Float_t* XX);
  void SetDetectorPositiveX(Float_t* XX);
  void Unit(Float_t* V);
  Int_t SegmentShieldX(Int_t NpointsY, Int_t NpointsZ, vector<vector<Float_t> >* centers, vector<vector<Float_t> >* hLengths, Int_t sideflag);
  Int_t SegmentShieldY(Int_t NpointsX, Int_t NpointsZ, vector<vector<Float_t> >* centers, vector<vector<Float_t> >* hLengths, Int_t sideflag);
  Int_t SegmentShieldZ(Int_t NpointsX, Int_t NpointsY, vector<vector<Float_t> >* centers, vector<vector<Float_t> >* hLengths, Int_t sideflag);
private:
  string outname;
  Float_t detector_origin[3];
  Float_t measurement_origin[3];
  Float_t detector_positiveX[3];
  Float_t measurement_positiveX[3];
  Float_t ShieldSize[3];
};
#endif

BGFieldTools::BGFieldTools(){
  ShieldSize[0] = 128*2.54;
  ShieldSize[1] = 116*2.54;
  ShieldSize[2] = 125.375*2.54;   // mm, from DocDB 1250 with Water bricks
  
  detector_origin[0] = 0;
  detector_origin[1] = 0;
  detector_origin[2] = 0;
  
  detector_positiveX[0] = 1; 
  detector_positiveX[1] = 0;
  detector_positiveX[2] = 0;
  
  measurement_origin[0] = 0;
  measurement_origin[1] = 0;
  measurement_origin[2] = 0;
  
  measurement_positiveX[0] = 1; 
  measurement_positiveX[1] = 0;
  measurement_positiveX[2] = 0;
}

 void BGFieldTools::SetOutputFile(const string& oname){
   outname = oname;
}

Float_t* BGFieldTools::Coord_Transfer_DtoM(Float_t* vec){
  //take a point in the detector coordinate system and get the coordinates in the measurement coordinate system

  Float_t* newvec = new Float_t[3];
  Float_t transvec[3]; 
  
  Float_t cth = detector_positiveX[0]*measurement_positiveX[0] + detector_positiveX[1]*measurement_positiveX[1];   // law of cosines
  Float_t sth = sqrt(1-cth*cth);
  
  transvec[0] = vec[0] + detector_origin[0] - measurement_origin[0];
  transvec[1] = vec[1] + detector_origin[1] - measurement_origin[1];
  transvec[2] = vec[2] + detector_origin[2] - measurement_origin[2];

  newvec[0] = transvec[0]*cth - transvec[1]*sth;
  newvec[1] = transvec[0]*sth + transvec[1]*cth;
  newvec[2] = transvec[2];

  return newvec;
}

Float_t* BGFieldTools::Coord_Transfer_MtoD(Float_t* vec){
  //take a point in the measurement coordinate system and get the coordinates in the detector coordinate system
  Float_t* newvec = new Float_t[3];
  Float_t rotvec[3]; 
  
  Float_t cth = detector_positiveX[0]*measurement_positiveX[0] + detector_positiveX[1]*measurement_positiveX[1];   // law of cosines
  Float_t sth = -sqrt(1-cth*cth);

  rotvec[0] = vec[0]*cth - vec[1]*sth;
  rotvec[1] = vec[0]*sth + vec[1]*cth;
  rotvec[2] = vec[2];

  newvec[0] = rotvec[0] - detector_origin[0] + measurement_origin[0];
  newvec[1] = rotvec[1] - detector_origin[1] + measurement_origin[1];
  newvec[2] = rotvec[2] - detector_origin[2] + measurement_origin[2];
  
  return newvec;
}

vector<Float_t> BGFieldTools::Coord_Transfer_DtoM(vector<Float_t> vec){
  Float_t array[3] = {vec[0], vec[1], vec[2]};
  Float_t* newarr = Coord_Transfer_DtoM(array);
  vector<Float_t> newvec;
  for(int i=0;i<3;i++){ newvec.push_back(newarr[i]);}
  return newvec;
}

vector<Float_t> BGFieldTools::Coord_Transfer_MtoD(vector<Float_t> vec){
  Float_t array[3] = {vec[0], vec[1], vec[2]};
  Float_t* newarr = Coord_Transfer_MtoD(array);
  vector<Float_t> newvec;
  for(int i=0;i<3;i++){ newvec.push_back(newarr[i]);}
  return newvec;
}

void BGFieldTools::AddPatch(TH1D* tempH, Float_t* Center, Float_t* Half, Int_t PDG=22)
{
    cout<<"Adding Patch"<<endl;

    float specL, specH;  
    Float_t hX, hY, hZ;
    Float_t XX, YY, ZZ;
    Int_t PDGid = PDG; 

    hX = Half[0];
    hY = Half[1];
    hZ = Half[2];

    XX = Center[0];
    YY = Center[1];
    ZZ = Center[2];

    specL = tempH->GetXaxis()->GetXmin();
    specH = tempH->GetXaxis()->GetXmax();
    //cout<<specL<<", "<<specH<<endl;
    TFile *outfile = new TFile(outname.c_str(),"Update");
    TTree *RadField;
    if(outfile->GetListOfKeys()->Contains("RadField"))
    {
        //cout<<"found tree"<<endl;
        vector<Double_t> *Spectrum = new vector<Double_t>();
        for(int i=0;i<tempH->GetNbinsX();i++)
        {
            Spectrum->push_back(tempH->GetBinContent(i));
        }
        RadField = (TTree*)outfile->Get("RadField"); 
        RadField->SetBranchAddress("Spectrum",&Spectrum); 
        RadField->SetBranchAddress("specL",&specL); 
        RadField->SetBranchAddress("specH",&specH);
        RadField->SetBranchAddress("CenterX",&XX); 
        RadField->SetBranchAddress("CenterY",&YY); 
        RadField->SetBranchAddress("CenterZ",&ZZ); 
        RadField->SetBranchAddress("HalfX",&hX); 
        RadField->SetBranchAddress("HalfY",&hY); 
        RadField->SetBranchAddress("HalfZ",&hZ); 
        RadField->SetBranchAddress("PDGid",&PDGid); 
    }
    else
    {
        //cout << "Didn't find tree"<<endl;
        vector<Double_t> *Spectrum = new vector<Double_t>();
        //    vector<Double_t> Spectrum;
        for(int i=0;i<tempH->GetNbinsX();i++)
        {
            Spectrum->push_back(tempH->GetBinContent(i));
        }

        RadField= new TTree("RadField","Radiation Field");
        RadField->Branch("Spectrum",&Spectrum);
        RadField->Branch("specL",&specL); 
        RadField->Branch("specH",&specH);
        RadField->Branch("CenterX",&XX);
        RadField->Branch("CenterY",&YY);
        RadField->Branch("CenterZ",&ZZ);
        RadField->Branch("HalfX",&hX);
        RadField->Branch("HalfY",&hY);
        RadField->Branch("HalfZ",&hZ);
        RadField->Branch("PDGid",&PDGid);
    }
    RadField->Fill();
    outfile->Write();
    outfile->Close();
}

void BGFieldTools::SetMeasurementOrigin(Float_t* OO){
  measurement_origin[0] = OO[0];
  measurement_origin[1] = OO[1];
  measurement_origin[2] = OO[2];
}

void BGFieldTools::SetDetectorOrigin(Float_t* OO){
  detector_origin[0] = OO[0];
  detector_origin[1] = OO[1];
  detector_origin[2] = OO[2];
}

void BGFieldTools::SetMeasurementPositiveX(Float_t* XX){
  Unit(XX);
  measurement_positiveX[0] = XX[0]; 
  measurement_positiveX[1] = XX[1];
  measurement_positiveX[2] = XX[2];
}

void BGFieldTools::SetDetectorPositiveX(Float_t* XX){
  Unit(XX);
  detector_positiveX[0] = XX[0]; 
  detector_positiveX[1] = XX[1];
  detector_positiveX[2] = XX[2];
}

void BGFieldTools::Unit(Float_t *V){
  Float_t lV = sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);
  V[0]=V[0]/lV;
  V[1]=V[1]/lV;
  V[2]=V[2]/lV;
}

Int_t BGFieldTools::SegmentShieldX(Int_t NpointsY, Int_t NpointsZ, vector<vector<Float_t> >* centers, vector<vector<Float_t> >* hLengths, Int_t sideflag=0){
  // if sideflag is positive, only one side will be done, if sigeflag is negative, the other side will be done, if sideflag is 0, both sides will be done.
  Int_t pointcnt = 0;
  Float_t Xpos = ShieldSize[0]/2.0;
  vector<Float_t> center;
  vector<Float_t> hLen;
  center.resize(3);
  hLen.push_back(0);
  hLen.push_back(ShieldSize[1]/(2*NpointsY));
  hLen.push_back(ShieldSize[2]/(2*NpointsZ));

  for(int yy=0; yy<NpointsY; yy++){
    for(int zz=0; zz<NpointsZ; zz++){
      center[1] = (-ShieldSize[1]/2.0 + hLen[1]*(2*yy+1));
      center[2] = (-ShieldSize[2]/2.0 + hLen[2]*(2*zz+1));
      if(sideflag>=0){
	center[0] = (Xpos);
	centers->push_back(center);
	hLengths->push_back(hLen);
	pointcnt++;
      }
      if(sideflag<=0){
	center[0] = (-Xpos);
	centers->push_back(center);
	hLengths->push_back(hLen);
	pointcnt++;
      }
    }
  }
  return pointcnt;
}

Int_t BGFieldTools::SegmentShieldY(Int_t NpointsX, Int_t NpointsZ, vector<vector<Float_t> >* centers, vector<vector<Float_t> >* hLengths, Int_t sideflag=0){
  // if sideflag is positive, only one side will be done, if sigeflag is negative, the other side will be done, if sideflag is 0, both sides will be done.
  Int_t pointcnt = 0;
  Float_t Ypos = ShieldSize[1]/2.0;
  vector<Float_t> center;
  vector<Float_t> hLen;
  center.resize(3);
  hLen.push_back(ShieldSize[0]/(2*NpointsX));
  hLen.push_back(0);
  hLen.push_back(ShieldSize[2]/(2*NpointsZ));

  for(int xx=0; xx<NpointsX; xx++){
    for(int zz=0; zz<NpointsZ; zz++){
      center[0] = (-ShieldSize[0]/2.0 + hLen[0]*(2*xx+1));
      center[2] = (-ShieldSize[2]/2.0 + hLen[2]*(2*zz+1));
      if(sideflag>=0){
	center[1] = (Ypos);
	centers->push_back(center);
	hLengths->push_back(hLen);
	pointcnt++;
      }
      if(sideflag<=0){
	center[1] = (-Ypos);
	centers->push_back(center);
	hLengths->push_back(hLen);
	pointcnt++;
      }
    }
  }
  return pointcnt;
}

Int_t BGFieldTools::SegmentShieldZ(Int_t NpointsX, Int_t NpointsY, vector<vector<Float_t> >* centers, vector<vector<Float_t> >* hLengths, Int_t sideflag=0){
  // if sideflag is positive, only one side will be done, if sigeflag is negative, the other side will be done, if sideflag is 0, both sides will be done.
  Int_t pointcnt = 0;
  Float_t Zpos = ShieldSize[2]/2.0;
  vector<Float_t> center;
  vector<Float_t> hLen;
  center.resize(3);
  hLen.push_back(ShieldSize[0]/(2*NpointsX));
  hLen.push_back(ShieldSize[1]/(2*NpointsY));
  hLen.push_back(0);

  for(int xx=0; xx<NpointsX; xx++){
    for(int yy=0; yy<NpointsY; yy++){
      center[0] = (-ShieldSize[0]/2.0 + hLen[0]*(2*xx+1));
      center[1] = (-ShieldSize[1]/2.0 + hLen[1]*(2*yy+1));
      if(sideflag>=0){
	center[2] = (Zpos);
	centers->push_back(center);
	hLengths->push_back(hLen);
	pointcnt++;
      }
      if(sideflag<=0){
	center[2] = (-Zpos);
	centers->push_back(center);
	hLengths->push_back(hLen);
	pointcnt++;
      }
    }
  }
  return pointcnt;
}



