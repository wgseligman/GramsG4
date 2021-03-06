//
/// \file GramsG4/gramsg4.cc
/// \author William Seligman (seligman@nevis.columbia.edu)
///
// --------------------------------------------------------------
//      GEANT 4 - gramsg4
// --------------------------------------------------------------

// For processing command-line and XML file options.
#include "Options.h" // in util/

// There are three mandatory classes needed for any Geant4 application
// to work. One is the detector geometry:
#include "GramsG4DetectorConstruction.hh"

// The other two are a way to create or fetch primary events, and
// something for the simulation to with its events. These are both
// defined here:
#include "GramsG4ActionInitialization.hh"

// Classes required for the UserActionManager.
// See these header files for a description
// of what a "user action manager" is and
// why it's useful. 
#include "UserAction.h"        // in g4util/
#include "UserActionManager.h" // in g4util/

// The user-action class that will write output.
#include "GramsG4WriteNtuplesAction.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4TransportationManager.hh"
#include "Randomize.hh"

// Refer to examples/extended/physicslists/extensibleFactory within
// the Geant4 installation for details about the physics-list
// factory. All the physics-list code (including the stubs for example
// user physics lists) was pulled from there.
#include "G4PhysListFactoryAlt.hh"
// Allow ourselves to extend the short names for physics ctor
// addition/replace along the same lines as EMX, EMY, etc
#include "G4PhysListRegistry.hh"

// For turning on optical photons.
#include "G4OpticalPhysics.hh"

// Allow ourselves to give the user extra info about available physics ctors
#include "G4PhysicsConstructorFactory.hh"

// Pull in a user defined physics list definition into the main
// program and register it with the factory (doesn't have to be the
// main program but the .o containing the declaration _must_ get
// linked/loaded)
#include "G4PhysListStamper.hh"  // defines macro for factory registration
#include "MySpecialPhysList.hh"
G4_DECLARE_PHYSLIST_FACTORY(MySpecialPhysList);

// --------------------------------------------------------------

// forward declaration.
void PrintAvailablePhysics();

int main(int argc,char **argv)
{
  // Initialize the options from the XML file and the
  // command line. Make sure this happens first!
  auto options = util::Options::GetInstance();

  // The third argument of ParseOptions, 'gramsg4', is the name of the
  // tag to use for this program's options. See options.xml and/or
  // README.md to see how this works.
  auto result = options->ParseOptions(argc, argv, "gramsg4");

  // Abort if we couldn't parse the job options.
  if (result) G4cout << "ParseOptions succeeded" << G4endl;
  else {
    G4ExceptionDescription description;
    description << "File " << __FILE__ << " Line " << __LINE__ << " " << std::endl
		<< "GramsG4: Aborting job due to failure to parse options";
    G4Exception("GramsG4 main()","invalid options",
		FatalException, description);
    exit(EXIT_FAILURE);
  }

  // We may not be debugging this at this moment, but if we
  // want to use the debug flag later in this routine,
  // let's have it ready.
  G4bool debug;
  options->GetOption("debug",debug);

  // Check for help message.
  G4bool help;
  options->GetOption("help",help);
  if (help) {
    options->PrintHelp();
    exit(EXIT_SUCCESS);
  }

  // Does the user just want to see the physics lists?
  G4bool showLists;
  options->GetOption("showphysicslists",showLists);
  if (showLists) {
    PrintAvailablePhysics();
    exit(EXIT_SUCCESS);
  }

  G4bool verbose;
  options->GetOption("verbose",verbose);
  if (verbose) {
    // Display all program options and physics lists.
    options->PrintOptions();
    PrintAvailablePhysics();
  }
  
  // Set up the Geant4 Run Manager. 
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  G4int nThreads;
  options->GetOption("nthreads",nThreads);
  if ( nThreads <= 0 ) nThreads = 1;
  if (verbose) G4cout << "GramsG4::main(): Setting number of worker threads to "
		      << nThreads << G4endl;
  runManager->SetNumberOfThreads(nThreads);
#else
  G4RunManager* runManager = new G4RunManager;
#endif
  
  // g4alt::G4PhysListFactory is the extensible factory
  g4alt::G4PhysListFactory factory;
  
  // This is an example of how to assign an alternate name to an
  // existing physics-list extension.  For example, after the
  // following definitions, you could specific a physics list by
  // "FTFP_BERT+OPTICAL" instead of "FTFP_BERT+G4OpticalPhysics".
  auto plreg = G4PhysListRegistry::Instance();
  plreg->AddPhysicsExtension("OPTICAL","G4OpticalPhysics");
  plreg->AddPhysicsExtension("STEPLIMIT", "G4StepLimiterPhysics");
  plreg->AddPhysicsExtension("RADIO","G4RadioactiveDecayPhysics");
  plreg->AddPhysicsExtension("MYPHYSICS","MyG4PhysicsPhysics");
  if ( verbose ) {
    G4cout << "Extensible physics factory: adding extensions:" << G4endl
	   << "   OPTICAL   ===> G4OpticalPhysics" << G4endl
	   << "   STEPLIMIT ===> G4StepLimiterPhysics" << G4endl
	   << "   RADIO     ===> G4RadioactiveDecayPhysics" << G4endl
	   << "   MYPHYSICS ===> MyG4PhysicsPhysics" << G4endl
	   << G4endl;
  }
  
  G4String physicsList;
  options->GetOption("physicslist",physicsList);
  auto physics = factory.GetReferencePhysList(physicsList);
  if (verbose) G4cout << "GramsG4::main(): Setting physics list to "
		      << physicsList << G4endl;
  
  if ( physics == NULL )
    {
      G4ExceptionDescription description;
      description << "File " << __FILE__ << " Line " << __LINE__ << " " << std::endl
		  << "GramsG4: Could not find or interpret physics list option"
		  << G4endl
		  << "Use '" << argv[0] << " -l' to see all available lists";
      G4Exception("GramsG4 main()","invalid option",
		  FatalException, description);
      exit(EXIT_FAILURE);
    }

  // Include optical photons if requested.
  // This was copied from 
  // artg4tk/artg4tk/pluginActions/physicsList/PhysicsList_service.cc 

  G4OpticalPhysics* opticalPhysics = (G4OpticalPhysics*) physics->GetPhysics("Optical");
  if ( opticalPhysics != NULL ) {

    if (verbose) G4cout << "GramsG4::main(): Optical physics is on" << G4endl;

    // There are many more optical-physics options than the ones
    // listed below. At some point we may want to expand the user
    // options to allow for greater control. For now, duplicate the
    // defaults used by artg4tk in LArSoft.

    opticalPhysics->SetScintillationStackPhotons(false);
    G4bool disable;
    options->GetOption("noscint",disable);
    if (disable) {
      opticalPhysics->Configure(kScintillation,false);
      if (verbose) G4cout << "GramsG4::main(): Scintillation is off" << G4endl;
    }
    else {
      opticalPhysics->Configure(kScintillation,true);
      if (verbose) G4cout << "GramsG4::main(): Scintillation is on" << G4endl;
    }

    // For now, make absolutely sure that the Cerenkov process is
    // turned off.
    opticalPhysics->Configure(kCerenkov,false);
    opticalPhysics->SetCerenkovStackPhotons(false);
    
  } // if opticalphysics
 
  /// NOTE: Almost certainly this code is not structured correctly
  /// for a multi-threaded application. 

  // Working with random numbers. First, define a RNG (Random Number
  // Generator). (For a list of engines, see
  // http://geant4.web.cern.ch/ooaandd/analysis/class_spec/global/randommisstat)
  // Note that I have no reason to expect that RanecuEngine is better
  // or worse than any of the other flat distributions.
  auto rngEngine = new CLHEP::RanecuEngine();

  // Did the user supply a random-number seed?
  // (Note that if they requested that the RNG
  // be restored from a file, this will be ignored.)
  G4int rngSeed;
  options->GetOption("rngseed",rngSeed);
  if ( rngSeed != 0 ) {
    if (verbose) G4cout << "GramsG4::main(): Setting RNG seed to " 
			<< rngSeed << G4endl;
    rngEngine->setSeed(rngSeed);
  }

  // Save the engine.
  G4Random::setTheEngine(rngEngine);

  // Working with random-number generator (RNG) states.
  G4String rngDirectory;
  options->GetOption("rngdir",rngDirectory);
  if ( ! rngDirectory.empty() ) {
    // The user wants to save/restore the RNG state.
    runManager->SetRandomNumberStoreDir(rngDirectory);
    runManager->SetRandomNumberStore(true);
    if (verbose) G4cout << "GramsG4::main(): Saving/restoring RNG states from "
			<< rngDirectory << G4endl;

    // For the meaning of 'rngPerEvent', see either
    // options.xml or $G4INSTALL/include/Geant4/G4RunManager.hh.
    G4int rngPerEvent;
    options->GetOption("rngperevent",rngPerEvent);
    if ( rngPerEvent != 0 ) {
      runManager->SetRandomNumberStorePerEvent(true);
      runManager->StoreRandomNumberStatusToG4Event(rngPerEvent);
    }

    // Does the user want to restore the RNG
    // state from a file?
    G4String rngRestoreFile;
    options->GetOption("rngrestorefile",rngRestoreFile);
    if ( ! rngRestoreFile.empty() ) {
      if (verbose) G4cout << "GramsG4::main(): restoring RNG state from "
			  << rngRestoreFile << G4endl;
      runManager->RestoreRandomNumberStatus(rngRestoreFile);
    }
  }

  // Usual initializations: detector, physics, and user actions.
  runManager->SetUserInitialization(new gramsg4::DetectorConstruction());
  runManager->SetUserInitialization(physics);

  // ***** Set up the User Action Manager *****
  // See the header files in directory g4util for a lengthy
  // description of this facility. 

  g4util::UserActionManager* uaManager = new g4util::UserActionManager();
  // The UserActionManager is itself a UserAction class.
  g4util::UserAction* uam = (g4util::UserAction*) uaManager;

  // Add this application's user actions to our user-action manager.
  uaManager->AddAndAdoptAction( new gramsg4::WriteNtuplesAction() );

  // Pass the g4util::UserActionManager to Geant4's user-action initializer.
  auto actInit = new gramsg4::ActionInitialization();
  actInit->SetUserActionLink(uam);
  runManager->SetUserInitialization(actInit);

  // ***** end User Action Manager setup *****

  // Tell Geant4 we're about to begin. 
  runManager->Initialize();

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
   
  const G4String command("/control/execute ");

  // Which macro file will we execute? If the user specified the UI
  // mode, then use the uimacrofile parameter. Otherwise use the
  // macrofile parameter. (This is to make it easier to switch between
  // batch mode and UI mode for testing.)

  G4bool uiMode(false);
  options->GetOption("ui",uiMode);
  G4String macroFile;
  if (uiMode) 
    options->GetOption("uimacrofile",macroFile);
  else
    options->GetOption("macrofile",macroFile);

  if (uiMode) {
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand(command+macroFile);
    ui->SessionStart();
    delete ui;
  }
  else
    UImanager->ApplyCommand(command+macroFile);

  // Clean-up
  delete visManager;
  delete runManager;
  return 0;
}

// --------------------------------------------------------------

void PrintAvailablePhysics() {
  G4cout << "Available physics lists:"
	 << G4endl;
  g4alt::G4PhysListFactory factory;
  factory.PrintAvailablePhysLists();
  
  G4cout << G4endl;
  G4cout << "Available physics ctors that can be added:"
	 << G4endl;
  auto g4pctorFactory =
    G4PhysicsConstructorRegistry::Instance();
  g4pctorFactory->PrintAvailablePhysicsConstructors();
}
