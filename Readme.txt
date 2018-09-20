###################################################################################################
###################### Corner Detection Tool for B231 FBK ver.0 ###################################
###################################################################################################

Build:
mkdir build
cd build
cmake ..
make

Usage: 
./CornerDetector <Corner Type [1](harris) [2](shi-tomasi)> <image file directory and name>

Result:
Results will be store in the current directory in txt format. They are the coordinates of detected corners' positions.

