{
  description = "TRONG's ChadWM NixOS Flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }: {
    packages = {
      default = nixpkgs.lib.mkFlake {
        inherit self;
        packages = {
          dmenu = import ./default.nix { inherit nixpkgs; app = "dmenu"; };
        };
      };
    };

    nixosModules = {
      chadwm = ./modules/chadwm.nix;
    };
  };
}
