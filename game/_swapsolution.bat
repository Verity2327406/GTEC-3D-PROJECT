echo This script will swap the solution files into or out of the project.
choice /M "Continue"
copy MyGame.cpp solution\MyGameC.cpp
copy MyGame.h solution\MyGameC.h
copy solution\MyGame.cpp MyGame.cpp
copy solution\MyGame.h MyGame.h
copy solution\MyGameC.h solution\MyGame.h
copy solution\MyGameC.cpp solution\MyGame.cpp
del solution\MyGameC.h
del solution\MyGameC.cpp
echo "Solutions swapped"