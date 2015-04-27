# Multistereo

A C-program based on [PortAudio](http://portaudio.com) to apply binaural filters on 5-channels audio signals.

## Install

You should start by installing `portaudio`. See [this link](http://portaudio.com/download.html) to do so.

[`libsndfile`](http://www.mega-nerd.com/libsndfile/) should also be installed.

If you're running Mac OS X, you can use `brew install portaudio`and `brew install libsndfile` to get these two libraries installed, after having installed [`Brew`](http://brew.sh).

Then, compile the project files as usual.

## Usage

Launch the program with `./multistereo`.

It should automatically detect the first five inputs of your soundcard and convolve it with left and right HRTF.  
If you have no soundcard with this number of I/O and are running Mac OS X, you can use *soundflower*, formerly developped by *Cycling74*.

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
The `binauralize` function is prototyped and will be based on "brute convolution", but may be based on FFT once a first working version will be produced.

## Contributing

Any help is welcome to improve this project.

The project is firstly aimed to work with 5.1 sound signals and with "brute" convolution.  
Different upgrades may include:

* support for more multichannel sound formats (up to 22.2)
* convolution by FFT or block convolution algorithms to increase performances

## License

MIT © [Jean Cruypenynck](nonatomiclabs.com/blog)