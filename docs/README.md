# Docs

This folder contains documentation for the lag in TTK4155.

## Build documentation

To build documentation, please install:
- [Doxygen](https://www.doxygen.nl/)
- [Sphinx](https://www.sphinx-doc.org/en/master/)
- [Breathe](https://breathe.readthedocs.io/en/latest/index.html)
- [GNU Make](https://www.gnu.org/software/make/)

To build documentation, first move to the [docs](./docs/) folder and build Doxygen XML documentation via the command
```sh
doxygen Doxyfile
```

Then you can either build html documentation via the command
```sh
make html
```

Or pdf via the command (this requires [texlive](https://tug.org/texlive/))
```sh
make latexpdf
```

## Read documentation

After building, the documentation can be found in the build folder in [docs](./docs).

Here you can either open the html documentation by opening `index.html` in your browser, 
or open the pdf by opening `byggern.pdf` in your pdf reader.