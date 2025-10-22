# hardyDarty

# use this as an inital read-me for how to use this repo (for the time being)
VSC setup -> https://www.youtube.com/watch?v=EbDuTtqA8-k

Intro to git:

-This is properly optimised to be used on Visual Studio Code
-Please view this link to install VSC: https://code.visualstudio.com/docs/setup/setup-overview
  Also lmk if you have any issues with installation!

-Once installed download the following libraries:
-Makefile Tools (by Microsoft)
-CodeLLDB (by Vadim Chugunov)
-C/C++ (by Microsoft)
-C/C++ Clang Command Adapter (by Yasuaki MITANI)
-C/C++ Extension Pack (by Microsoft)
-PlatformIO( by PlatformIO IDE)
-clangd (by LLVM)
  These should be all the libraries required, if it prompts you to download a separate one please do so and lmk the name to add to this list!

-Installing Git:
-Open CMD or Terminal and follow along the instructions for installing Brew
Brew: https://brew.sh/
-Once installed type 'brew install git'

Using Git in VSC:
-Open your terminal and type in: 
git clone https://github.com/raniamaaraba/HardyDarty.git
-Make sure you let me know at this point your github username so I can add you to the editor list
-Quick code for using the repositiory:
to make changes
git add .
git commit -m "Add new feature"
git push -u origin new-feature
git checkout -b new-branch-name
git switch branch-name

PlatformIO:
if you do not see the side bar alien then do the following command:
cmd/crtl shift p
platformio: home

BEWARE OF ISSUES WITH CLANG

PLEASE ENSURE YOU ARE PUSHING TO THE CORRECT LOCATION AND NOT OVERRIDING BASE SOURCE CODE!!!



