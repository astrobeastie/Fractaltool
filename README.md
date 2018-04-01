# Fractaltool
This project includes two different tools:
## Fractalviewer
Fractalviewer is a GUI application used to explore the Mandelbrot set as well as the Julia sets corresponding to arbitrary points
on the complex plane. It offers different rendering algorithms and gradients to color the images.New gradients can also be defined in
separate files.

## Fractaltool
This tool can be used to render high quality images of the Mandelbrot- and Juliasets. It can also generate sets of images that can be
stitched together (e.g. with ffmpeg) to  create animations.

### Usage

Generate an image of the Mandelbrot set:
$ fractaltool mandel -c center -z magnification -o outputfile

Generate an image of the Julia set:
fractaltool julia -s parameter -c center -z magnification -o outputfile 

Generate a set of images zooming into the Mandelbrot set:
$ fractaltool mandelzoom -c center -e maxzoom -o outputformat

Generate a set of images of Julia sets with gradually changing parameter:
$ fractaltool juliamorph -b startingpoint -e endpoint -o outputformat
