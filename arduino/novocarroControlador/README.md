
sudo apt-get install libbluetooth-dev

sudo apt-get install libudev-dev libusb-1.0-0-dev libfox-1.6-dev

cirar 99-hidraw-permissions.rules em /etc/udev/rules.d
KERNEL=="hidraw*", SUBSYSTEM=="hidraw", MODE="0664", GROUP="plugdev"

# Instalar o n

npm install -g n

# mudar para a versao do node 

n v8.17.0

# voltar para o node latest

n stable  



n ls




################ OU ##################

nvm install v8.17.0
nvm use v8.17.0

nvm list    