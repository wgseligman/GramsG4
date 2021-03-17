/// \file LArHit.cc
/// \brief Implementation of the LArHit class

/// As it says in GramsG4LArHit.hh, this is duplicated
/// from Geant4 example B2a.

#include "GramsG4LArHit.hh"

#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

namespace gramsg4 {

  G4ThreadLocal G4Allocator<LArHit>* LArHitAllocator=0;

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  LArHit::LArHit()
    : G4VHit() 
    , m_trackID(-1)
    , m_pdgCode(0)
    , m_numPhotons(-1)
    , m_energy(0.)
    , m_time(0.)
    , m_position(G4ThreeVector())
  {}

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  LArHit::~LArHit() {}

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  LArHit::LArHit(const LArHit& right)
    : G4VHit()
  {
    m_trackID     = right.m_trackID;
    m_pdgCode     = right.m_pdgCode;
    m_numPhotons  = right.m_numPhotons;
    m_energy      = right.m_energy;
    m_time        = right.m_time;
    m_position    = right.m_position;
    m_identifier  = right.m_identifier;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  const LArHit& LArHit::operator=(const LArHit& right)
  {
    m_trackID     = right.m_trackID;
    m_pdgCode     = right.m_pdgCode;
    m_numPhotons  = right.m_numPhotons;
    m_energy      = right.m_energy;
    m_time        = right.m_time;
    m_position    = right.m_position;
    m_identifier  = right.m_identifier;

    return *this;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  G4bool LArHit::operator==(const LArHit& right) const
  {
    return ( this == &right ) ? true : false;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


  // This is standard routine for drawing hits in the visualizer.
  // You'll find almost identical code in chapter 8.5.6 of the 
  // Geant4 Applications Guide. 
  void LArHit::Draw()
  {
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(pVVisManager)
      {
	G4Circle circle(m_position);
	circle.SetScreenSize(4.0);
	circle.SetFillStyle(G4Circle::filled);
	G4Colour colour(1.,0.,0.); // red
	G4VisAttributes attribs(colour);
	circle.SetVisAttributes(attribs);
	pVVisManager->Draw(circle);
      }
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void LArHit::Print()
  {
    auto eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

    G4cout 
      << " LArHit: eventID=" << eventID
      << " trackID=" << m_trackID 
      << " PDG=" << m_pdgCode
      << " numPhotons=" << m_numPhotons
      << " Edep="
      << std::setw(7) << G4BestUnit(m_energy,"Energy")
      << " Time="
      << std::setw(7) << G4BestUnit(m_time,"Time")
      << " Position="
      << std::setw(7) << G4BestUnit(m_position,"Length")
      << " Identifier="
      << m_identifier
      << G4endl;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

} // namespace gramsg4
