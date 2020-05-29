# GramsG4

If you want a formatted (or easier-to-read) version of this file, scroll to the bottom.

## Installing GramsG4
    
### Working with github

Visit <https://github.com/> and sign up for an account if you don't already have one.

I strongly advise you to use SSH as the means to access a repository, 
and to [set up an SSH key](https://help.github.com/articles/generating-an-ssh-key/),
otherwise you'll have to type in a password every time you issue a git
request against the repository. 

Once this is done, you can download a copy of the GramsG4 repository:

    git clone git@github.com:wgseligman/GramsG4.git
    cd GramsG4
    git fetch
    git checkout develop

### Prerequisites

If you're working on a system of the [Nevis Linux cluster](https://twiki.nevis.columbia.edu/twiki/bin/view/Main/LinuxCluster), just type

    module load cmake root geant4

and skip to the next section. Otherwise, read on.

You will need recent versions of:

   - [Cmake](https://cmake.org/) (at least version 3.4, perhaps higher)
   - [ROOT](https://root.cern.ch/) (verified to work with ROOT 6.16 and higher)
   - [Geant4](http://geant4.web.cern.ch/) (verified to work with Geant4 10.5.0 and higher)
   
You will also need the development libraries for:

   - [GNU C++](https://gcc.gnu.org/) (version 6.2 or higher, though the compilation might work with [clang](https://clang.llvm.org/))
   - [Xerces-C](https://xerces.apache.org/xerces-c/)
   - [OpenGL](https://www.opengl.org/)
   - [QT4](https://www.qt.io/)
   
On RHEL-derived Linux distributions (e.g., Scientific Linux, CentOS) you can install these development packages with:

    sudo yum install gcc-c++ glibc-devel \
       freeglut-devel xerces-c-devel \
       qt-devel mesa-libGLw-devel
 
### Prepare your local computer 
   
If you are working remotely (e.g., on a laptop), and you want to use
the interactive display, you will also need to
install and/or activate both X-Windows and OpenGL for your local
computer. You can find instructions
[here](https://www.hoffman2.idre.ucla.edu/access/x11_forwarding/).

## Running GramsG4

To make things easier, I'm going to define a variable for the repository
directory in your area. You may want to include a suitably modified version
of this command in one of your shell startup files:

    # $GGDIR is the directory that contains GramsG4, not GramsG4 itself.
    cd ..
    export GGDIR=$PWD

To build/compile:

    # Set up ROOT and Geant4 as appropriate for your system
    # For example, at Nevis type
    module load cmake root geant4
    
    # Create a separate build/work directory. This directory should
    # not be the GramsG4 directory or a sub-directory of it.
    cd $GGDIR
    mkdir GramsG4-work
    cd GramsG4-work
    
    # Build
    cmake $GGDIR/GramsG4
    make

To run the simulation:

    # After succesfully making the executable "gramsg4", you can run it
    # in the build directory:
    ./gramsg4

If no one has changed the defaults (see **Program Options** below), the Geant4 simulation
will execute the contents of the file `run.mac`. If you'd like to see an interactive
display:

    ./gramsg4 --ui
     
### Making changes

When you want to start editing code for the first time:

    cd $GGDIR/GramsG4
    git flow init 

You'll be asked a bunch of questions. The answer to the first one
"Branch name for production releases:" is 'develop' (without the
quotes). Accept the defaults for everything else.

If you get a message that git flow is not recognized, it means that
the gitflow package has to be installed on your machine.   

You only have to type `git flow init` once, the first time you edit
code in that directory. Don't type it again, unless you git clone a
brand-new repository.

When you want to start on a new task:

    git flow feature start $USER_MyMagnificentFeatureName

The '$USER_' means that your feature name should begin with your 
account name. That way you know who is doing what. 

To "bookmark" changes you've made to your copy of the `$GGDIR/GramsG4` directory:

    git add my_file.cc # if you create a new file
    git commit -m "Comment about your changes"

To incorporate changes that other people have made and checked into
the repository:

    git fetch origin
    git rebase origin/develop

When you've finished working on your task (after testing it, putting
in the comments, writing the documentation, discussing it at a group
meeting, etc.):

    git flow feature finish
    git push origin/develop

If you want other people to see your feature work without making
changes to the develop branch:

    git flow feature publish $USER_MyMagnificentFeatureName

### Work files

The `gramsg4` program requires several input files (e.g., `grams.gdml`, `options.xml`).
These are copied from the GramsG4 directory to your work/build directory when you executed the `cmake` command. Go ahead and make
any changes you want to these work files; they won't affect the original files in 
`$GGDIR/GramsG4`. 

If you feel that your changes to these work files should become part of the git
repository, be sure to copy the changed work files to `$GGDIR/GramsG4` and
"bookmark" them as described above. 

### Development "flow"

What may not be clear from the above is that you'll typically "sit" in your
work/build directory. You'll run `gramsg4`, make changes, then run it again. 

If you want to change the program code, edit the C++ files in `$GGDIR/GramsG4`;
e.g.,

    cd $GGDIR/GramsG4-work
    emacs ../GramsG4/include/MyPhysicsList.hh
    make
    ./gramsg4 
    
If you want to change the work files, just edit them in your build directory:

    emacs grams.gdml
    
You do *not* have to execute the `cmake` command if you edit the C++ files. 
However, if you edit or change the work files in `$GGDIR/GramsG4`, you *will* have 
to type

    cd $GGDIR/GramsG4-work
    cmake $GGDIR/GramsG4

again to copy the revised work files.

When you've made your changes and wish to "bookmark" them:

    cd $GGDIR/GramsG4
    git commit -m "Comment about your changes"

*Because I am lazy, I usually do something like this:*

    (cd ../GramsG4; git commit -m)
    
*and use the up-arrow key to re-invoke that command after each development milestone.*

## Detector geometry

A version of the detector geometry is defined in `grams.gdml`. As the extension implies,
it is written in the geometry-definition language [GDML](http://lcgapp.cern.ch/project/simu/framework/GDML/doc/GDMLmanual.pdf).

If you want to make changes to the detector geometry (including the colors used for
the `--ui` interactive display), edit `grams.gdml`. If you're having trouble 
understanding the contents of the file:

   - Read the comments within the file.
   - Refer to the [GDML](http://lcgapp.cern.ch/project/simu/framework/GDML/doc/GDMLmanual.pdf) manual.
   - Refer to the [Geant4 Applications Guide](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/html/), especially the [geometry section](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/html/Detector/Geometry/geometry.html) which explains the difference between solids, logical volumes, and physical volumes. 

## Program options

Short version: look at `options.xml`. 

If you want to start working with the program options, read the
next couple of sections. Otherwise, skip to physics lists below.  

### Options XML file - details

What you'll first notice that is that within the global `<parameters>` tag there are
at least two sections. It looks something like this:

```
<parameters>
  <global>
    <option [...] />
    <option [...] />
  </global>

  <gramsg4>
    <option [...] />
    <option [...] />
  </gramsg4>
</parameters>
```

The idea is that this same options XML file might eventually be used by more programs
in this analysis. The `<global>` section contains parameters that will apply to more
than one program. Individual programs will have their options in their own sections.

```
Anatomy of <option> tag: 

   name  - used on the command line, and by the program
           to fetch the value of the option
           
   short - optional one-character abbreviation for
           a short command-line option
           
   value - the number/text/bool used by the program;
           can be overriden on the command line

   type  - string/bool/integer/double
   
   desc  - optional; brief description of the option (keep it 
           less than 20 characters) ; used in the --help|-h message
```

Note that the options in this XML file can be overridden by command-line options.
However, the contents of this file _define_ those command-line options. For example:

```  
  <option name="energyCut" value="12.5" type="double" desc="muon energy cut [MeV]"/>
```

means that you can set energyCut by either editing the XML file, or by using the
option on the command line; e.g.:

    gramsg4 --energyCut 15.6

The default name and location of the XML file is "options.xml" in the
current directory. You can override this on the command line; e.g.:

    gramsg4 myOptions.xml  

If you're going to use that format, the XML file name must be the
first argument. You can also supply the XML file name via the
--options option, e.g.:

    gramsg4 --options myWorkDirectory/myOptions.xml

Of course, you can mix and mash to your heart's content:


    gramsg4 --energyCut 199.9 --options myEnergyStudyOptions.xml

If a job option has `type="boolean"`, then on the command line it takes no
arguments; either it's there or it isn't. For example, if this is
in the XML file:

```  
  <option name="makeHistograms" value="false" type="bool" desc="make my special hists"/>
```  

then you could do this on the command line:

 
    gramsg4 --makeHistograms --energyCut 123.45                      


You can define one-character short options:

```
  <option name="energyCut" short="e" value="12.5" type="double" />
```
Then you can do:

    gramsg4 -e 123.45 

Be careful not to overuse the short options, since they can make the
command harder to understand. 

If you duplicate the short character between different options the behavior is
unpredictable. However, case is signficant; e.g., you can do this:

```
  <option name="energyMin" short="e" value="12.5" type="double" desc="min pion energy [MeV]"/>
  <option name="energyMax" short="E" value="125.0" type="double" desc="max pion energy [Mev]"/>
```  

The usual UNIX shell flexibility is available with these options. Assume this line is in 
the options XML file. 

```
    <option name="nthreads" short="t" value="0" type="integer" desc="number of threads"/>
```

Then all of the following are equivalent:

    gramsg4 --nthreads 5
    gramsg4 --nthreads=5
    gramsg4 -t 5
    gramsg4 -t5

### Programming new options

Just having an option defined in the XML file is not enough.
You need the programming to do something with that option.
Within the code, you can access the value of a given option:

```
  std::string optionValue; /* ... or int or double or bool ... */
  auto success = GramsG4Options::GetInstance()->GetOption("option-name",optionValue);
  if (success) { ... do whatever with optionValue ... }
  else { there is no option with name "option-name" that is of the type of optionValue }
```  

For example:

```  
  auto options = GramsG4Options::GetInstance();
  G4double myCut;
  auto success = options->GetOption("energyCut",myCut);
  if (success) { 
     ... do whatever with myCut ... 
  }
  else {
     G4ExceptionDescription description;
     description << "There is no floating-point option defined for 'energyCut'";
     G4Exception("myRoutine::myMethod","option missing",
        JustWarning, description);
  }

```

Units are whatever you want them to be, but it's probably safer to
assume Geant4's standard units as defined in `SystemOfUnits.h`; these come from [CLHEP](https://proj-clhep.web.cern.ch/proj-clhep/). In particular:


```
  distance: mm
  time:     ns
  energy:   MeV
```

## Physics lists and how to extend them

If you looked at `options.xml` you saw an intriguing option (the list may not be `FTFP_BERT`):


```
    <option name="physicslist" short= "p" value="FTFP_BERT" type="string" desc="physics list"/>
```

The program uses the extensible physics-list factory described in a
Geant4 example; at Nevis, this example can be found at
`$G4INSTALL/share/*/examples/extended/physicslists/extensibleFactory`.
If you run the program using the "&#x2011;&#x2011;showphysicslists" option (or simply
"&#x2011;l" [that's "dash&#x2011;ell", not "dash&#x2011;one"]), you'll see all the available physics
lists and modifiers. 

Here's the first part of the output when I run this command with Geant4 10.5.1:
```
gramsg4 -l
    
Available physics lists:
Base G4VModularPhysicsLists in G4PhysListRegistry are:
 [  0]  "FTFP_BERT"
 [  1]  "FTFP_BERT_ATL"
 [  2]  "FTFP_BERT_HP"
 [  3]  "FTFP_BERT_TRV"
 [  4]  "FTFP_INCLXX"
 [  5]  "FTFP_INCLXX_HP"
 [  6]  "FTFQGSP_BERT"
 [  7]  "FTF_BIC"
 [  8]  "G4GenericPhysicsList"
 [  9]  "LBE"
 [ 10]  "MySpecialPhysList"
```

and so on.

### Using the available physics lists

If you've gotten this far, you're probably overwhelmed with the choices.
Your first question is probably "What does each of them do?"
Unfortunately, at the time of this writing (May-2020) there isn't one
single source that provides an answer. Here are some links to get you
started:

* [Short Guide to Choosing Your Physics List](https://indico.cern.ch/event/776050/contributions/3241826/attachments/1789270/2914266/ChoosingPhysLists.pdf) (PDF presentation)
* [G4 Physics List Guide](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/PhysicsListGuide/BackupVersions/V10.5-2.0/html/physicslistguide.html) (incomplete)

If you're just starting out, the Geant4 collaboration's recommendations for initial tests is `FTFP_BERT`. 
Here it is set explicitly on the command line:


    ./gramsg4 --physicslist FTFP_BERT

If your research indicates you should try one of the other base physics lists, e.g., `QBBC`


    ./gramsg4 --physicslist QBBC

Perhaps you decide that you want to try one of the alternative models for EM physics, listed under *Replacement mappings* in the full output of &#x2011;&#x2011;showphysicslists:

    ./gramsg4 --physicslist QBBC_EMV

If you need to include one or more of the additional constructors, e.g., `G4OpticalPhysics`:

    ./gramsg4 --physicslist QBBC_EMV+G4OpticalPhysics

### Custom physics lists

If you look at the above list carefully, you saw `MySpecialPhysList`
among the available lists. This comes from the Geant4 extensibleFactory
example, and can serve as a basis for your own fine-tuning of a physics
list. To understand what you need to do to add your own custom list, see
these files:

```
GramsG4/gramsg4.cc
GramsG4/include/MySpecialPhysList.hh
GramsG4/include/MySpecialPhysList.icc
```

These second two files are unaltered from the Geant4 extensibleFactory
example. As it stands, they just implement the QBBC physics list without
any changes.

To get some idea of what and how you can change in your own custom
physics list, see the Geant4 examples. To find relevant files, at Nevis
I use the UNIX `find` command:

    find $G4INSTALL/share/*/examples/ -name \*PhysicsList\*

This will show you many places to get started!

## References

   - git: <https://git-scm.com/doc> 
   - git flow: <http://danielkummer.github.io/git-flow-cheatsheet/>

Some git tips from other collaborations:  
   - [git flow quick start guide](https://cdcvs.fnal.gov/redmine/projects/cet-is-public/wiki/Git_flow_quick_start)
   - [while you are developing](https://cdcvs.fnal.gov/redmine/projects/uboonecode/wiki/Uboone_guide#While-you-are-developing)

Toolkits:
   - [Geant4 Manual](http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/)
   - [ROOT Tutorial](https://www.nevis.columbia.edu/~seligman/root-class/)


## Viewing a Markdown document

This document is written in
[Markdown](https://www.markdownguide.org/cheat-sheet/), a tool for
formatting documents but still keeping the unformatted versions
readable.

If you want to read a formatted version of this document (so you're
spared the funny backticks and hashtags and whatnot), do a web search on
"Markdown viewer" to find a suitable program. For example, at Nevis, all
the Linux cluster systems have [pandoc](https://pandoc.org/) installed.
You can view a plain text version of this document with:

    pandoc README.md -t text | less

You can type `man pandoc` to learn more formatting options. For example,
if you want to format this into PDF:

    pandoc README.md -o README.pdf
