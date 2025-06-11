{ app, nixpkgs }:

let
  commonBuild = src:
    nixpkgs.stdenv.mkDerivation {
      pname = "${app}";
      version = "1.0";

      src = ./${app};

      nativeBuildInputs = [
        nixpkgs.pkg-config
        nixpkgs.patchelf
        nixpkgs.autoPatchelfHook
      ];

      buildInputs = [
        nixpkgs.xorg.libX11
        nixpkgs.xorg.libXinerama
        nixpkgs.xorg.libXft
        nixpkgs.xorg.libXi
        nixpkgs.xorg.xorgproto
        nixpkgs.xorg.libXrender
        nixpkgs.fontconfig
        nixpkgs.imlib2
      ];

      patchPhase = ''
        substituteInPlace config.mk \
          --replace '/usr/X11R6/include' "" \
          --replace '/usr/X11R6/lib' "" \
          --replace '/usr/include/freetype2' "" \
          --replace '-march=native' "" \
          --replace 'INCS =.*' 'INCS = $(shell pkg-config --cflags x11 xft xinerama xi) -I${nixpkgs.xorg.xorgproto}/include' \
          --replace 'LIBS =.*' 'LIBS = $(shell pkg-config --libs x11 xft xinerama xi xrender fontconfig imlib2)' \
          --replace 'CFLAGS   =.*' 'CFLAGS = -std=c99 -pedantic -Wall -Wno-deprecated-declarations $(INCS) $(CPPFLAGS)' \
          --replace 'LDFLAGS  =.*' 'LDFLAGS = $(LIBS)'
      '';

      installPhase = ''
        make PREFIX=$out install
      '';

      meta = with nixpkgs.lib; {
        description = "Builds ${app}";
        license = licenses.mit;
        platforms = platforms.unix;
      };
    };
in
  commonBuild ./${app}
