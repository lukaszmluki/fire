#!/bin/bash

die() {
  echo -e "\nContrib contiguration failed";
  exit 1;
}

DEBUG="--disable-debug"

if [ $1="debug" ]; then
   DEBUG="--enable-debug=3 --disable-optimizations --disable-stripping"
fi

if [ ! -d "ffmpeg" ]; then
  git clone https://github.com/lukaszmluki/ffmpeg.git ffmpeg
  pushd ffmpeg
  git checkout -b fire origin/fire 
  popd
fi
pushd ffmpeg
git fetch origin
git checkout fire
git reset --hard origin/libavengine
./configure  --prefix=../../external \
             --disable-programs --disable-doc \
             --enable-libpulse \
             --enable-libssh \
             --enable-opengl \
             --disable-muxers \
             --disable-encoders \
             --disable-bsfs \
             --disable-indevs \
             --disable-outdev=xv \
             --disable-outdev=sdl \
             --disable-outdev=fbdev \
             --enable-indev=lavfi \
             --enable-encoder=pcm_alaw \
             --enable-encoder=pcm_f32be \
             --enable-encoder=pcm_f32le \
             --enable-encoder=pcm_f64be \
             --enable-encoder=pcm_f64le \
             --enable-encoder=pcm_mulaw \
             --enable-encoder=pcm_s16be \
             --enable-encoder=pcm_s16le \
             --enable-encoder=pcm_s24be \
             --enable-encoder=pcm_s24daud \
             --enable-encoder=pcm_s24le \
             --enable-encoder=pcm_s32be \
             --enable-encoder=pcm_s32le \
             --enable-encoder=pcm_s8 \
             --enable-encoder=pcm_u16be \
             --enable-encoder=pcm_u16le \
             --enable-encoder=pcm_u24be \
             --enable-encoder=pcm_u24le \
             --enable-encoder=pcm_u32be \
             --enable-encoder=pcm_u32le \
             --enable-encoder=pcm_u8 \
             --enable-encoder=rawvideo \
             ${DEBUG} || die

make -j16
make install
popd



