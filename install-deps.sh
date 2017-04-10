distro=$(cat /etc/*release*)
distro=${distro,,}

if [[ $distro == *"arch"* || $distro == *"manjaro"* ]]
then
	sudo pacman -S cmake base-devel libx11 libjpeg curl glib2 gdk-pixbuf2 libnotify xclip
elif [[ $distro == *"ubuntu"* || $distro == *"debian"* ]]
then
	sudo apt-get install cmake libx11-dev libjpeg-dev libcurl4-gnutls-dev libglib2.0-dev libgdk-pixbuf2.0-dev libnotify-dev xclip
fi
