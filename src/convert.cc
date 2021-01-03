//
// Created by mikhail on 12/31/20.
//

#include <EventInitialState.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <UEvent.h>
#include <UParticle.h>
#include <fstream>
#include <iostream>
#include <stdexcept>

void AddFilesFromList( const std::string& list_name, TChain* chain );

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
  for( int i=0; i<n_events; ++i ){
    chain->GetEntry(i);
    auto evt_id =  event->GetEventNr();
    auto b = event->GetB();
    auto n_part = event->GetNpa();
    auto Ebeam = 1.23;
    std::string line = std::to_string( evt_id )+"\t"+std::to_string( n_part )+"\t"+std::to_string( Ebeam )+"\t"+std::to_string( b )+"\t0";
    file_out << line << std::endl;
    auto n_particles = event->GetNpa();
    for( int j=0; j<n_particles; ++j ){
      auto particle = event->GetParticle(j);
      auto E = particle->E();
      auto px = particle->Px();
      auto py = particle->Py();
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
          default:
            continue;
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