
# Arch

sudo pacman -S alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama

wget https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_linux_amd64.tar.gz
tar -xvf raylib-5.5_linux_amd64.tar.gz
cd raylib-5.5_linux_amd64
sudo cp -r ./lib ./src /usr/ 
cd ..
rm -rf raylib-5.5_linux_amd64

