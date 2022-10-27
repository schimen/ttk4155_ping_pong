{ pkgs ? import <nixpkgs> {} }:
let
  avarice = pkgs.callPackage ./avarice.nix {};
in
pkgs.mkShell {
  buildInputs = with pkgs; [
    pkgsCross.avr.avrlibc
    pkgsCross.avr.buildPackages.binutils
    pkgsCross.avr.buildPackages.gcc
    avrdude
    avarice
    gcc-arm-embedded
    killall
    openocd
    doxygen
  ];
}
