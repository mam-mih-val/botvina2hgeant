//
// Created by mikhail on 12/31/20.
//

#include <EventInitialState.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TH1F.h>
#include <UEvent.h>
#include <UParticle.h>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>

void AddFilesFromList( const std::string& list_name, TChain* chain );
std::map<int, int> InitializeGeantNucleiCodesMap();

int main(int n_args, char** args){
  if( n_args < 2 )
    throw std::runtime_error( "2 arguments expected, but only "+std::to_string( n_args )+" were provided" );
  std::string out_file_name{"out.root"};
  if( n_args > 2 )
    out_file_name=args[2];
  auto chain = new TChain("events");
  AddFilesFromList(args[1], chain);
  UEvent *event = new UEvent;
  EventInitialState* initial_state = new EventInitialState;
  chain->SetBranchAddress("event", &event);
  chain->SetBranchAddress("iniState", &initial_state);
  std::ofstream file_out{out_file_name};
  auto n_events = chain->GetEntries();
  auto geant_numbers = InitializeGeantNucleiCodesMap();
  for( int i=0; i<n_events; ++i ){
    chain->GetEntry(i);
    auto evt_id =  event->GetEventNr();
    auto b = event->GetB();
    auto Ebeam = 1.23;
    auto n_particles = event->GetNpa();
    int n_part=0;
    for( int j=0; j<n_particles; ++j ) {
      auto particle = event->GetParticle(j);
      auto pid = TDatabasePDG::Instance()->ConvertPdgToGeant3(particle->GetPdg());
      if (pid == 0) {
        auto pdg = particle->GetPdg();
        switch (pdg) {
        case 100'001'002'0:
          pid = 45;
          break;
        case 100'001'003'0:
          pid = 46;
          break;
        case 100'002'003'0:
          pid = 49;
          break;
        case 100'002'004'0:
          pid = 47;
          break;
        case 100'003'006'0:
          pid = 61;
          break;
        case 100'003'007'0:
          pid = 62;
          break;
        case 100'004'007'0:
          pid = 63;
          break;
        case 100'004'009'0:
          pid = 64;
          break;
        case 100'005'010'0:
          pid = 65;
          break;
        case 100'005'011'0:
          pid = 66;
          break;
        case 311: {
          std::random_device r;
          std::default_random_engine e1(r());
          std::uniform_int_distribution<int> uniform_dist(0, 1);
          int prob{uniform_dist(e1)};
          if (prob == 0)
            pid = 10;
          if (prob == 1)
            pid = 16;
          break;
        }
        default:
          int charge = (int)(pdg / 1E+4) % (int)1e+3;
          if (charge == 0)
            continue;
          while (pid == 0)
            try {
              pid = geant_numbers.at(charge);
            } catch (std::out_of_range &) {
              pid = 0;
              charge--;
            }
        }
      }
      n_part++;
    }
    std::string line = std::to_string( evt_id )+"\t"+std::to_string( n_part )+"\t"+std::to_string( Ebeam )+"\t"+std::to_string( b )+"\t0";
    file_out << line << std::endl;
    auto rp = event->GetPhi();
    for( int j=0; j<n_particles; ++j ){
      auto particle = event->GetParticle(j);
      auto E = particle->E();
      auto phi = particle->GetMomentum().Phi();
      auto pt = particle->GetMomentum().Pt();
      auto px = pt*cos(phi-rp);
      auto py = pt*sin(phi-rp);
      auto pz = particle->Pz();
      auto pid = TDatabasePDG::Instance()->ConvertPdgToGeant3(particle->GetPdg());
      if( pid == 0 ){
        auto pdg = particle->GetPdg();
        switch (pdg) {
          case 100'001'002'0:
            pid=45;
            break;
          case 100'001'003'0:
            pid=46;
            break;
          case 100'002'003'0:
            pid=49;
            break;
          case 100'002'004'0:
            pid=47;
            break;
          case 100'003'006'0:
            pid=61;
            break;
          case 100'003'007'0:
            pid=62;
            break;
          case 100'004'007'0:
            pid=63;
            break;
          case 100'004'009'0:
            pid=64;
            break;
          case 100'005'010'0:
            pid=65;
            break;
          case 100'005'011'0:
            pid=66;
            break;
          case 311: {
            std::random_device r;
            std::default_random_engine e1(r());
            std::uniform_int_distribution<int> uniform_dist(0, 1);
            int prob{uniform_dist(e1)};
            if (prob == 0)
              pid = 10;
            if (prob == 1)
              pid = 16;
            break;
          }
          default:
            int charge = (int) ( pdg / 1E+4) % (int) 1e+3;
            if(charge==0)
              continue;
          while ( pid==0 )
            try {
              pid = geant_numbers.at(charge);
            } catch (std::out_of_range&) { pid = 0; charge--; }
        }
      }
      line = std::to_string( E )+"\t"+std::to_string( px )+"\t"+std::to_string( py )+"\t"+std::to_string( pz )+"\t"+std::to_string( pid );
      file_out << line << std::endl;
    }
  }
  file_out << "0\t0\t0\t0\t0" << std::endl;
  file_out.close();
  return 0;
}

void AddFilesFromList( const std::string& list_name, TChain* chain ){
  std::ifstream file_list;
  file_list.open( list_name );
  std::string file;
  short line{0};
  while (std::getline( file_list, file )){
    chain->Add( file.c_str() );
    std::cout << line << " " << file << " has been added to a files sequence" << std::endl;
    line++;
  }
  std::cout << line << " files have been successfully added to a chain" << std::endl;
  std::cout << chain->GetEntries() << " events in a chain" << std::endl;
  file_list.close();
}

std::map<int, int> InitializeGeantNucleiCodesMap(){
  std::map<int, int> map{
      {2, 47},{3, 62 },{4, 64},{6, 67},{7, 68},{8, 69},
      {9, 70},{10, 71},{11, 72},{12,73},{13,74},
      {14,75},{15,76},{16,77},{17,78},{18,79},
      {19,80},{20,81},{21,82},{22,83},{23,84},
      {24,85},{25,86},{26,87},{27,88},{28,89},
      {29,90},{30,91},{32,92},{34,93},{36,94},
      {38,95},{40,96},{42,97},{46,98},{48,99},
      {50,100},{54,101},{56,102},{58,103},{62,104},
      {66,105},{70,106},{74,107},{78,108},{79,109},
      {80,110},{82,111},{92,112},
  };
  return map;
}