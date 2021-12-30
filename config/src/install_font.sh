#!/bin/sh
#Download fonts
mkdir -p ~/Downloads/fonts_MesloLGS
cd ~/Downloads/fonts_MesloLGS
wget https://github.com/romkatv/powerlevel10k-media/raw/master/MesloLGS%20NF%20Regular.ttf
wget https://github.com/romkatv/powerlevel10k-media/raw/master/MesloLGS%20NF%20Bold.ttf
wget https://github.com/romkatv/powerlevel10k-media/raw/master/MesloLGS%20NF%20Italic.ttf
wget https://github.com/romkatv/powerlevel10k-media/raw/master/MesloLGS%20NF%20Bold%20Italic.ttf
sudo chmod 777 *
cd ..
#Install fonts
sudo mv fonts_MesloLGS /usr/share/fonts/
sudo mkfontscale
sudo mkfontdir
sudo fc-cache -fv