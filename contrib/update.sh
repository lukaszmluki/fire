#!/bin/bash

if [ ! -d "ffmpeg" ]; then
  git clone https://github.com/lukaszmluki/ffmpeg.git ffmpeg
  pushd ffmpeg
  git checkout -b fire origin/fire 
  popd
fi
pushd ffmpeg
git fetch origin
git checkout fire
git reset --hard origin/fire
./configure  --prefix=../../external --disable-programs --disable-doc --enable-libpulse
make -j16
make install
popd



