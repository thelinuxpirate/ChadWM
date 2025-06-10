{ app, nixpkgs }:

let
  commonBuild = src:
    nixpkgs.stdenv.mkDerivation {
      pname = "${app}";
      version = "1.0";

      src = ./${app};

      nativeBuildInputs = [
        nixpkgs.patchelf
        nixpkgs.autoPatchelfHook
        nixpkgs.pkg-config
      ];

      buildInputs = [
        nixpkgs.xorg.libX11
        nixpkgs.xorg.libXft
        nixpkgs.xorg.libXi
        nixpkgs.xorg.libXinerama
        nixpkgs.xorg.xrdb
        nixpkgs.alsa-lib
      ];

      patchPhase = ''
        substituteInPlace config.mk \
          --replace "-I/usr/X11R6/include" "" \
          --replace "-I/usr/include/freetype2" "" \
          --replace "-march=native" ""

        substituteInPlace Makefile \
          --replace "CFLAGS =" "CFLAGS += \$(shell pkg-config --cflags x11 xft xinerama xi)\nCFLAGS +=" \
          --replace "LDFLAGS =" "LDFLAGS += \$(shell pkg-config --libs x11 xft xinerama xi)\nLDFLAGS +="
      '';

      installPhase = ''
        make PREFIX=$out install
      '';

      meta = with nixpkgs.lib; {
        description = "Builds ${app}";
        license = licenses.gpl3Plus;
        platforms = platforms.unix;
      };
    };
in
  commonBuild ./${app}
