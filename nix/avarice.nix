{ stdenv, lib, fetchurl,
  automake, autoconf, sedutil,
  perl, libbfd
}:
stdenv.mkDerivation rec {
  pname = "avarice";
  version = "2_14";
  src = fetchurl {
    url = "https://github.com/avrdudes/${pname}/archive/refs/tags/${pname}-rel-${version}.tar.gz";
    sha256 = "0dpx6v2ax728pmskdw2n2l43xw1a06fsqv7jf0iskws3qyf9bnrg";
  };
  nativeBuildInputs = [
    sedutil
    automake
    autoconf
  ];
  buildInputs = [
    perl
    libbfd
  ];
  configurePhase = ''
    sed -i 's/__unused/__attribute__((unused))/g' src/jtag3io.cc src/jtag3prog.cc
    ./Bootstrap
    ./configure
  '';
  installPhase = ''
    mkdir $out
    make install prefix=$out
  '';
  meta = with lib; {
    description = "Program for interfacing the Atmel JTAG ICE to GDB for debugging AVR devices.";
    homepage    = "https://github.com/avrdudes/avarice";
    license     = licenses.gpl2;
    platforms   = platforms.linux;
    maintainers = [ ];
  };
}