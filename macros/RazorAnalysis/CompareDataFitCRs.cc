//Macro to plot yields in data and MC in the razor control regions and compare with the prediction from the fit method

#include <iostream>
#include <map>
#include <string>

#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TTreeFormula.h"
#include "TStyle.h"
#include "TROOT.h"
#include "THStack.h"
#include "TLegend.h"
#include "TPad.h"
#include "assert.h"
#include "math.h"

#include "RazorAnalyzerRun1/include/ControlSampleEvents.h"
#include "include/MacroHelper.h"

using namespace std;

void SetHistogramColor(TH1 *hist, string name){
    if(name == "QCD") hist->SetFillColor(33);
    if(name == "ZJetsNuNu") hist->SetFillColor(kCyan+1);
    if(name == "WJets") hist->SetFillColor(kRed+1);
    if(name == "TTJets") hist->SetFillColor(kGreen+3);
    if(name == "DYJets") hist->SetFillColor(kAzure);
    if(name == "SingleTop") hist->SetFillColor(kBlue+3);
    if(name == "TTV") hist->SetFillColor(kSpring);
    if(name == "VV") hist->SetFillColor(kViolet+2);
    if(name == "GJets") hist->SetFillColor(8);
    if(name == "VG") hist->SetFillColor(38);
    if(name == "TTG") hist->SetFillColor(7);
}

void CompareDataFitCRs(){
    bool makeScaleFactors = true; //compute the scale factors for each control region
    //bool makeScaleFactors = false; 

    gROOT->SetBatch();

    int lumiInData = 19700; //in pb
    int lumiInMC = 1; //luminosity used to normalize input MC ntuples

    //set color palette 
    const Int_t NCont = 101;
    gStyle->SetNumberContours(NCont);
    gStyle->SetPaintTextFormat("1.0f");

    //for plots
    float nMRBins = 8;
    float nRsqBins = 7;
    float MRBinLowEdges[] = {300, 350, 400, 450, 550, 700, 900, 1200, 4000};
    float RsqBinLowEdges[] = {0.15, 0.20, 0.25, 0.30, 0.41, 0.52, 0.64, 1.5};

    //get input files
    map<string, map<string, string> > mcfilenames;
    map<string, map<string, string> > datafilenames;
    string baseDir = "root://eoscms://store/group/phys_susy/razor/Run1Analysis/RunOneRazorControlRegions/";
    string mcSuffix = "_1pb_weighted.root";
    string dataSuffix = "_GoodLumi.root";

    mcfilenames["SingleLeptonRazorSkim"] = map<string, string>();
    mcfilenames["DileptonRazorSkim"] = map<string, string>();
    mcfilenames["ZNuNuDileptonRazorSkim"] = map<string, string>();
    datafilenames["SingleLeptonRazorSkim"] = map<string, string>();
    datafilenames["DileptonRazorSkim"] = map<string, string>();
    datafilenames["ZNuNuDileptonRazorSkim"] = map<string, string>();

    mcfilenames["SingleLeptonRazorSkim"]["DYJets"] = baseDir+"/SingleLeptonRazorSkim/RunOneRazorControlRegions_SingleLeptonRazorSkim_DYJetsToLL_HTBinned"+mcSuffix;
    mcfilenames["SingleLeptonRazorSkim"]["TTJets"] = baseDir+"/SingleLeptonRazorSkim/RunOneRazorControlRegions_SingleLeptonRazorSkim_TTJets"+mcSuffix;
    mcfilenames["SingleLeptonRazorSkim"]["WJets"] = baseDir+"SingleLeptonRazorSkim/RunOneRazorControlRegions_SingleLeptonRazorSkim_WJetsToLNu_HTBinned"+mcSuffix;
    mcfilenames["SingleLeptonRazorSkim"]["TTV"] = baseDir+"/SingleLeptonRazorSkim/RunOneRazorControlRegions_SingleLeptonRazorSkim_TTV"+mcSuffix;
    mcfilenames["SingleLeptonRazorSkim"]["VV"] = baseDir+"/SingleLeptonRazorSkim/RunOneRazorControlRegions_SingleLeptonRazorSkim_VV"+mcSuffix;
    mcfilenames["SingleLeptonRazorSkim"]["QCD"] = baseDir+"/SingleLeptonRazorSkim/RunOneRazorControlRegions_SingleLeptonRazorSkim_QCD"+mcSuffix;
    mcfilenames["SingleLeptonRazorSkim"]["SingleTop"] = baseDir+"/SingleLeptonRazorSkim/RunOneRazorControlRegions_SingleLeptonRazorSkim_SingleTop"+mcSuffix;
    mcfilenames["DileptonRazorSkim"]["DYJets"] = baseDir+"/DileptonRazorSkim/RunOneRazorControlRegions_DileptonRazorSkim_DYJetsToLL_HTBinned"+mcSuffix;
    mcfilenames["DileptonRazorSkim"]["TTJets"] = baseDir+"/DileptonRazorSkim/RunOneRazorControlRegions_DileptonRazorSkim_TTJets"+mcSuffix;
    mcfilenames["DileptonRazorSkim"]["WJets"] = baseDir+"DileptonRazorSkim/RunOneRazorControlRegions_DileptonRazorSkim_WJetsToLNu_HTBinned"+mcSuffix;
    mcfilenames["DileptonRazorSkim"]["TTV"] = baseDir+"/DileptonRazorSkim/RunOneRazorControlRegions_DileptonRazorSkim_TTV"+mcSuffix;
    mcfilenames["DileptonRazorSkim"]["VV"] = baseDir+"/DileptonRazorSkim/RunOneRazorControlRegions_DileptonRazorSkim_VV"+mcSuffix;
    mcfilenames["DileptonRazorSkim"]["SingleTop"] = baseDir+"/DileptonRazorSkim/RunOneRazorControlRegions_DileptonRazorSkim_SingleTop"+mcSuffix;
    mcfilenames["ZNuNuDileptonRazorSkim"]["DYJets"] = baseDir+"/ZNuNuDileptonSkim/RunOneRazorControlRegions_ZNuNuDileptonSkim_DYJetsToLL_HTBinned"+mcSuffix;
    mcfilenames["ZNuNuDileptonRazorSkim"]["TTJets"] = baseDir+"/ZNuNuDileptonSkim/RunOneRazorControlRegions_ZNuNuDileptonSkim_TTJets"+mcSuffix;
    mcfilenames["ZNuNuDileptonRazorSkim"]["WJets"] = baseDir+"ZNuNuDileptonSkim/RunOneRazorControlRegions_ZNuNuDileptonSkim_WJetsToLNu_HTBinned"+mcSuffix;
    mcfilenames["ZNuNuDileptonRazorSkim"]["TTV"] = baseDir+"/ZNuNuDileptonSkim/RunOneRazorControlRegions_ZNuNuDileptonSkim_TTV"+mcSuffix;
    mcfilenames["ZNuNuDileptonRazorSkim"]["VV"] = baseDir+"/ZNuNuDileptonSkim/RunOneRazorControlRegions_ZNuNuDileptonSkim_VV"+mcSuffix;
    mcfilenames["ZNuNuDileptonRazorSkim"]["SingleTop"] = baseDir+"/ZNuNuDileptonSkim/RunOneRazorControlRegions_ZNuNuDileptonSkim_SingleTop"+mcSuffix;
    //mcfilenames["PhotonRazorSkim"]["GJets"] = baseDir+"/PhotonRazorSkim/RunOneRazorControlRegions_PhotonRazorSkim_GJets_HTBinned"+mcSuffix;
    //mcfilenames["PhotonRazorSkim"]["VG"] = baseDir+"/PhotonRazorSkim/RunOneRazorControlRegions_PhotonRazorSkim_VG"+mcSuffix;
    //mcfilenames["PhotonRazorSkim"]["TTG"] = baseDir+"/PhotonRazorSkim/RunOneRazorControlRegions_PhotonRazorSkim_TTG"+mcSuffix;
    //mcfilenames["PhotonRazorSkim"]["QCD"] = baseDir+"/PhotonRazorSkim/RunOneRazorControlRegions_PhotonRazorSkim_QCD"+mcSuffix;

    //data
    datafilenames["SingleLeptonRazorSkim"]["SingleMuon"] = baseDir+"/SingleLeptonRazorSkim/RunOneRazorControlRegions_SingleLeptonRazorSkim_Data_SingleMu"+dataSuffix;
    datafilenames["SingleLeptonRazorSkim"]["SingleElectron"] = baseDir+"/SingleLeptonRazorSkim/RunOneRazorControlRegions_SingleLeptonRazorSkim_Data_SingleElectron"+dataSuffix;
    datafilenames["DileptonRazorSkim"]["DoubleMuon"] = baseDir+"/DileptonRazorSkim/RunOneRazorControlRegions_DileptonRazorSkim_Data_DoubleMuParked"+dataSuffix;
    datafilenames["DileptonRazorSkim"]["DoubleElectron"] = baseDir+"/DileptonRazorSkim/RunOneRazorControlRegions_DileptonRazorSkim_Data_DoubleElectron"+dataSuffix;
    datafilenames["DileptonRazorSkim"]["MuE"] = baseDir+"/DileptonRazorSkim/RunOneRazorControlRegions_DileptonRazorSkim_Data_MuEG"+dataSuffix;
    datafilenames["ZNuNuDileptonRazorSkim"]["DoubleMuon"] = baseDir+"/ZNuNuDileptonSkim/RunOneRazorControlRegions_ZNuNuDileptonSkim_Data_DoubleMuParked"+dataSuffix;
    //datafilenames["PhotonRazorSkim"]["Photon"] = baseDir+"/PhotonRazorSkim/RunOneRazorControlRegions_PhotonRazorSkim_Data_Photon"+dataSuffix;

    //list of control regions
    vector<string> controlRegions {"TTBarSingleLepton", "TTBarDilepton", "WSingleLepton", "ZNuNuDilepton"};

    //assign datasets to control regions
    map<string, vector<string> > controlRegionMC;
    controlRegionMC["TTBarSingleLepton"] = vector<string> {"TTV", "VV", "QCD", "SingleTop", "DYJets", "WJets", "TTJets"};
    controlRegionMC["TTBarDilepton"] = vector<string> {"TTV", "VV", "SingleTop", "WJets", "DYJets", "TTJets"};
    controlRegionMC["WSingleLepton"] = vector<string> {"TTV", "VV", "QCD", "SingleTop", "DYJets", "TTJets", "WJets"};
    //controlRegionMC["ZLLDilepton"] = vector<string> {"TTV", "VV", "SingleTop", "WJets", "TTJets", "DYJets"};
    controlRegionMC["ZNuNuDilepton"] = vector<string> {"TTV", "VV", "SingleTop", "WJets", "TTJets", "DYJets"};
    //controlRegionMC["ZNuNuSingleLepton"] = vector<string> {"TTV", "VV", "QCD", "SingleTop", "DYJets", "TTJets", "WJets"};
    //controlRegionMC["ZNuNuPhoton"] = vector<string> {"TTG", "VG", "QCD", "GJets"};

    map<string, vector<string> > controlRegionData;
    controlRegionData["TTBarSingleLepton"] = vector<string> {"SingleMuon", "SingleElectron"};
    controlRegionData["TTBarDilepton"] = vector<string> {"DoubleMuon", "DoubleElectron", "MuE"};
    controlRegionData["WSingleLepton"] = vector<string> {"SingleMuon", "SingleElectron"};
    //controlRegionData["ZLLDilepton"] = vector<string> {"DoubleMuon", "DoubleElectron"};
    controlRegionData["ZNuNuDilepton"] = vector<string> {"DoubleMuon"};
    //controlRegionData["ZNuNuSingleLepton"] = vector<string> {"SingleMuon"};
    //controlRegionData["ZNuNuPhoton"] = vector<string> {"Photon"};

    map<string, string> controlRegionSkim;
    controlRegionSkim["TTBarSingleLepton"] = "SingleLeptonRazorSkim";
    controlRegionSkim["TTBarDilepton"] = "DileptonRazorSkim";
    controlRegionSkim["WSingleLepton"] = "SingleLeptonRazorSkim";
    //controlRegionSkim["ZLLDilepton"] = "DileptonRazorSkim";
    controlRegionSkim["ZNuNuDilepton"] = "ZNuNuDileptonRazorSkim";
    //controlRegionSkim["ZNuNuSingleLepton"] = "SingleLeptonRazorSkim";
    //controlRegionSkim["ZNuNuPhoton"] = "PhotonRazorSkim";

    //get trees
    map<string, map<string, ControlSampleEvents*> > mcevents;
    map<string, map<string, ControlSampleEvents*> > dataevents;
    mcevents["SingleLeptonRazorSkim"] = map<string, ControlSampleEvents*>();
    mcevents["DileptonRazorSkim"] = map<string, ControlSampleEvents*>();
    mcevents["ZNuNuDileptonRazorSkim"] = map<string, ControlSampleEvents*>();
    //mcevents["PhotonRazorSkim"] = map<string, ControlSampleEvents*>();
    map<string, int> kTreeTypes;
    kTreeTypes["SingleLeptonRazorSkim"] = 1;
    kTreeTypes["DileptonRazorSkim"] = 5;
    kTreeTypes["ZNuNuDileptonRazorSkim"] = 7;
    for(auto &skim : mcfilenames){
        for(auto &file : mcfilenames[skim.first]){
            mcevents[skim.first][file.first] = new ControlSampleEvents;
            mcevents[skim.first][file.first]->LoadTree(file.second.c_str(), kTreeTypes[skim.first]);
        }
        for(auto &file : datafilenames[skim.first]){
            dataevents[skim.first][file.first] = new ControlSampleEvents;
            dataevents[skim.first][file.first]->LoadTree(file.second.c_str(), kTreeTypes[skim.first]);
        }
    }

    map<string, TH2F*> SFHists;
    //load TTbar scale factor histograms
    TFile *SFFileTTJets = new TFile("data/ScaleFactors/Run1/TTBarSingleLeptonScaleFactors.root");
    SFHists["TTJets"] = (TH2F*)SFFileTTJets->Get("TTBarSingleLeptonScaleFactor");
    //TFile *SFFileTTJets = new TFile("data/ScaleFactors/Run1/TTBarDileptonScaleFactors.root");
    //SFHists["TTJets"] = (TH2F*)SFFileTTJets->Get("TTBarDileptonScaleFactor");

    //load WJets scale factor histogram
    TFile *SFFileWJets = new TFile("data/ScaleFactors/Run1/WJetsSingleLeptonScaleFactors.root");
    SFHists["WJets"] = (TH2F*)SFFileWJets->Get("WJetsSingleLeptonScaleFactor");

    //load DYJets scale factor histogram
    TFile *SFFileDYJets = new TFile("data/ScaleFactors/Run1/ZToLLScaleFactors.root");
    SFHists["DYJets"] = (TH2F*)SFFileDYJets->Get("ZToLLDileptonScaleFactor");

    //load ZNuNu scale factor histograms
    TFile *SFFileZJetsNuNu = new TFile("mcScaleFactorsRunOne.root"); //use the scale factors derived using this macro
    SFHists["ZJetsNuNu"] = (TH2F*)SFFileZJetsNuNu->Get("ZNuNuDileptonScaleFactors");

    //TFile *SFFileZJetsNuNu = new TFile("data/ScaleFactors/Run1/ZInvisibleScaleFactorsRun1.root");
    //TH2F *SFHistZJetsNuNu = (TH2F*)SFFileZJetsNuNu->Get("DYJetsScaleFactors");
    //TH2F *SFHistZJetsNuNu = (TH2F*)SFFileZJetsNuNu->Get("WJetsScaleFactors");
    //SFHists["ZJetsNuNu"] = (TH2F*)SFFileZJetsNuNu->Get("GJetsScaleFactors");

    //load ZNuNu-->DYJets weighting factors
    TFile *ZNuNuToDYWeightFile = new TFile("data/ScaleFactors/Run1/ZNuNuToDYScaleFactorsRun1.root");
    TH2F *ZNuNuToDYWeightHist = (TH2F*)ZNuNuToDYWeightFile->Get("razormcDYJets");
    float maxMRZNuNuToDY = ZNuNuToDYWeightHist->GetXaxis()->GetXmax() - 1;
    float maxRsqZNuNuToDY = ZNuNuToDYWeightHist->GetYaxis()->GetXmax() - 0.01;

    //load muon efficiency scale factor histogram
    TFile muIdSFFile("data/ScaleFactors/MuonEfficiencies_ID_Run2012ReReco_53X.root");
    TFile muIsoSFFile("data/ScaleFactors/MuonEfficiencies_ISO_Run_2012ReReco_53X.root");
    //TODO: use muon efficiency scale factors

    //load pileup reweighting histogram
    TFile *pileupWeightFile = new TFile("data/Run1PileupWeights.root", "READ");
    TH1F *pileupWeightHist = (TH1F*)pileupWeightFile->Get("PUWeight_Run1");
    assert(pileupWeightHist);

    ///////////////////////////////////////////////////////////
    // Fill Histograms
    ///////////////////////////////////////////////////////////
    map<string, map<string, TH2F*> > razorHistosMC;
    map<string, map<string, TH2F*> > razorErrorHistosMC; //store sum(w^2*error(SF)^2)
    map<string, TH2F*> razorHistosData; 
    ControlSampleEvents *curTree;

    //stuff for scale factor calculation
    map<string, string> signalNames; //desired sample to isolate in each control region
    signalNames["TTBarSingleLepton"] = "TTJets";
    signalNames["TTBarDilepton"] = "TTJets";
    signalNames["WSingleLepton"] = "WJets";
    //signalNames["ZLLDilepton"] = "DYJets";
    signalNames["ZNuNuDilepton"] = "DYJets";
    //signalNames["ZNuNuSingleLepton"] = "WJets";
    //signalNames["ZNuNuPhoton"] = "GJets";
    TFile *outSFFile;
    if(makeScaleFactors) outSFFile = new TFile("mcScaleFactorsRunOne.root", "RECREATE");
    map<string, TH2F* > newScaleFactors;

    for(auto &region : controlRegions){ 
        string theSkim = controlRegionSkim[region];
        ///////////////////////////////////////////////////////////
        // Get MC distribution
        ///////////////////////////////////////////////////////////
        cout << "Filling MC histograms for control region " << region << endl;
        for(auto &sample : controlRegionMC[region]){
            cout << "   Filling MC histograms: " << sample << endl;
            curTree = mcevents[theSkim][sample];

            //make histograms
            razorHistosMC[region][sample] = new TH2F(Form("razormc%s%s", region.c_str(), sample.c_str()), "; MR (GeV); Rsq", nMRBins, MRBinLowEdges, nRsqBins, RsqBinLowEdges);
            razorErrorHistosMC[region][sample] = new TH2F(Form("razorErrormc%s%s", sample.c_str(), region.c_str()), "sum(w^2*error(SF)^2); MR (GeV); Rsq", nMRBins, MRBinLowEdges, nRsqBins, RsqBinLowEdges);
            razorHistosMC[region][sample]->Sumw2();
            razorErrorHistosMC[region][sample]->Sumw2();

            //loop over entries
            uint nEntries = curTree->tree_->GetEntries();
            for(uint i = 0; i < nEntries; i++){
                //get entry
                curTree->tree_->GetEntry(i); 

                float eventWeight = curTree->weight*lumiInData*1.0/lumiInMC;
                float sysErrorSquared = 0.0;

                //selection cuts
                if(!curTree->inControlSample(region)) continue;

                // Apply scale factors
                eventWeight *= curTree->getMCCorrection(pileupWeightHist, region);

                //Data/MC scale factors -- apply existing SFs
                if(!makeScaleFactors){
                    if(SFHists.count(sample) > 0){
                        //cout << "Applying " << sample << " scale factor from file" << endl;
                        pair<double, double> sfAndErr = getDataMCSFAndError(SFHists[sample], curTree->MR, curTree->Rsq);
                        eventWeight *= sfAndErr.first; //multiply event weight by scale factor
                        sysErrorSquared += curTree->weight*curTree->weight*sfAndErr.second*sfAndErr.second; //add (w*sigma)^2 to the systematic uncertainty
                    }
                }
                else{ //apply SFs computed so far
                    if(newScaleFactors.count(sample) > 0 && sample != signalNames[region]){
                        //cout << "Applying " << sample << " scale factor calculated earlier" << endl;
                        pair<double, double> sfAndErr = getDataMCSFAndError(newScaleFactors[sample], curTree->MR, curTree->Rsq);
                        eventWeight *= sfAndErr.first; //multiply event weight by scale factor
                        sysErrorSquared += curTree->weight*curTree->weight*sfAndErr.second*sfAndErr.second; //add (w*sigma)^2 to the systematic uncertainty
                    }
                }

                ///////////////////////////////////////////////////////////
                // Fill histograms
                ///////////////////////////////////////////////////////////

                //ZNuNuDilepton CR
                if(region == "ZNuNuDilepton"){
                    razorHistosMC[region][sample]->Fill(curTree->MR_NoZ, curTree->Rsq_NoZ, eventWeight);
                    razorErrorHistosMC[region][sample]->Fill(curTree->MR_NoZ, curTree->Rsq_NoZ, sysErrorSquared);
                }
                //ZNuNuSingleLepton CR
                else if(region == "ZNuNuSingleLepton"){
                    //razorHistosMC[region][sample]->Fill(curTree->MR_noW, curTree->Rsq_noW, eventWeight);
                    //razorErrorHistosMC[region][sample]->Fill(curTree->MR_noW, curTree->Rsq_noW, sysErrorSquared);
                }
                //ZNuNuPhoton CR
                else if(region == "ZNuNuPhoton"){
                    //razorHistosMC[region][sample]->Fill(curTree->MR_noPho, curTree->Rsq_noPho, eventWeight);
                    //razorErrorHistosMC[region][sample]->Fill(curTree->MR_noPho, curTree->Rsq_noPho, sysErrorSquared);
                }
                //other CRs
                else{
                    razorHistosMC[region][sample]->Fill(curTree->MR, curTree->Rsq, eventWeight);
                    razorErrorHistosMC[region][sample]->Fill(curTree->MR, curTree->Rsq, sysErrorSquared);
                }
            } //end of event loop

            //update errors to take into account systematic uncertainties
            for(int i = 0; i < razorHistosMC[region][sample]->GetNbinsX()+1; i++){
                for(int j = 0; j < razorHistosMC[region][sample]->GetNbinsY()+1; j++){
                    double squaredError = razorErrorHistosMC[region][sample]->GetBinContent(i, j);
                    razorHistosMC[region][sample]->SetBinError(i, j, sqrt(pow(razorHistosMC[region][sample]->GetBinError(i, j), 2) + squaredError));
                }
            }

            //for rare background processes, include a 20% uncertainty on the total yield in each bin, summed in quadrature with the statistical uncertainty
            double sysErrorFrac = 0.2;
            //for QCD, assign a 100% uncertainty
            double qcdErrorFrac = 1.0;
            //only do this for rare processes 
            if(sample != "DYJets" && sample != "WJets" && sample != "ZJetsNuNu" && sample != "TTJets"){
                for(int i = 0; i < razorHistosMC[region][sample]->GetNbinsX()+1; i++){
                    for(int j = 0; j < razorHistosMC[region][sample]->GetNbinsY()+1; j++){
                        double error = 0.0;
                        if(sample == "QCD"){
                            error = qcdErrorFrac*razorHistosMC[region][sample]->GetBinContent(i, j);
                        }
                        else{
                            error = sysErrorFrac*razorHistosMC[region][sample]->GetBinContent(i, j);
                        }
                        razorHistosMC[region][sample]->SetBinError(i, j, sqrt(pow(razorHistosMC[region][sample]->GetBinError(i, j), 2) + error*error));
                    }
                }
            } //end if
        } //end of loop over MC samples

        ///////////////////////////////////////////////////////////
        // Get data distributions
        ///////////////////////////////////////////////////////////
        cout << "Filling data histograms for control region " << region << endl;
        razorHistosData[region] = new TH2F(Form("razordata%s", region.c_str()), "; MR (GeV); Rsq", nMRBins, MRBinLowEdges, nRsqBins, RsqBinLowEdges);
        razorHistosData[region]->Sumw2();

        for(auto &sample : controlRegionData[region]){
            cout << "   Filling data histograms: " << sample << endl;
            curTree = dataevents[theSkim][sample];

            //loop over entries
            uint nEntries = curTree->tree_->GetEntries();
            for(uint i = 0; i < nEntries; i++){
                //get entry
                curTree->tree_->GetEntry(i);

                //noise filters
                if(!(curTree->Flag_HBHENoiseFilter) || !(curTree->Flag_CSCTightHaloFilter) || !(curTree->Flag_eeBadScFilter) ) continue;

                //apply selection cuts
                if(!curTree->inControlSample(region)) continue;

                float eventWeight = 1.0;

                //reweight for photon triggers
                if(region == "ZNuNuPhoton"){
                    if(curTree->pho1.Pt()>5000) continue; // reject noise
                    eventWeight *= curTree->getPhotonTriggerWeight();
                }

                ///////////////////////////////////////////////////////////
                // Fill histograms
                ///////////////////////////////////////////////////////////

                //ZNuNuDilepton CR
                if(region == "ZNuNuDilepton"){
                    razorHistosData[region]->Fill(curTree->MR_NoZ, curTree->Rsq_NoZ, eventWeight);
                }
                //ZNuNuSingleLepton CR
                else if(region == "ZNuNuSingleLepton"){
                    //razorHistosData[region]->Fill(curTree->MR_noW, curTree->Rsq_noW, eventWeight);
                }
                //ZNuNuPhoton CR
                else if(region == "ZNuNuPhoton"){
                    //razorHistosData[region]->Fill(curTree->MR_noPho, curTree->Rsq_noPho, eventWeight);
                }
                //other CRs
                else{
                    razorHistosData[region]->Fill(curTree->MR, curTree->Rsq, eventWeight);
                }
            } //end event loop
        } //end loop over datasets 

        //create background subtracted histogram in each control region
        if(makeScaleFactors){
            outSFFile->cd();
            for(auto &sample : controlRegionMC[region]){ //loop over samples
                if(sample != signalNames[region]){ //subtract contribution from this process
                    cout << "Removing " << sample << " from " << region << " data" << endl;
                    razorHistosData[region]->Add(razorHistosMC[region][sample], -1);
                } //end if
            } //end loop over samples
            //create data/MC scale factor histogram and save it
            cout << "Creating scale factor histogram for " << region << endl;
            TH2F *newSFHist = (TH2F*)razorHistosData[region]->Clone(Form("%sScaleFactors", region.c_str()));
            newSFHist->Divide(razorHistosMC[region][signalNames[region]]);
            newSFHist->Write();
            if(region == "ZNuNuDilepton" || region == "ZNuNuSingleLepton" || region == "ZNuNuPhoton"){
                if(newScaleFactors.count("ZJetsNuNu") == 0){
                    newScaleFactors["ZJetsNuNu"] = newSFHist;
                }
            }
            else if(newScaleFactors.count(signalNames[region]) == 0){
                newScaleFactors[signalNames[region]] = newSFHist;
            }
        } //end if

    } //end of loop over control regions

    ///////////////////////////////////////////////////////////
    // Make plots
    ///////////////////////////////////////////////////////////
    cout << "Saving output plots..." << endl;
    TCanvas c("c", "c", 800, 600);
    for(auto &cr : controlRegions){
        cout << "Control region: " << cr << endl;
        gStyle->SetPaintTextFormat("1.0f");
        c.SetLogx();

        //create legend
        TLegend *RazorLegend = new TLegend(0.6, 0.6, 0.9, 0.9);
        cout << "Building legend: ";
        for(int i = controlRegionMC[cr].size() - 1; i >= 0; i--){
            string name = (controlRegionMC[cr])[i];
            SetHistogramColor(razorHistosMC[cr][name], name);
            RazorLegend->AddEntry(razorHistosMC[cr][name], name.c_str());
            cout << name.c_str() << " ";
        }
        cout << endl;
        razorHistosData[cr]->SetMarkerStyle(20);
        razorHistosData[cr]->SetMarkerSize(1);
        RazorLegend->AddEntry(razorHistosData[cr], "2012 Data");

        //plot slices of MR and Rsq
        for(int i = 0; i < nRsqBins; i++){
            map<string, TH1F*> ThisRsqSliceMCMap;    
            TH1F *ThisRsqSliceData = (TH1F*)razorHistosData[cr]->ProjectionX(Form("ThisRsqSliceData%d%s", i, cr.c_str()), i+1, i+1);
            THStack *ThisRsqSliceMC = new THStack("ThisRsqSliceMC", Form("MR (%.2f < Rsq < %.2f), %s Box", RsqBinLowEdges[i], RsqBinLowEdges[i+1], cr.c_str()));
            for(auto &sample : controlRegionMC[cr]){
                TH1F *thisHist;
                thisHist = (TH1F*)razorHistosMC[cr][sample]->ProjectionX(Form("hist%s%d%s", sample.c_str(), i, cr.c_str()), i+1, i+1);
                SetHistogramColor(thisHist, sample);
                ThisRsqSliceMCMap[sample] = thisHist;
                ThisRsqSliceMC->Add(ThisRsqSliceMCMap[sample]);
            }
            DrawDataVsMCRatioPlot(ThisRsqSliceData, ThisRsqSliceMC, RazorLegend, "MR (GeV)", "MRExclusiveSlice"+to_string(i)+cr, true);
        }
        for(int i = 0; i < nMRBins; i++){
            map<string, TH1F*> ThisMRSliceMCMap;    
            TH1F *ThisMRSliceData = (TH1F*)razorHistosData[cr]->ProjectionY(Form("ThisMRSliceData%d%s", i, cr.c_str()), i+1, i+1);
            THStack *ThisMRSliceMC = new THStack("ThisMRSliceMC", Form("Rsq (%0.f < MR < %.0f), %s Box", MRBinLowEdges[i], MRBinLowEdges[i+1], cr.c_str()));
            for(auto &sample : controlRegionMC[cr]){
                TH1F *thisHist;
                thisHist = (TH1F*)razorHistosMC[cr][sample]->ProjectionY(Form("hist%s%d%s", sample.c_str(), i, cr.c_str()), i+1, i+1);
                SetHistogramColor(thisHist, sample);
                ThisMRSliceMCMap[sample] = thisHist;
                ThisMRSliceMC->Add(ThisMRSliceMCMap[sample]);
            }
            DrawDataVsMCRatioPlot(ThisMRSliceData, ThisMRSliceMC, RazorLegend, "Rsq", "RsqExclusiveSlice"+to_string(i)+cr, true);
        }
        //inclusive slices
        for(int i = 0; i < nRsqBins; i++){
            map<string, TH1F*> ThisRsqSliceMCMap;    
            TH1F *ThisRsqSliceData = (TH1F*)razorHistosData[cr]->ProjectionX(Form("ThisRsqIncSliceData%d%s", i, cr.c_str()), i+1);
            THStack *ThisRsqSliceMC = new THStack("ThisRsqIncSliceMC", Form("MR (Rsq > %.2f), %s Box", RsqBinLowEdges[i], cr.c_str()));
            for(auto &sample : controlRegionMC[cr]){
                TH1F *thisHist;
                thisHist = (TH1F*)razorHistosMC[cr][sample]->ProjectionX(Form("histinc%s%d%s", sample.c_str(), i, cr.c_str()), i+1);
                SetHistogramColor(thisHist, sample);
                ThisRsqSliceMCMap[sample] = thisHist;
                ThisRsqSliceMC->Add(ThisRsqSliceMCMap[sample]);
            }
            DrawDataVsMCRatioPlot(ThisRsqSliceData, ThisRsqSliceMC, RazorLegend, "MR (GeV)", "MRInclusiveSlice"+to_string(i)+cr, true);
        }
        for(int i = 0; i < nMRBins; i++){
            map<string, TH1F*> ThisMRSliceMCMap;    
            TH1F *ThisMRSliceData = (TH1F*)razorHistosData[cr]->ProjectionY(Form("ThisMRSliceIncData%d%s", i, cr.c_str()), i+1);
            THStack *ThisMRSliceMC = new THStack("ThisMRSliceMC", Form("Rsq (MR > %.0f), %s Box", MRBinLowEdges[i], cr.c_str()));
            for(auto &sample : controlRegionMC[cr]){
                TH1F *thisHist;
                thisHist = (TH1F*)razorHistosMC[cr][sample]->ProjectionY(Form("histinc%s%d%s", sample.c_str(), i, cr.c_str()), i+1);
                SetHistogramColor(thisHist, sample);
                ThisMRSliceMCMap[sample] = thisHist;
                ThisMRSliceMC->Add(ThisMRSliceMCMap[sample]);
            }
            DrawDataVsMCRatioPlot(ThisMRSliceData, ThisMRSliceMC, RazorLegend, "Rsq", "RsqInclusiveSlice"+to_string(i)+cr, true);
        }

        delete RazorLegend;
    }
}

int main(){
    CompareDataFitCRs();
    return 0;
}
