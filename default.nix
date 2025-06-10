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
      ];

      buildInputs = [
        nixpkgs.xorg.libX11
        nixpkgs.xorg.libXft
        nixpkgs.xorg.libXinerama
        nixpkgs.xorg.xrdb
        nixpkgs.alsa-lib
      ];

      installPhase = ''
        make PREFIX=$out install
      '';

      meta = with nixpkgs.lib; {
        description = "${app} builds ChadWM";
        license = licenses.gpl3Plus;
        platforms = platforms.unix;
      };
    };
in
  commonBuild ./${app}
