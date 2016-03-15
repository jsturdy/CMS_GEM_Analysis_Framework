//
//  AnalyzeResponseUniformityHits.cpp
//  
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#include "DetectorMPGD.h"
#include "AnalyzeResponseUniformityHits.h"

using std::cout;
using std::endl;
using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

using Timing::getString;
using Timing::printROOTFileStatus;
using Timing::HistoSetup;
using Timing::stofSafe;

using namespace Uniformity;

//Default Constructor
AnalyzeResponseUniformityHits::AnalyzeResponseUniformityHits(){
    strAnalysisName = "analysis";
} //End Default Constructor

//Set inputs at construction
AnalyzeResponseUniformityHits::AnalyzeResponseUniformityHits(AnalysisSetupUniformity inputSetup, DetectorMPGD inputDet){
    strAnalysisName = "analysis";
    
    //Store Analysis Parameters
    aSetup = inputSetup;
    
    //Store Detector
    detMPGD = inputDet;
} //End Constructor

//Loops over all stored Hits in detMPGD and Book histograms for the full detector
void AnalyzeResponseUniformityHits::fillHistos(){
    //Variable Declaration
    
    //Initialize Summary Histograms
    //  Placeholder
    
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        
        //Grab Eta Sector width (for ClustPos Histo)
        aSetup.histoSetup_hitPos.fHisto_xLower = 0.;
        aSetup.histoSetup_hitPos.fHisto_xUpper = 128. * (*iterEta).second.map_sectorsPhi.size();
        
        //Initialize iEta Histograms - 1D
        (*iterEta).second.hitHistos.hPos = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_hitPos ) );
        (*iterEta).second.hitHistos.hTime = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_hitTime ) );
        
        //Initialize iEta Histograms - 2D
        //  Placeholder
        
        //Debugging
        //cout<<"(*iterEta).second.hitHistos.hADC->GetName() = " << (*iterEta).second.hitHistos.hADC->GetName() << endl;
        
        //Loop Over Stored iPhi Sectors
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over iPhi Sectors
            
            //Initialize iPhi Histograms - 1D
            (*iterPhi).second.hitHistos.hTime = make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_hitTime ) );
            
            //Initialize iPhi Histograms - 2D
            //  Placeholder
            
            //Loop Over Stored Hits
            for (auto iterHit = (*iterPhi).second.vec_hits.begin(); iterHit != (*iterPhi).second.vec_hits.end(); ++iterHit) { //Loop Over Stored Hits
                //Fill iEta Histograms
                (*iterEta).second.hitHistos.hPos->Fill( (*iterHit).iStripNum );
                (*iterEta).second.hitHistos.hTime->Fill( (*iterHit).iTimeBin );
                
                //Fill iPhi Histograms
                (*iterPhi).second.hitHistos.hTime->Fill( (*iterHit).iTimeBin);
            } //End Loop Over Stored Hits
        } //End Loop Over iPhi Sectors
        
        //Debugging
        std::cout<<"(*iterEta).second.hitHistos.hPos->Integral() = " << (*iterEta).second.hitHistos.hPos->Integral() << std::endl;
    } //End Loop Over iEta Sectors
    
    return;
} //End AnalyzeResponseUniformityHits::fillHistos() - Full Detector

//Loops over all slices in detMPGD and fits Booked histograms for the full detector
//Assumes Histos have been filled already (obviously)
void AnalyzeResponseUniformityHits::fitHistos(){
    
    //Placeholder
    
    return;
} //End AnalyzeResponseUniformityHits::fitHistos()

//Loads a ROOT file previously created by an instance of AnalyzeResponseUniformityHits
//Loads all TObjects found in the input ROOT file into detMPGD;
//Any previously stored information in detMPGD is lost.
void AnalyzeResponseUniformityHits::loadHistosFromFile( std::string & strInputMappingFileName, std::string & strInputROOTFileName ){
    
    //Placeholder
    
    return;
} //End AnalyzeResponseUniformityHits::loadHistosFromFile()

//Stores booked histograms (for those histograms that are non-null)
void AnalyzeResponseUniformityHits::storeHistos( string & strOutputROOTFileName, std::string strOption ){
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("AnalyzeResponseUniformityHits","storeHistos","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("AnalyzeResponseUniformityHits","storeHistos", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeResponseUniformityHits","storeHistos", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Loop over ieta's
        //Create/Load file structure
        //Store ieta level histograms
        //Loop over iphi's within ieta's
            //Create/Load file structure
            //Store iphi level histograms
                //Loop over slices
                    //Create/Load file structure
                    //store slice level histograms
    //Close File
    
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        
        //Get Directory
        //-------------------------------------
        //Check to see if the directory exists already
        TDirectory *dir_SectorEta = ptr_fileOutput->GetDirectory( ( "SectorEta" + getString( (*iterEta).first ) ).c_str(), false, "GetDirectory" );
        
        //If the above pointer is null the directory does NOT exist, create it
        if (dir_SectorEta == nullptr) { //Case: Directory did not exist in file, CREATE
            dir_SectorEta = ptr_fileOutput->mkdir( ( "SectorEta" + getString( (*iterEta).first ) ).c_str() );
        } //End Case: Directory did not exist in file, CREATE
        
        //Debugging
        cout<<"dir_SectorEta->GetName() = " << dir_SectorEta->GetName()<<endl;
        
        //Store Histograms - SectorEta Level
        //-------------------------------------
        dir_SectorEta->cd();
        (*iterEta).second.hitHistos.hPos->Write();
        (*iterEta).second.hitHistos.hTime->Write();
        
        //Loop Over Stored iPhi Sectors within this iEta Sector
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over Stored iPhi Sectors
            //Get Directory
            //-------------------------------------
            //Check to see if the directory exists already
            TDirectory *dir_SectorPhi = dir_SectorEta->GetDirectory( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str(), false, "GetDirectory"  );
            
            //If the above pointer is null the directory does NOT exist, create it
            if (dir_SectorPhi == nullptr) { //Case: Directory did not exist in file, CREATE
                dir_SectorPhi = dir_SectorEta->mkdir( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str() );
            } //End Case: Directory did not exist in file, CREATE
            
            //Debugging
            //cout<<"dir_SectorPhi->GetName() = " << dir_SectorPhi->GetName()<<endl;
            
            //Store Histograms - SectorPhi Level
            //-------------------------------------
            dir_SectorPhi->cd();
            (*iterPhi).second.hitHistos.hTime->Write();
        } //End Loop Over Stored iPhi Sectors
    } //End Loop Over Stored iEta Sectors
    
    //Close the ROOT file
    ptr_fileOutput->Close();
    
    return;
} //End storeHistos()

void AnalyzeResponseUniformityHits::storeFits( string & strOutputROOTFileName, std::string strOption ){
    
    //Placeholder
    
} //End storeHistos()
