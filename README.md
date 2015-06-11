# Multistereo

A C-program based on [PortAudio](http://portaudio.com) and [FFTW](http://www.fftw.org) to apply binaural filters on 5-channels audio signals.

## Install

You should start by installing `portaudio`, `libsndfile` and `FFTW`. See these links ot do so:

* `PortAudio`: [http://portaudio.com/download.html](http://portaudio.com/download.html)
* `libsndfile`: [http://www.mega-nerd.com/libsndfile/#Download](http://www.mega-nerd.com/libsndfile/#Download)
* `FFTW`: [http://www.fftw.org/download.html](http://www.fftw.org/download.html)

If you're running Mac OS X, you can run

	brew install portaudio
	brew install libsndfile
	brew install fftw3
	
to get these three libraries installed, after having installed [`Brew`](http://brew.sh).

Then, compile the project files using the `make` command or using:

	gcc multistereo.c main.c -lportaudio -lfftw3f -lsndfile -o multistereo

## Usage

Launch the program with `./multistereo` and it will display you the usage which is the following:

* list available audio devices with the flag `-devs`
* binauralize an input signal using:
	* *I/O that you select* with the flag `-binauralize <inputIndex> <outputIndex>`
	* *default I/O* with the flag `-binauralize default`
	In each case, the flag should be followed by the absolute or relative path to your impulse response files.
* binauralize an input file using the flag `-binauralize file` followed by the path to your impulse responses and the path to your multichannel file

Your impulse responses should be named as the following: `IR_LL.wav`, `IR_RL.wav`, `IR_LC.wav`, `IR_RC.wav`, `IR_LR.wav`, `IR_RR.wav`, `IR_LRs.wav`, `IR_RRs.wav`, `IR_LLs.wav`, `IR_RLs.wav`.

## Documentation

`portaudio` works in two different ways:

* read/write I/O method
* callback I/O method

For this project, I decided to use the callback method.  
It consists in:

1. writing our `main` function in which audio streams are opened and closed, error handled, etc.
2. writing a "callback" function which will be called every time that audio is needed by the program.
3. writing other functions called by our callback function.

In our case, the callback function (defined in `MSCallback.h/.c) basically sends each of our 5 inputs in its corresponding left and right binaural transfer function.  
The transfer function in itself is described in another file: `binauralize.h/.c`

For now, only the main and callback functions are established.  
The `binauralize` function is based on FFT.

## Contributing

Any help is welcome to improve this project.

The project is firstly aimed to work with 5.1 sound signals.  
Different upgrades may include:

* auto-truncate impulse responses to the buffer size
* support for more multichannel sound formats (up to 22.2)

## License

MIT © [Jean Cruypenynck](nonatomiclabs.com/blog)