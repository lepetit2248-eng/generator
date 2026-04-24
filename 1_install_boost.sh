#!/bin/bash
echo "Boost files copy, please wait ..."
sudo cp boost/boost_1_72_0.tar.bz2 /usr/local/
cd /usr/local/
echo "Boost files extraction, please wait ..."
sudo tar --bzip2 -xf boost_1_72_0.tar.bz2
cd boost_1_72_0
sudo ./bootstrap.sh --with-libraries=serialization
sudo ./b2
sudo ./b2 headers
cd ../
sudo rm boost_1_72_0.tar.bz2
